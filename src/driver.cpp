#include "../include/driver.hpp"
#include <cstring>
#include <fstream>
#include <sstream>
#include "../include/color.hpp"

extern FILE *yyin;
extern bool yy_flex_debug;

static std::string read_n_to_m_lines(std::istream &in_file, unsigned n,
                                     unsigned m) {
    assert(n <= m);

    std::string read_buf;
    std::stringstream retval;

    // skip n lines
    for (unsigned i = 1; i < n; i++) std::getline(in_file, read_buf);

    // read from n-th to m-th line
    for (unsigned i = n; i <= m; i++) {
        std::getline(in_file, read_buf);
        retval << read_buf;
    }

    return retval.str();
}

Driver::Driver() {}
void Driver::set_active_instance(Driver &driver) {
    m_active_instance = &driver;
}

const Driver &Driver::get_active_instance() { return *m_active_instance; }

void Driver::scan_begin() {
    yy_flex_debug = trace_scanning;
    if (this->file_path.empty()) {
        yyin = stdin;
    } else if (NULL == (yyin = fopen(this->file_path.c_str(), "r"))) {
        std::cerr << "Error opening file " << this->file_path << ": "
                  << ::strerror(errno) << "\n";
        exit(EXIT_FAILURE);
    }
}

void Driver::scan_end() { fclose(yyin); }

int Driver::parse(const std::string &file_path) {
    this->file_path = file_path;

    /* Start scanning */
    this->scan_begin();

    /* Making parser and forwarding it to bison */
    yy::parser myparser(*this);
    myparser.set_debug_level(trace_parsing);

    /* Parse file */
    int retval = myparser.parse();

    /* End scanning */
    this->scan_end();

    return retval;
}

void Driver::error(const yy::location &l, const std::string &msg) const {
    std::string preciding_ws(l.begin.column - 1, ' ');
    std::stringstream err_strm;

    Color::Modifier color_red(Color::FG_RED_BOLD);
    Color::Modifier color_blue(Color::FG_BLUE_BOLD);
    Color::Modifier color_def(Color::FG_DEFAULT);

    if (!file_path.empty()) {
        std::ifstream in_file(this->file_path);  // read file
        std::string lines_with_error(
            ::read_n_to_m_lines(in_file, l.begin.line, l.end.line));
        std::replace(lines_with_error.begin(), lines_with_error.end(), '\t',
                     ' ');  // replace tabs with spaces

        err_strm << color_red << "Error" << color_def << " in ";
        err_strm << color_blue << file_path << ":";
        err_strm << l << ": " << color_def << msg << '\n';
        err_strm << lines_with_error.substr(0, l.begin.column - 1) << color_blue
                 << lines_with_error.substr(l.begin.column - 1,
                                            l.end.column - l.begin.column)
                 << color_def
                 << lines_with_error.substr(l.end.column - 1,
                                            lines_with_error.length())
                 << '\n';
        err_strm << preciding_ws << color_blue << "^"
                 << std::string(l.end.column - l.begin.column - 1, '~')
                 << color_def << '\n';
    } else {
        // err_strm << preciding_ws << "^\n";
        // err_strm << l << ": " << msg << "\n";
        err_strm << color_red << "Error: " << color_def << msg << color_def
                 << '\n';
    }

    std::cerr << err_strm.str() << std::flush;
}


void Driver::warning(const yy::location &l, const std::string &msg) const {
    std::string preciding_ws(l.begin.column - 1, ' ');
    std::stringstream err_strm;

    Color::Modifier color_red(Color::FG_RED_BOLD);
    Color::Modifier color_blue(Color::FG_BLUE_BOLD);
    Color::Modifier color_def(Color::FG_DEFAULT);

    if (!file_path.empty()) {
        std::ifstream in_file(this->file_path);  // read file
        std::string lines_with_error(
            ::read_n_to_m_lines(in_file, l.begin.line, l.end.line));
        std::replace(lines_with_error.begin(), lines_with_error.end(), '\t',
                     ' ');  // replace tabs with spaces

        err_strm << color_red << "Warning" << color_def << " in ";
        err_strm << color_blue << file_path << ":";
        err_strm << l << ": " << color_def << msg << '\n';
        err_strm << lines_with_error.substr(0, l.begin.column - 1) << color_blue
                 << lines_with_error.substr(l.begin.column - 1,
                                            l.end.column - l.begin.column)
                 << color_def
                 << lines_with_error.substr(l.end.column - 1,
                                            lines_with_error.length())
                 << '\n';
        err_strm << preciding_ws << color_blue << "^"
                 << std::string(l.end.column - l.begin.column - 1, '~')
                 << color_def << '\n';
    } else {
        // err_strm << preciding_ws << "^\n";
        // err_strm << l << ": " << msg << "\n";
        err_strm << color_red << "Warning: " << color_def << msg << color_def
                 << '\n';
    }

    std::cerr << err_strm.str() << std::flush;
}

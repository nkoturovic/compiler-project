#ifndef __TERM_COLOR_H__
#define __TERM_COLOR_H__
#include <map>
#include <ostream>

namespace Color {
    enum Code {
        FG_DEFAULT,
        FG_RED,
        FG_RED_BOLD,
        FG_GREEN,
        FG_GREEN_BOLD,
        FG_BLUE,
        FG_BLUE_BOLD,
    };

class Modifier {
    private:
    Code m_code;

    inline const static std::map<Code, std::string> code_to_modifier {
        { FG_DEFAULT, "0;39" },
        { FG_RED, "0;31" },
        { FG_RED_BOLD, "1;31" },
        { FG_GREEN, "0;32" },
        { FG_GREEN_BOLD, "1;32" },
        { FG_BLUE, "0;34" },
        { FG_BLUE_BOLD, "1;34" },
    };

    public:
        Modifier(Code code) : m_code(code) {}
        friend std::ostream&

        operator<<(std::ostream& os, const Modifier& mod) {
            if (auto it = code_to_modifier.find(mod.m_code); it != code_to_modifier.end()) 
                os << "\033[" << it->second << "m";
            return os;
        }
    };

}; // end ns

#endif

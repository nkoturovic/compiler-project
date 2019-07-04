#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__

#include <map>
#include <string>
#include "structs.hpp"
#include "parser.tab.hpp"

/* Tell Flex the lexer's prototype ... */
#define YY_DECL\
       	yy::parser::symbol_type yylex(Driver& driver)
/* And Declare it for the parser's sake. */
YY_DECL;

class Driver {
public:
	/* File being processed */
	std::string file_path;

	/* Processing options */
	bool trace_scanning = false;
	bool trace_parsing = false;

	/* Methods needed by bison */
	void scan_begin();
	void scan_end();
	int parse(const std::string & file_path = "");
	void error(const yy::location &l, const std::string &msg);
};

#endif

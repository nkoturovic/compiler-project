#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <stack>
#include "location.hh"

namespace cpl {

struct Error {
    yy::location loc;
    std::string msg;
    Error(yy::location loc, std::string msg);
};

}

#endif

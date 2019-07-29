#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <stack>
#include "lang/types.hpp"
#include "location.hh"
#include "third_party/polymorphic_value.h"

namespace compiler::structs {

struct Error {
    yy::location loc;
    std::string msg;
};

struct FuncArg {
    jbcoe::polymorphic_value<lang::types::Type> type;
    std::string name{};
};

}  // namespace compiler::structs

#endif

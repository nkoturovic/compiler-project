#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <vector>
#include "lang/types.hpp"
#include "location.hh"
#include "third_party/polymorphic_value.h"

namespace llvm {class Value;}

namespace compiler::structs {

struct Error {
    yy::location loc;
    std::string msg;
};

struct FuncArg {
    jbcoe::polymorphic_value<lang::types::Type> type;
    std::string name{};
};

struct FuncProto {
   std::string name;
   std::vector<structs::FuncArg> args;
   jbcoe::polymorphic_value<lang::types::Type> retval_t;
};

struct TypeValuePair {
    jbcoe::polymorphic_value<lang::types::Type> type;
    llvm::Value *value;
};



}  // namespace compiler::structs

#endif

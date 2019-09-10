#ifndef __STRUCTS_H__
#define __STRUCTS_H__

#include <vector>
#include <functional>
#include "lang/types.hpp"
#include "codegen/codegen.hpp"
#include "location.hh"
#include "third_party/polymorphic_value.h"

namespace llvm {class Value;}
namespace compiler::ast {class Expression;}

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

bool operator==(const FuncProto &fp1, const FuncProto &fp2);
bool operator!=(const FuncProto &fp1, const FuncProto &fp2);

struct TypeValuePair {
    jbcoe::polymorphic_value<lang::types::Type> type;
    llvm::Value *value;
};

struct TypeCodegenFuncPair {
    jbcoe::polymorphic_value<lang::types::Type> type;
    std::function<llvm::Value*()> codegen_func;
};

struct LocProtoFuncTriple {
    yy::location loc;
    FuncProto proto;
    llvm::Function* func;
};

struct StrOptExprPair {
    std::string str;
    std::optional<jbcoe::polymorphic_value<ast::Expression>> expr;
};


}  // namespace compiler::structs

#endif

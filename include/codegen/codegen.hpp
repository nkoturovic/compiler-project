#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include <functional>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include "llvm/IR/NoFolder.h"

#include <llvm/IR/Type.h>
#include "llvm/IR/Function.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/BasicBlock.h"

#include "../lang/types.hpp"

namespace compiler::codegen {

struct global {
    static inline llvm::LLVMContext context = llvm::LLVMContext();
    static inline std::unique_ptr<llvm::Module> module =
        std::make_unique<llvm::Module>("top", context);

    /* TODO: Disable no FOLDER */
    static inline llvm::IRBuilder<llvm::NoFolder> builder =
        llvm::IRBuilder<llvm::NoFolder>(context);
};

llvm::Type* llvm_type(jbcoe::polymorphic_value<lang::types::Type> type);

}  // namespace compiler::codegen

#endif

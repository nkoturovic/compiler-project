#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include <functional>

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include "llvm/IR/NoFolder.h"

#include <llvm/IR/Type.h>
#include "llvm/ADT/APInt.h"
#include <llvm/IR/DerivedTypes.h>
#include "llvm/IR/Function.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/LegacyPassManager.h"

#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/IR/AssemblyAnnotationWriter.h"


#include "../lang/types.hpp"

namespace compiler::codegen {

struct global {
    static inline llvm::LLVMContext context = llvm::LLVMContext();
    static inline std::unique_ptr<llvm::Module> module =
        std::make_unique<llvm::Module>("top", context);
    static inline std::unique_ptr<llvm::legacy::FunctionPassManager> fpass_manager = 
        std::make_unique<llvm::legacy::FunctionPassManager>(&*module);

    /* TODO: Disable no FOLDER */
    static inline llvm::IRBuilder<> builder =
        llvm::IRBuilder<>(context);
};

llvm::Type* llvm_type(jbcoe::polymorphic_value<lang::types::Type> type);

}  // namespace compiler::codegen

#endif

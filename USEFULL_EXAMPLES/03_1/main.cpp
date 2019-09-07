#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>

#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/NoFolder.h"
#include <vector>
#include <string>

int main()
{
  //; ModuleID = 'top'
  llvm::LLVMContext context;
  llvm::Module* module = new llvm::Module("top", context);
  llvm::IRBuilder<llvm::NoFolder> builder(context);

  // declare void @main()
  llvm::FunctionType *funcType =
      llvm::FunctionType::get(builder.getInt32Ty(), false);
  llvm::Function *mainFunc =
      llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, "main", module);

  // define void(i32?) @main() { entrypoint: }
  llvm::BasicBlock *entry = llvm::BasicBlock::Create(context, "entrypoint", mainFunc);
  builder.SetInsertPoint(entry);

  // @0 = internal(private?) unnamed_addr constant [14 x i8] c"hello world!\0A\00"
  llvm::Value *helloWorld = builder.CreateGlobalStringPtr("hello world!");

  std::vector<llvm::Type *> putsArgs;
  putsArgs.push_back(builder.getInt8Ty()->getPointerTo());
  llvm::ArrayRef<llvm::Type*> argsRef(putsArgs);

  // declare i32 @puts(i8*)
  llvm::FunctionType *putsType =
  llvm::FunctionType::get(builder.getInt32Ty(), argsRef, false);
  llvm::Constant *putsFunc = module->getOrInsertFunction("puts", putsType);

  builder.CreateCall(putsFunc, helloWorld);

  // create 0
  llvm::Value * retval = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context), 0);
  builder.CreateRet(retval); // return 0

  module->print(llvm::outs(), nullptr);
}

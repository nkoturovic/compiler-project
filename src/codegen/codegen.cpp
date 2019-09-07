#include "../../include/codegen/codegen.hpp"

using namespace compiler::lang::types;

namespace compiler::codegen {

llvm::Type* llvm_type(jbcoe::polymorphic_value<lang::types::Type> type) {
    if (type->id <= TypeId::INT) {
        return llvm::Type::getIntNTy(global::context, type->size() * 8);
    } else if (type->id == TypeId::DOUBLE) {
        return llvm::Type::getDoubleTy(global::context);
    } else if (type->id == TypeId::PTR) {
        // TODO - POINTER TYPE
    }
    return nullptr;
}

}  // namespace compiler::codegen

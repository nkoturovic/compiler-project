#include "../../include/lang/semantic.hpp"

using poly_type = jbcoe::polymorphic_value<compiler::lang::types::Type>;
using namespace compiler::lang::types;
using namespace compiler::lang::operators;

namespace compiler::lang::semantic {

jbcoe::polymorphic_value<Type> binop_check(
    BinOpId op_id, const jbcoe::polymorphic_value<Type> &op1_t,
    const jbcoe::polymorphic_value<Type> &op2_t) {
    // INVALID op ANY -> INVALID
    if (op1_t->id == TypeId::INVALID || op2_t->id == TypeId::INVALID)
        return poly_type(InvalidType());

    // VOID op ANY -> INVALID
    if (op1_t->id == TypeId::VOID || op2_t->id == TypeId::VOID)
        return poly_type(InvalidType());

    // mod INT, INT -> INT else ERROR
    if (op_id == BinOpId::MOD || op_id == BinOpId::SHL ||
        op_id == BinOpId::SHR) {
        if (op1_t->id <= TypeId::INT && op2_t->id <= TypeId::INT)
            return poly_type(IntType());
        else
            return poly_type(InvalidType());
    }

    // PTR +- INT -> PTR else ERROR
    if (op1_t->id == TypeId::PTR && op2_t->id <= TypeId::INT) {
        if (op_id != BinOpId::PLUS && op_id != BinOpId::MINUS) {
            return poly_type(InvalidType());
        } else {
            return op1_t;
        }
    } else if (op1_t->id <= TypeId::INT && op2_t->id == TypeId::PTR) {
        if (op_id != BinOpId::PLUS && op_id != BinOpId::MINUS) {
            return poly_type(InvalidType());
        } else {
            return op2_t;
        }
    } else if (op1_t->id == TypeId::PTR || op2_t->id == TypeId::PTR) {
        return poly_type(InvalidType());
    }

    // Down and up cast to INT for some operators
    switch (op_id) {
        case BinOpId::EQ:
        case BinOpId::NEQ:
        case BinOpId::LT:
        case BinOpId::GT:
        case BinOpId::LEQ:
        case BinOpId::GEQ:
        case BinOpId::L_AND:
        case BinOpId::L_OR:
        case BinOpId::B_AND:
        case BinOpId::B_OR:
        case BinOpId::B_XOR:
            return poly_type(IntType());
        default:
            break;
    }

    // is basic type check (if type <= DOUBLE than is Basic type)
    if (op1_t->id <= TypeId::DOUBLE || op2_t->id <= TypeId::DOUBLE) {
        auto max_t = op1_t->id > op2_t->id ? op1_t : op2_t;
        max_t = max_t->id >= TypeId::INT ? max_t : poly_type(IntType());
        //std::cout << op1_t->str() << "," << op2_t->str() << "->";
        //std::cout << max_t->str() << std::endl;
        return max_t;
    }

    return poly_type(InvalidType());
}

jbcoe::polymorphic_value<Type> unop_check(
    UnOpId op_id, const jbcoe::polymorphic_value<Type> &type) {
    if (op_id == UnOpId::INVALID || type->id == TypeId::INVALID ||
        type->id == TypeId::VOID)
        return poly_type(InvalidType());
    return poly_type(type);
}


structs::TypeCodegenFuncPair convert(const structs::TypeValuePair &type_value_pair, jbcoe::polymorphic_value<types::Type> to_type) {

    llvm::Value * retval = type_value_pair.value;
    if (*to_type == *type_value_pair.type)
        return { to_type, [type_value_pair](){ return type_value_pair.value;} };

    poly_type ret_type(to_type);
    poly_type invalid_type = poly_type(lang::types::InvalidType());

    if (to_type->id == lang::types::TypeId::DOUBLE && type_value_pair.type->id <= lang::types::TypeId::INT) {
        if (retval != nullptr)
            retval = codegen::global::builder.CreateSIToFP(retval, codegen::llvm_type(to_type), "sitofp");

    } else if (to_type->id <= lang::types::TypeId::INT && type_value_pair.type->id == lang::types::TypeId::DOUBLE) {
        if (retval != nullptr)
            retval = codegen::global::builder.CreateFPToSI(retval, codegen::llvm_type(to_type), "fptosi");

    } else if (to_type->id == lang::types::TypeId::INT && type_value_pair.type->id <= lang::types::TypeId::INT) {
            retval = codegen::global::builder.CreateIntCast(retval, codegen::llvm_type(poly_type(lang::types::IntType())), true);
    } else if (to_type->id == lang::types::TypeId::PTR && type_value_pair.type->id == lang::types::TypeId::PTR) {
        ret_type = invalid_type; retval = nullptr;
    } else {
        ret_type = invalid_type; retval = nullptr;
    }
    auto ret_func = [retval] { return retval; };

    return {ret_type, ret_func};
}

}  // namespace compiler::lang::semantic

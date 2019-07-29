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
        const BasicType &op1_basic_t = dynamic_cast<const BasicType &>(*op1_t);
        const BasicType &op2_basic_t = dynamic_cast<const BasicType &>(*op2_t);
        return poly_type(std::max<BasicType>(
            std::max<BasicType>(op1_basic_t, op2_basic_t), IntType()));
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

}  // namespace compiler::lang::semantic

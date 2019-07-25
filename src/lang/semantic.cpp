#include "../../include/lang/semantic.hpp"

namespace cpl::lang {

inline auto poly_type(auto derived) {
    return jbcoe::polymorphic_value<DataType>(derived);
}

jbcoe::polymorphic_value<DataType> Semantic::binop_check(BinOpId op_id, const jbcoe::polymorphic_value<DataType> &op1_t, const jbcoe::polymorphic_value<DataType> &op2_t) {
    // INVALID op ANY -> INVALID
    if (op1_t->id == DataTypeId::INVALID || op2_t->id == DataTypeId::INVALID)
        return poly_type(InvalidType());

    // VOID op ANY -> INVALID
    if (op1_t->id == DataTypeId::VOID || op2_t->id == DataTypeId::VOID)
        return poly_type(InvalidType());

    // mod INT, INT -> INT else ERROR
    if (op_id == BinOpId::MOD || op_id == BinOpId::SHL || op_id == BinOpId::SHR) {
        if (op1_t->id <= DataTypeId::INT && op2_t->id <= DataTypeId::INT)
            return poly_type(BasicType(DataTypeId::INT));
        else 
            return poly_type(InvalidType());
    }

    // PTR +- INT -> PTR else ERROR
    if (op1_t->id == DataTypeId::PTR && op2_t->id <= DataTypeId::INT) {
        if (op_id != BinOpId::PLUS && op_id != BinOpId::MINUS) {
            return poly_type(InvalidType());
        } else {
            return op1_t;
        }
    } else if (op1_t->id <= DataTypeId::INT && op2_t->id == DataTypeId::PTR) {
        if (op_id != BinOpId::PLUS && op_id != BinOpId::MINUS) {
            return poly_type(InvalidType());
        } else {
            return op2_t;
        }
    } else if (op1_t->id == DataTypeId::PTR || op2_t->id == DataTypeId::PTR) {
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
           return poly_type(BasicType(DataTypeId::INT));
       default: break;
    }

    // is basic type check (if type <= DOUBLE than is Basic type)
    if (op1_t->id <= DataTypeId::DOUBLE || op2_t->id <= DataTypeId::DOUBLE) {
        const BasicType &op1_basic_t = dynamic_cast<const BasicType&>(*op1_t);
        const BasicType &op2_basic_t = dynamic_cast<const BasicType&>(*op1_t);
        return poly_type(std::max(std::max(op1_basic_t, op2_basic_t), BasicType(DataTypeId::INT)));
    }

    return poly_type(InvalidType());
}

jbcoe::polymorphic_value<DataType> Semantic::unop_check(UnOpId op_id, const jbcoe::polymorphic_value<DataType> &type) {
    if (op_id == UnOpId::INVALID || type->id == DataTypeId::INVALID || type->id == DataTypeId::VOID)
        return poly_type(InvalidType());
    return poly_type(type);
}

}

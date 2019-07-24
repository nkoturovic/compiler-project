#include "../../include/lang/semantic.hpp"

namespace cpl::lang {

DataType Semantic::binop_check(BinOpId op_id, const DataType &op1_t, const DataType &op2_t) {
    // INVALID op ANY -> INVALID
    if (op1_t.id == DataTypeId::INVALID || op2_t.id == DataTypeId::INVALID)
        return InvalidType();

    // VOID op ANY -> INVALID
    if (op1_t.id == DataTypeId::VOID || op2_t.id == DataTypeId::VOID)
        return InvalidType();

    // mod INT, INT -> INT else ERROR
    if (op_id == BinOpId::MOD || op_id == BinOpId::SHL || op_id == BinOpId::SHR) {
        if (op1_t.id <= DataTypeId::INT && op2_t.id <= DataTypeId::INT)
            return BasicType(DataTypeId::INT);
        else 
            return InvalidType();
    }

    // PTR +- INT -> PTR else ERROR
    if (op1_t.id == DataTypeId::PTR && op2_t.id <= DataTypeId::INT) {
        if (op_id != BinOpId::PLUS && op_id != BinOpId::MINUS) 
            return InvalidType();
        else 
            return op1_t;
       
    } else if (op1_t.id <= DataTypeId::INT && op2_t.id == DataTypeId::PTR) {
        if (op_id != BinOpId::PLUS && op_id != BinOpId::MINUS) 
            return InvalidType();
        else 
            return op2_t;
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
           return BasicType(DataTypeId::INT);
       default: break;
    }

    return std::max(std::max(op1_t, op2_t), (DataType) BasicType(DataTypeId::INT));
}

DataType Semantic::unop_check(UnOpId op_id, const DataType &type) {
    if (op_id == UnOpId::INVALID || type.id == DataTypeId::INVALID || type.id == DataTypeId::VOID)
        return InvalidType();
    return type;
}

}

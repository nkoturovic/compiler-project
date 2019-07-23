#include "../../include/lang/semantic.hpp"

namespace cpl::lang {

DataType Semantic::binop_check(BinOpId op_id, DataType op1_t, DataType op2_t) {
    // INVALID op ANY -> INVALID
    if (op1_t == DataType::INVALID || op2_t == DataType::INVALID)
        return DataType::INVALID;

    // VOID op ANY -> INVALID
    if (op1_t == DataType::VOID || op2_t == DataType::VOID)
        return DataType::INVALID;

    // mod INT, INT -> INT else ERROR
    if (op_id == BinOpId::MOD || op_id == BinOpId::SHL || op_id == BinOpId::SHR) {
        if (op1_t <= DataType::INT && op2_t <= DataType::INT)
            return DataType::INT;
        else 
            return DataType::INVALID;
    }

    // PTR +- INT -> PTR else ERROR
    if ((op1_t == DataType::PTR && op2_t <= DataType::INT) ||
     (op1_t <= DataType::INT && op2_t == DataType::PTR)) {
        if (op_id != BinOpId::PLUS && op_id != BinOpId::MINUS) 
            return DataType::INVALID;
        else 
            return DataType::PTR;
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
           return DataType::INT;
       default: break;
    }

    return std::max(std::max(op1_t, op2_t), DataType::INT);
}

}

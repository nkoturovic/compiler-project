#include "../../include/lang/semantic.hpp"

namespace cpl::lang {

DataType Semantic::binop_check(BinOpId op_id, DataType op1_t, DataType op2_t) {

    // TODO: Start implementing BinOp Semantic checks
    if (op_id == BinOpId::PLUS && op1_t == DataType::INT && op2_t == DataType::INT)
        return DataType::INVALID;

    return std::max(std::max(op1_t, op2_t), DataType::INT);
}

}

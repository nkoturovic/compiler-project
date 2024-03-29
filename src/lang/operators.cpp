#include "../../include/lang/operators.hpp"

namespace compiler::lang::operators {

std::string BinOpInfo::binop_id_to_string(BinOpId op_id) {
    return binop_id_to_string_table.at(op_id);
}

std::string UnOpInfo::unop_id_to_string(UnOpId op_id) {
    return unop_id_to_string_table.at(op_id);
}

}  // namespace compiler::lang::operators

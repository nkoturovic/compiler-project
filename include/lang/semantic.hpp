#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#include "operators.hpp"
#include "types.hpp"
#include "../structs.hpp"

namespace compiler::lang::semantic {

jbcoe::polymorphic_value<types::Type> binop_check(
    operators::BinOpId op_id, const jbcoe::polymorphic_value<types::Type> &op1,
    const jbcoe::polymorphic_value<types::Type> &op2);
jbcoe::polymorphic_value<types::Type> unop_check(
    operators::UnOpId op_id, const jbcoe::polymorphic_value<types::Type> &type);

structs::TypeCodegenFuncPair convert(const structs::TypeValuePair &type_value_pair, jbcoe::polymorphic_value<types::Type> to_type);

};  // namespace compiler::lang::semantic

#endif

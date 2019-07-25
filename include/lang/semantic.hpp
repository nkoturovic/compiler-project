#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#include "data_types.hpp"
#include "operators.hpp"

namespace cpl::lang {

class Semantic {
public:
    Semantic() = delete;
    static jbcoe::polymorphic_value<DataType> binop_check(BinOpId op_id, const jbcoe::polymorphic_value<DataType> &op1, const jbcoe::polymorphic_value<DataType> &op2);
    static jbcoe::polymorphic_value<DataType> unop_check(UnOpId op_id, const jbcoe::polymorphic_value<DataType> &type);
};

}

#endif

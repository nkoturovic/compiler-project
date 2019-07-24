#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#include "data_types.hpp"
#include "operators.hpp"

namespace cpl::lang {

class Semantic {
public:
    Semantic() = delete;
    static DataType binop_check(BinOpId op_id, const DataType &op1, const DataType &op2);
    static DataType unop_check(UnOpId op_id, const DataType &type);
};

}

#endif

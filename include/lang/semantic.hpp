#ifndef __SEMANTIC_H__
#define __SEMANTIC_H__

#include "data_types.hpp"
#include "operators.hpp"

namespace cpl::lang {

class Semantic {
public:
    Semantic() = delete;
    static DataType binop_check(BinOpId op_id, DataType op1, DataType op2);
};

}

#endif

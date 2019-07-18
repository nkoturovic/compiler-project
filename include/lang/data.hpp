#ifndef __AST_DATA__
#define __AST_DATA__

#include "data_types.hpp"

namespace cpl::lang {

typedef int integer_t; // Type holder for all integers types
typedef double real_t; // Type holder for all real types

enum class DataRepresent {NONE=0, INTEGER=1, REAL=2, INVALID = 3};

class DataInfo {
public:
    inline const static std::map <DataType, DataRepresent> type_to_represent_table = {
        { DataType::VOID, DataRepresent::INVALID },
        { DataType::CHAR, DataRepresent::INTEGER },
        { DataType::INT, DataRepresent::INTEGER },
        { DataType::DOUBLE, DataRepresent::REAL },
        { DataType::INVALID, DataRepresent::INVALID }
    };
    DataInfo() = delete;
    static DataRepresent type_to_represent(DataType type);
};

class Data {
private:
    std::variant<integer_t, real_t> m_data;
public:
    DataType type = DataType::INVALID;

    Data();
    Data(DataType type, integer_t data);
    Data(DataType type, real_t data);
    Data& fix_precision();

    static Data convert(const Data &data, DataType type);
    static Data do_bin_op(BinOpId op, const Data &lhs, const Data &rhs, DataType res_type);

    friend std::ostream& operator << (std::ostream & out, const cpl::lang::Data& data);
};

std::ostream& operator << (std::ostream & out, const Data& data);

}


#endif

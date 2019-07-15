#ifndef __AST_DATA_TYPES__
#define __AST_DATA_TYPES__

#include <map>
#include <variant>

namespace ast {

typedef int integer_t; // Type holder for all integers types
typedef double real_t; // Type holder for all real types

/* Types for DataTypes and Data Reperesentation */
enum class DataType {NONE=0, CHAR = 1, INT=2, DOUBLE=3, INVALID = 4};
enum class DataRepresent {NONE=0, INTEGER=1, REAL=2, INVALID = 3};

/* Function declarations */
DataRepresent type_to_represent(DataType type);
std::size_t size_of(DataType type);

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
    static Data plus(DataType type, const Data &lhs, const Data &rhs);

    friend std::ostream& operator << (std::ostream & out, const Data& data);
};

std::ostream& operator << (std::ostream & out, const Data& data);

}

#endif

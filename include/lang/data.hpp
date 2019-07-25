#ifndef __AST_DATA__
#define __AST_DATA__

#include <variant>
#include "data_types.hpp"

namespace cpl::lang {

typedef intmax_t integer_t;  // Type holder for all integers types
typedef double real_t;       // Type holder for all real types

enum class DataRepresent { NONE = 0, INTEGER = 1, REAL = 2, INVALID = 3 };

class Data {
   private:
    std::variant<integer_t, real_t> m_data;

    inline const static std::map<DataTypeId, DataRepresent>
        m_type_to_represent_table = {
            {DataTypeId::VOID, DataRepresent::INVALID},
            {DataTypeId::CHAR, DataRepresent::INTEGER},
            {DataTypeId::INT, DataRepresent::INTEGER},
            {DataTypeId::PTR, DataRepresent::INTEGER},
            {DataTypeId::DOUBLE, DataRepresent::REAL},
            {DataTypeId::INVALID, DataRepresent::INVALID}};

   public:
    static DataRepresent type_to_represent(
        const jbcoe::polymorphic_value<DataType> &type);

    jbcoe::polymorphic_value<DataType> type;
    Data();
    Data(jbcoe::polymorphic_value<DataType> type, integer_t data);
    Data(jbcoe::polymorphic_value<DataType> type, int data);
    Data(jbcoe::polymorphic_value<DataType> type, real_t data);
    Data(jbcoe::polymorphic_value<DataType> type, const char *data);
    Data &fix_precision();

    static Data convert(const Data &data,
                        const jbcoe::polymorphic_value<DataType> &res_type);
    static Data do_bin_op(BinOpId op, const Data &lhs, const Data &rhs,
                          const jbcoe::polymorphic_value<DataType> &res_type);
    static Data do_un_op(UnOpId op, const Data &expr,
                         const jbcoe::polymorphic_value<DataType> &res_type);

    friend std::ostream &operator<<(std::ostream &out,
                                    const cpl::lang::Data &data);
};

std::ostream &operator<<(std::ostream &out, const Data &data);

}  // namespace cpl::lang

#endif

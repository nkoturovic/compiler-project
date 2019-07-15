#include "../../include/ast/data_types.hpp"
#include <ostream>
#include <cmath>

namespace ast {

DataRepresent type_to_represent(DataType type) {
    const static std::map <DataType, DataRepresent> type_to_represent_table = {
        { DataType::CHAR, DataRepresent::INTEGER },
        { DataType::INT, DataRepresent::INTEGER },
        { DataType::DOUBLE, DataRepresent::REAL },
        { DataType::INVALID, DataRepresent::INVALID }
    };
    return type_to_represent_table.at(type);
}

std::size_t size_of(DataType type) {
    const static std::map <DataType, std::size_t> type_to_size_table = {
        { DataType::CHAR, 1 },
        { DataType::INT, 4 },
        { DataType::DOUBLE, 8 },
        { DataType::INVALID, 0 }
    };
    return type_to_size_table.at(type);
}

/* Data Constructors */
Data::Data() : type(DataType::INVALID) {}
Data::Data(DataType type, integer_t data) : type(type) { m_data.emplace<integer_t>(data); this->fix_precision(); }
Data::Data(DataType type, real_t data) : type(type) { m_data.emplace<real_t>(data); this->fix_precision(); }

std::ostream& operator << (std::ostream & out, const Data& data) {

    DataRepresent rep = type_to_represent(data.type);

    if (rep == DataRepresent::INTEGER) {
        integer_t val = std::get<integer_t>(data.m_data);
        if (data.type == DataType::CHAR)
            out << static_cast<char>(val);
        else if (data.type == DataType::INT)
            out << static_cast<int>(val);
    } else if (rep == DataRepresent::REAL) {
        real_t val = std::get<real_t>(data.m_data);
        if (data.type == DataType::DOUBLE)
            out << val;
    }
    return out;
}

Data Data::plus(DataType type, const Data &lhs, const Data &rhs) {
    DataType to_type = type;

    if (type < DataType::INT)
        to_type = DataType::INT;

    Data op1 = Data::convert(lhs, to_type);
    Data op2 = Data::convert(rhs, to_type);
    DataRepresent type_repr = type_to_represent(to_type);

    if (type_repr == DataRepresent::INTEGER) 
        return Data(to_type, std::get<integer_t>(op1.m_data) + std::get<integer_t>(op2.m_data)).fix_precision();
    else if (type_repr == DataRepresent::REAL) 
        return Data(to_type, std::get<real_t>(op1.m_data) + std::get<real_t>(op2.m_data)).fix_precision();
    else 
        return Data();
}

Data& Data::fix_precision() {
    DataRepresent repr = type_to_represent(this->type); 
    if (repr == DataRepresent::INTEGER) {
            this->m_data.emplace<integer_t>(std::get<integer_t>(this->m_data) % static_cast<int>(pow(2, size_of(this->type) * 8)-1));
    } else if (repr == DataRepresent::REAL) {
        if (size_of(this->type) == sizeof(float)) 
            this->m_data.emplace<real_t>(static_cast<float>(std::get<real_t>(this->m_data)));
         else if (size_of(this->type) == sizeof(double))
            this->m_data.emplace<real_t>(static_cast<double>(std::get<real_t>(this->m_data)));
    }
    return *this;
}

Data Data::convert(const Data &from, DataType type) {
    Data to = from;
    to.type = type;

    if (from.type == to.type)
        return to;

    DataRepresent from_repr = type_to_represent(from.type); 
    DataRepresent to_repr = type_to_represent(type); 

    /* Type representation */
    if (from_repr != to_repr) {
        if (from_repr == DataRepresent::INTEGER && to_repr == DataRepresent::REAL)
            to.m_data.emplace<real_t>(std::get<integer_t>(to.m_data));
        else if (from_repr == DataRepresent::REAL && to_repr == DataRepresent::INTEGER)
            to.m_data.emplace<integer_t>(std::get<real_t>(to.m_data));
        else
            to.type = DataType::INVALID;
    }

    to.fix_precision(); 

    return to;
}

}

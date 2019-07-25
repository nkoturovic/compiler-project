#include "../../include/lang/data.hpp"
#include "../../include/lang/data_types.hpp"
#include <ostream>
#include <iostream>
#include <cmath>

namespace cpl::lang {

DataRepresent Data::type_to_represent(const jbcoe::polymorphic_value<DataType> &type) {
   return m_type_to_represent_table.at(type->id);
}

/* Data Constructors */
Data::Data() : type(InvalidType()) {}
Data::Data(jbcoe::polymorphic_value<DataType> type, integer_t data) : type(std::move(type)) { m_data.emplace<integer_t>(data); this->fix_precision(); }
Data::Data(jbcoe::polymorphic_value<DataType> type, int data) : Data(std::move(type), static_cast<integer_t>(data)) { }
Data::Data(jbcoe::polymorphic_value<DataType> type, const char * data) : Data(std::move(type), static_cast<integer_t>(reinterpret_cast<std::intptr_t>(data))) { }
Data::Data(jbcoe::polymorphic_value<DataType> type, real_t data) : type(std::move(type)) { m_data.emplace<real_t>(data); this->fix_precision(); }

Data Data::do_bin_op(BinOpId op, const Data &lhs, const Data &rhs, const jbcoe::polymorphic_value<DataType> &res_type) {

    if (res_type->id == DataTypeId::INVALID || lhs.type->id == DataTypeId::INVALID || rhs.type->id == DataTypeId::INVALID)
        return Data();

    if (lhs.type->id == DataTypeId::PTR && rhs.type->id <= DataTypeId::INT) {
        integer_t o1 = std::get<integer_t>(lhs.m_data);
        integer_t o2 = std::get<integer_t>(rhs.m_data); 
        const PointerType &ptt = dynamic_cast<const PointerType&>(*lhs.type);

        switch (op) {
            case BinOpId::PLUS : return Data(res_type, static_cast<integer_t>(o1 + o2*ptt.points_to->size())).fix_precision();
            case BinOpId::MINUS : return Data(res_type, static_cast<integer_t>(o1 - o2*ptt.points_to->size())).fix_precision();
            default: return Data();
        }

    } else if (lhs.type->id <= DataTypeId::INT && rhs.type->id == DataTypeId::PTR) {
        integer_t o1 = std::get<integer_t>(lhs.m_data);
        integer_t o2 = std::get<integer_t>(rhs.m_data); 
        const PointerType &ptt = dynamic_cast<const PointerType&>(*rhs.type);

        switch (op) {
            case BinOpId::PLUS : return Data(res_type, static_cast<integer_t>(o1*ptt.points_to->size() + o2)).fix_precision();
            default: return Data();
        }
    }

    Data op1 = Data::convert(lhs, res_type);
    Data op2 = Data::convert(rhs, res_type);
    DataRepresent type_repr = Data::type_to_represent(res_type);

    if (type_repr == DataRepresent::INTEGER) {
        integer_t o1 = std::get<integer_t>(op1.m_data); 
        integer_t o2 = std::get<integer_t>(op2.m_data);
        switch (op) {
            case BinOpId::PLUS : return Data(res_type, o1 + o2).fix_precision();
            case BinOpId::MINUS : return Data(res_type, o1 - o2).fix_precision();
            case BinOpId::MUL : return Data(res_type, o1 * o2).fix_precision();
            case BinOpId::DIV : if (o2 == 0) return Data();
                                else return Data(res_type, o1 / o2).fix_precision();
            case BinOpId::MOD : if (o2 == 0) return Data();
                                return Data(res_type, o1 % o2).fix_precision();
            case BinOpId::EQ : return Data(res_type, o1 == o2).fix_precision();
            case BinOpId::NEQ : return Data(res_type, o1 != o2).fix_precision();
            case BinOpId::LT : return Data(res_type, o1 < o2).fix_precision();
            case BinOpId::GT : return Data(res_type, o1 > o2).fix_precision();
            case BinOpId::LEQ : return Data(res_type, o1 <= o2).fix_precision();
            case BinOpId::GEQ : return Data(res_type, o1 >= o2).fix_precision();
            case BinOpId::L_AND : return Data(res_type, o1 && o2).fix_precision();
            case BinOpId::L_OR : return Data(res_type, o1 || o2).fix_precision();
            case BinOpId::B_AND : return Data(res_type, o1 & o2).fix_precision();
            case BinOpId::B_OR : return Data(res_type, o1 | o2).fix_precision();
            case BinOpId::B_XOR : return Data(res_type, o1 ^ o2).fix_precision();
            case BinOpId::SHL : return Data(res_type, o1 << o2).fix_precision();
            case BinOpId::SHR : return Data(res_type, o1 >> o2).fix_precision();
            default : return Data();
        }
    }
    else if (type_repr == DataRepresent::REAL) {
        real_t o1 = std::get<real_t>(op1.m_data); 
        real_t o2 = std::get<real_t>(op2.m_data);
        switch (op) {
            case BinOpId::PLUS : return Data(res_type, o1 + o2).fix_precision();
            case BinOpId::MINUS : return Data(res_type, o1 - o2).fix_precision();
            case BinOpId::MUL : return Data(res_type, o1 * o2).fix_precision();
            case BinOpId::DIV : return Data(res_type, o1 / o2).fix_precision();
            //case BinOpId::MOD : return Data(res_type, o1 % o2).fix_precision();
            case BinOpId::EQ : return Data(res_type, o1 == o2).fix_precision();
            case BinOpId::NEQ : return Data(res_type, o1 != o2).fix_precision();
            case BinOpId::LT : return Data(res_type, o1 < o2).fix_precision();
            case BinOpId::GT : return Data(res_type, o1 > o2).fix_precision();
            case BinOpId::LEQ : return Data(res_type, o1 <= o2).fix_precision();
            case BinOpId::GEQ : return Data(res_type, o1 >= o2).fix_precision();
            case BinOpId::L_AND : return Data(res_type, o1 && o2).fix_precision();
            case BinOpId::L_OR : return Data(res_type, o1 || o2).fix_precision();
            /*case BinOpId::B_AND : return Data(res_type, o1 & o2).fix_precision();*/
            /*case BinOpId::B_OR : return Data(res_type, o1 | o2).fix_precision();*/
            /*case BinOpId::B_XOR : return Data(res_type, o1 ^ o2).fix_precision();*/
            /*case BinOpId::SHL : return Data(res_type, o1 << o2).fix_precision();*/
            /*case BinOpId::SHR : return Data(res_type, o1 >> o2).fix_precision();*/
            default : return Data();
        }
    } else {
        return Data();
    }
}

Data Data::do_un_op(UnOpId op, const Data &expr, const jbcoe::polymorphic_value<DataType> &res_type) {

    Data operand = Data::convert(expr, res_type);
    DataRepresent type_repr = Data::type_to_represent(res_type);

    if (type_repr == DataRepresent::INTEGER) {
        integer_t o = std::get<integer_t>(operand.m_data); 
        switch (op) {
            case UnOpId::PLUS : return Data(res_type, o).fix_precision();
            case UnOpId::MINUS : return Data(res_type, -o).fix_precision();
            case UnOpId::L_NOT : return Data(res_type, !o).fix_precision();
            case UnOpId::B_NOT : return Data(res_type, ~o).fix_precision();
            default : return Data();
        }
    }
    else if (type_repr == DataRepresent::REAL) {
        real_t o = std::get<real_t>(operand.m_data); 
        switch (op) {
            case UnOpId::PLUS : return Data(res_type, o).fix_precision();
            case UnOpId::MINUS : return Data(res_type, -o).fix_precision();
            case UnOpId::L_NOT : return Data(res_type, !o).fix_precision();
            default : return Data();
        }
    }
    return Data();
}

Data& Data::fix_precision() {
    DataRepresent repr = Data::type_to_represent(this->type); 
    if (repr == DataRepresent::INTEGER) {
            this->m_data.emplace<integer_t>(std::get<integer_t>(this->m_data) % static_cast<integer_t>(pow(2, this->type->size() * 8)-1));
    } else if (repr == DataRepresent::REAL) {
        if (this->type->size() == sizeof(float)) 
            this->m_data.emplace<real_t>(static_cast<float>(std::get<real_t>(this->m_data)));
         else if (this->type->size() == sizeof(double))
            this->m_data.emplace<real_t>(static_cast<double>(std::get<real_t>(this->m_data)));
    }
    return *this;
}

Data Data::convert(const Data &from, const jbcoe::polymorphic_value<DataType> &type) {
    Data to = from;
    to.type = type;

    if (from.type->id == to.type->id)
        return to;

    DataRepresent from_repr = Data::type_to_represent(from.type); 
    DataRepresent to_repr = Data::type_to_represent(type); 

    /* Type representation */
    if (from_repr != to_repr) {
        if (from_repr == DataRepresent::INTEGER && to_repr == DataRepresent::REAL)
            to.m_data.emplace<real_t>(std::get<integer_t>(to.m_data));
        else if (from_repr == DataRepresent::REAL && to_repr == DataRepresent::INTEGER)
            to.m_data.emplace<integer_t>(std::get<real_t>(to.m_data));
        else
            to.type = jbcoe::polymorphic_value<DataType>(InvalidType());
    }

    to.fix_precision(); 

    return to;
}

std::ostream& operator << (std::ostream &out, const cpl::lang::Data& data) {

    DataRepresent rep = Data::type_to_represent(data.type);

    if (rep == DataRepresent::INTEGER) {
        integer_t val = std::get<integer_t>(data.m_data);
        if (data.type->id == DataTypeId::CHAR) {
            out << static_cast<char>(val);
        } else if (data.type->id == DataTypeId::INT) {
            out << static_cast<int>(val);
        } else if (data.type->id == DataTypeId::PTR) {
            const PointerType &pt = dynamic_cast<const PointerType&>(*data.type);
            if (pt.points_to->id == DataTypeId::CHAR) {
              const char * str = reinterpret_cast<const char*>(static_cast<intptr_t>(std::get<integer_t>(data.m_data)));
              out << str;
            }
        }
    } else if (rep == DataRepresent::REAL) {
        real_t val = std::get<real_t>(data.m_data);
        if (data.type->id == DataTypeId::DOUBLE)
            out << val;
    } else if (rep == DataRepresent::INVALID) {
        out << "Data::operator << : Invalid data type";
    }

    return out;
}


} // end ns

#include "../../include/lang/data_types.hpp"
#include <cmath>
#include <ostream>

namespace cpl::lang {

DataType::DataType(DataTypeId type_id) : id(type_id) {}

bool DataType::operator==(const DataType &rhs) const {
    if (this->id != rhs.id) {
        return false;
    } else if (this->id == DataTypeId::PTR) /* => rhs.id == DataType::PTR */ {
        const PointerType &t1 = dynamic_cast<const PointerType &>(*this);
        const PointerType &t2 = dynamic_cast<const PointerType &>(rhs);

        if (t1 == t2)
            return true;
        else
            return false;
    } else {
        return true;
    }
    return false;
}

bool DataType::operator!=(const DataType &rhs) const { return !(*this == rhs); }
bool DataType::operator<(const DataType &rhs) const {
    return this->id < rhs.id;
}
bool DataType::operator<=(const DataType &rhs) const {
    return *this < rhs || *this == rhs;
}
bool DataType::operator>(const DataType &rhs) const {
    return this->id > rhs.id;
}
bool DataType::operator>=(const DataType &rhs) const {
    return *this > rhs || *this == rhs;
}

InvalidType::InvalidType() : DataType(DataTypeId::INVALID) {}
std::size_t InvalidType::size() const { return m_type_size; }
std::string InvalidType::str() const { return m_str; }

BasicType::BasicType(DataTypeId type_id) : DataType(type_id) {}
std::size_t BasicType::size() const {
    return m_basic_type_to_size_table.at(this->id);
}
std::string BasicType::str() const {
    return m_basic_type_to_string_table.at(this->id);
}

PointerType::PointerType(jbcoe::polymorphic_value<DataType> points_to)
    : DataType(DataTypeId::PTR), points_to(std::move(points_to)) {}
bool PointerType::operator==(const PointerType &rhs) const {
    if (this->id == rhs.id &&
        *this->points_to == *rhs.points_to)  // TODO: maybe bug
        return true;
    return false;
}

std::size_t PointerType::size() const { return m_type_size; }
std::string PointerType::str() const { return this->points_to->str() + "*"; }

}  // namespace cpl::lang

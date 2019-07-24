#include "../../include/lang/data_types.hpp"
#include <ostream>
#include <cmath>

namespace cpl::lang {

DataType::DataType(DataTypeId type_id) : id(type_id) { }

bool DataType::operator==(const DataType &rhs) const { 
    if (this->id != rhs.id) {
        return false;
    } else if (this->id == DataTypeId::PTR) /* => rhs.id == DataType::PTR */ {
        const PointerType &t1 = dynamic_cast<const PointerType&>(*this);
        const PointerType &t2 = dynamic_cast<const PointerType&>(rhs);

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
bool DataType::operator<(const DataType &rhs) const { return this->id < rhs.id; }
bool DataType::operator<=(const DataType &rhs) const { return *this < rhs || *this == rhs; }
bool DataType::operator>(const DataType &rhs) const { return this->id > rhs.id; }
bool DataType::operator>=(const DataType &rhs) const { return *this > rhs || *this == rhs; }

InvalidType::InvalidType() : DataType(DataTypeId::INVALID) { }
std::size_t DataType::size() { return TypeInfo::size_of(this->id); }
std::string DataType::str() { return TypeInfo::type_to_string(this->id); }

BasicType::BasicType(DataTypeId type_id) : DataType(type_id) { }

PointerType::PointerType(DataType ptr_to_type) : DataType(DataTypeId::PTR), ptr_to_type(ptr_to_type) { }
bool PointerType::operator==(const PointerType &rhs) const {
    if (this->id == rhs.id && this->ptr_to_type == this->ptr_to_type)  
        return true;
    return false;
}

std::string TypeInfo::type_to_string(DataTypeId type_id) { return type_to_string_table.at(type_id); }
std::size_t TypeInfo::size_of(DataTypeId type_id) { return type_to_size_table.at(type_id);  }

} // end ns

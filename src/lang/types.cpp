#include "../../include/lang/types.hpp"
#include <cmath>
#include <ostream>

namespace compiler::lang::types {

Type::Type(TypeId type_id) : id(type_id) {}

bool Type::operator==(const Type &rhs) const {
    if (this->id != rhs.id) {
        return false;
    } else if (this->id == TypeId::PTR) /* => rhs.id == Type::PTR */ {
        const PointerType &t1 = dynamic_cast<const PointerType &>(*this);
        const PointerType &t2 = dynamic_cast<const PointerType &>(rhs);

        if (t1 == t2)
            return true;
        else
            return false;
    }
    return true;
}

bool Type::operator!=(const Type &rhs) const { return !(*this == rhs); }
bool Type::operator<(const Type &rhs) const { return this->id < rhs.id; }
bool Type::operator<=(const Type &rhs) const {
    return *this < rhs || *this == rhs;
}
bool Type::operator>(const Type &rhs) const { return this->id > rhs.id; }
bool Type::operator>=(const Type &rhs) const {
    return *this > rhs || *this == rhs;
}

InvalidType::InvalidType() : Type(TypeId::INVALID) {}
std::size_t InvalidType::size() const { return m_type_size; }
std::string InvalidType::str() const { return m_str; }

BasicType::BasicType(TypeId type_id) : Type(type_id) {}
std::size_t BasicType::size() const {
    return m_basic_type_to_size_table.at(this->id);
}
std::string BasicType::str() const {
    return m_basic_type_to_string_table.at(this->id);
}

PointerType::PointerType(jbcoe::polymorphic_value<Type> points_to)
    : Type(TypeId::PTR), points_to(std::move(points_to)) {}
bool PointerType::operator==(const PointerType &rhs) const {
    if (this->id == rhs.id &&
        *this->points_to == *rhs.points_to)  // TODO: maybe bug
        return true;
    return false;
}

std::size_t PointerType::size() const { return m_type_size; }
std::string PointerType::str() const { return this->points_to->str() + "*"; }

}  // namespace compiler::lang::types

#ifndef __AST_DATA_TYPES__
#define __AST_DATA_TYPES__

#include <map>
#include <variant>
#include "operators.hpp"
#include "../third_party/polymorphic_value.h"

namespace cpl::lang {

enum class DataTypeId {VOID=0, CHAR=1, INT=2, DOUBLE=3, PTR=4, INVALID=5};

class DataType {
public:
    DataTypeId id;
    virtual std::size_t size() const = 0;
    virtual std::string str() const = 0;

    virtual bool operator==(const DataType &rhs) const;
    bool operator!=(const DataType &rhs) const;
    bool operator<(const DataType &rhs) const;
    bool operator<=(const DataType &rhs) const;
    bool operator>(const DataType &rhs) const;
    bool operator>=(const DataType &rhs) const;

protected:
    DataType(DataTypeId type_id);
 };

class InvalidType : public DataType {
private:
    static constexpr std::size_t m_type_size = 0;
    static constexpr const char * m_str ="Invalid Type";
public:
    InvalidType();

    std::size_t size() const override;
    std::string str() const override;
};

class BasicType : public DataType {
private:
    inline const static std::map <DataTypeId, std::size_t> m_basic_type_to_size_table = {
        { DataTypeId::VOID, 0 },
        { DataTypeId::CHAR, 1 },
        { DataTypeId::INT, 4 },
        { DataTypeId::DOUBLE, 8 },
    };

    inline const static std::map <DataTypeId, std::string> m_basic_type_to_string_table = {
        { DataTypeId::VOID, "void" },
        { DataTypeId::CHAR, "char" },
        { DataTypeId::INT, "int" },
        { DataTypeId::DOUBLE, "double" },
    };

public:
    BasicType(DataTypeId type_id);

    std::size_t size() const override;
    std::string str() const override;
};

class PointerType : public DataType {
private:
    static constexpr std::size_t m_type_size = 8;
public:
    jbcoe::polymorphic_value<DataType> points_to;
    PointerType(jbcoe::polymorphic_value<DataType> points_to);
    bool operator==(const PointerType &rhs) const;

    std::size_t size() const override;
    std::string str() const override;
};
} // end ns

#endif

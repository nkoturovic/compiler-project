#ifndef __AST_DATA_TYPES__
#define __AST_DATA_TYPES__

#include <map>
#include <variant>
#include "operators.hpp"

namespace cpl::lang {

/* Types for DataTypes and Data Reperesentation */
enum class DataTypeId {VOID=0, CHAR=1, INT=2, PTR=3, DOUBLE=4, INVALID=5};

class DataType {
public:
    DataTypeId id;
    std::size_t size();
    std::string str();

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
public:
    InvalidType();
};

class BasicType : public DataType {
public:
    BasicType(DataTypeId type_id);
};

class PointerType : public DataType {
public:
    DataType ptr_to_type = InvalidType();
    PointerType(DataType ptr_to_type);
    bool operator==(const PointerType &rhs) const;
};

class TypeInfo {
private:
    inline const static std::map <DataTypeId, std::size_t> type_to_size_table = {
        { DataTypeId::VOID, 0 },
        { DataTypeId::CHAR, 1 },
        { DataTypeId::INT, 4 },
        { DataTypeId::PTR, 8 },
        { DataTypeId::DOUBLE, 8 },
        { DataTypeId::INVALID, 0 }
    };

    inline const static std::map <DataTypeId, std::string> type_to_string_table = {
        { DataTypeId::VOID, "void" },
        { DataTypeId::CHAR, "char" },
        { DataTypeId::INT, "int" },
        { DataTypeId::PTR, "pointer" },
        { DataTypeId::DOUBLE, "double" },
        { DataTypeId::INVALID, "INVALID" }
    };

public: 
    TypeInfo() = delete;

    // Functions
    static std::string type_to_string(DataTypeId type_id);
    static std::size_t size_of(DataTypeId type_id);
};

} // end ns

#endif

#ifndef __AST_DATA_TYPES__
#define __AST_DATA_TYPES__

#include <map>
#include <variant>
#include "operators.hpp"

namespace cpl::lang {

/* Types for DataTypes and Data Reperesentation */
enum class DataType {VOID=0, CHAR=1, INT=2, PTR=3, DOUBLE=4, INVALID=5};

class TypeInfo {
private:
    inline const static std::map <DataType, std::size_t> type_to_size_table = {
        { DataType::VOID, 0 },
        { DataType::CHAR, 1 },
        { DataType::INT, 4 },
        { DataType::PTR, 8 },
        { DataType::DOUBLE, 8 },
        { DataType::INVALID, 0 }
    };

    inline const static std::map <DataType, std::string> type_to_string_table = {
        { DataType::VOID, "void" },
        { DataType::CHAR, "char" },
        { DataType::INT, "int" },
        { DataType::PTR, "pointer" },
        { DataType::DOUBLE, "double" },
        { DataType::INVALID, "INVALID" }
    };

public: 
    TypeInfo() = delete;

    // Functions
    static std::string type_to_string(DataType type);
    static std::size_t size_of(DataType type);
};

} // end ns

#endif

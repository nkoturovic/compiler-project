#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include <stack>
#include <unordered_map>
#include <optional>

#include "../lang/types.hpp"
#include "../structs.hpp"
#include "../third_party/polymorphic_value.h"
#include "codegen.hpp"

namespace compiler::codegen {

enum class SymbolType {VARIABLE, FUNCTION};

class SymbolException : std::exception {
private:
    std::string m_msg;
    const char* what() const noexcept;
public:
    SymbolException(std::string msg);
};


class SymbolTable {
    private:
    std::unordered_map<std::string, structs::TypeValuePair> m_global_scope;
    std::vector<std::unordered_map<std::string, structs::TypeValuePair>> m_scopes;
    std::unordered_map<std::string, structs::LocProtoFuncTriple> m_declared_functions;
    std::unordered_map<std::string, structs::LocProtoFuncTriple> m_defined_functions;
    
    // scope tracking
    unsigned int m_relevant_index = 0;
    std::vector<unsigned int> m_pushed_scopes_indexes {};

    public:

    std::optional<structs::TypeValuePair> get_variable(std::string id);
    std::optional<structs::TypeValuePair> get_var_curr_scope_only(std::string id);
    std::optional<structs::LocProtoFuncTriple> get_function(std::string id);

    void define_variable(std::string id, structs::TypeValuePair tv);
    void update_variable(std::string id, structs::TypeValuePair tv);
    std::optional<structs::Error> declare_function(std::string id, structs::LocProtoFuncTriple lpf);
    std::optional<structs::Error> define_function(std::string id, structs::LocProtoFuncTriple tv);

    void begin_scope();
    void end_scope();
    void push_scope();
    void pop_scope();
    unsigned num_of_scopes();
    unsigned active_scope_index();
};


} // ns

#endif

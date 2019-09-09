#include "../../include/codegen/symbol.hpp"


namespace compiler::codegen {

const char* SymbolException::what() const noexcept {
    std::string retval = "[SymbolTable]: " + m_msg;
    return retval.c_str();
}

SymbolException::SymbolException(std::string msg) : m_msg(msg) {}

void SymbolTable::begin_scope() {
    m_scopes.push_back(std::unordered_map<std::string, structs::TypeValuePair>());
}

void SymbolTable::end_scope() {
    if (m_scopes.size() == 0) 
        throw SymbolException("No scopes to end");
    m_scopes.resize(m_scopes.size() - 1);
}

void SymbolTable::push_scope() {
    if (m_scopes.size() == 0) 
        throw SymbolException("No scopes to push");

    m_pushed_scopes_indexes.push_back(m_scopes.size() - 1);
    m_relevant_index = m_scopes.size();
}

void SymbolTable::pop_scope() {
    if (m_pushed_scopes_indexes.size() == 0)
        throw SymbolException("No scopes to pop");

    m_relevant_index = m_pushed_scopes_indexes.back();
    m_pushed_scopes_indexes.resize(m_pushed_scopes_indexes.size() - 1);
}

std::optional<structs::TypeValuePair> SymbolTable::get_variable(std::string id) {
    if (m_scopes.size() == 0)
        return {std::nullopt};

    for (int i = m_scopes.size() - 1; i >= (int) m_relevant_index; i--) {
        auto it = m_scopes[i].find(id);
        if (it != m_scopes[i].end())
            return it->second;
    }
    return {std::nullopt};
}

std::optional<structs::FuncProto> SymbolTable::get_function(std::string id) {
}


void SymbolTable::define_variable(std::string id, structs::TypeValuePair tv) {
    if (m_scopes.size() == 0 || this->get_variable(id).has_value()) {
        throw SymbolException("No active scope for defining symbol!");
    } else {
        m_scopes[m_scopes.size()-1].insert(std::pair(id, tv));
    }
}

void SymbolTable::update_variable(std::string id, structs::TypeValuePair tv) {
    if (m_scopes.size() == 0 || !(this->get_variable(id).has_value())) {
            throw SymbolException("No active scope for updating symbol!");
    } else {
        for (int i = m_scopes.size() - 1; i >= (int) m_relevant_index; i--) {
            auto it = m_scopes[i].find(id);
            if (it != m_scopes[i].end()) {
                if (*(it->second.type) != *tv.type) {
                    throw SymbolException("Type mismatch!");
                } else {
                    it->second = tv;
                }
            }
        }
    }
    throw SymbolException("Cant update, Symbol " + id + " does not exists");
}



}

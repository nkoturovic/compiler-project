#ifndef __AST_TYPES__
#define __AST_TYPES__

#include <map>
#include "../third_party/polymorphic_value.h"
#include "operators.hpp"

namespace compiler::lang::types {

enum class TypeId {
    VOID = 0,
    CHAR = 1,
    INT = 2,
    DOUBLE = 3,
    PTR = 4,
    INVALID = 5
};

class Type {
   protected:
    Type(TypeId type_id);

   public:
    TypeId id;
    virtual std::size_t size() const = 0;
    virtual std::string str() const = 0;

    virtual ~Type() = default;
    virtual bool operator==(const Type &rhs) const;
    bool operator!=(const Type &rhs) const;
    bool operator<(const Type &rhs) const;
    bool operator<=(const Type &rhs) const;
    bool operator>(const Type &rhs) const;
    bool operator>=(const Type &rhs) const;
};

class InvalidType : public Type {
   private:
    static constexpr const std::size_t m_type_size = 0;
    static constexpr const char *m_str = "Invalid Type";

   public:
    InvalidType();

    virtual std::size_t size() const override;
    virtual std::string str() const override;
};

class BasicType : public Type {
   private:
    inline const static std::map<TypeId, std::size_t>
        m_basic_type_to_size_table = {
            {TypeId::VOID, 0},
            {TypeId::CHAR, 1},
            {TypeId::INT, 4},
            {TypeId::DOUBLE, 8},
        };

    inline const static std::map<TypeId, std::string>
        m_basic_type_to_string_table = {
            {TypeId::VOID, "void"},
            {TypeId::CHAR, "char"},
            {TypeId::INT, "int"},
            {TypeId::DOUBLE, "double"},
        };

   protected:
    BasicType(TypeId type_id);

    virtual std::size_t size() const override;
    virtual std::string str() const override;
};

class VoidType : public BasicType {
   public:
    VoidType() : BasicType(TypeId::VOID) {}
};

class CharType : public BasicType {
   public:
    CharType() : BasicType(TypeId::CHAR) {}
};

class IntType : public BasicType {
   public:
    IntType() : BasicType(TypeId::INT) {}
};

class DoubleType : public BasicType {
   public:
    DoubleType() : BasicType(TypeId::DOUBLE) {}
};

class PointerType : public Type {
   private:
    static constexpr const std::size_t m_type_size = 8;

   public:
    jbcoe::polymorphic_value<Type> points_to;

    PointerType(jbcoe::polymorphic_value<Type> points_to);

    virtual std::size_t size() const override;
    virtual std::string str() const override;

    bool operator==(const PointerType &rhs) const;
};

class StringType : public PointerType {
   public:
    StringType() : PointerType(jbcoe::polymorphic_value<Type>(CharType())) {}
};



}  // namespace compiler::lang::types

#endif

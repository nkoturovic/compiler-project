#ifndef __AST_H__
#define __AST_H__

#include "../lang/data.hpp"
#include "../lang/data_types.hpp"
#include "../lang/operators.hpp"
#include "../location.hh"
#include "../structs.hpp"
#include "../third_party/polymorphic_value.h"

namespace cpl::ast {
/* Exceptions */
// AST
class AstNode {
   public:
    mutable std::optional<Error> opt_error{std::nullopt};
    yy::location loc;

    AstNode(yy::location loc);
    virtual ~AstNode() = default;
};

class Statement : public AstNode {
   public:
    Statement(yy::location loc);
    virtual ~Statement() = default;

    virtual void interpret() const = 0;
};

class Expression : public Statement {
   public:
    Expression(yy::location loc);
    virtual ~Expression() = default;

    virtual jbcoe::polymorphic_value<lang::DataType> check_type() const = 0;
    virtual lang::Data evaluate() const = 0;
    void interpret() const;
};

class Literal : public Expression {
   private:
    lang::Data m_data;

   public:
    Literal(yy::location loc, lang::Data data);
    jbcoe::polymorphic_value<lang::DataType> check_type() const override;
    lang::Data evaluate() const override;
};

class BinOp : public Expression {
   private:
    lang::BinOpId m_op_id;
    jbcoe::polymorphic_value<Expression> m_lhs, m_rhs;

   public:
    BinOp(yy::location loc, lang::BinOpId op_id,
          jbcoe::polymorphic_value<Expression> lhs,
          jbcoe::polymorphic_value<Expression> rhs);
    jbcoe::polymorphic_value<lang::DataType> check_type() const override;
    lang::Data evaluate() const override;
};

class UnOp : public Expression {
   private:
    lang::UnOpId m_op_id;
    jbcoe::polymorphic_value<Expression> m_expr;

   public:
    UnOp(yy::location loc, lang::UnOpId op_id,
         jbcoe::polymorphic_value<Expression> expr);
    jbcoe::polymorphic_value<lang::DataType> check_type() const override;
    lang::Data evaluate() const override;
};

}  // namespace cpl::ast
#endif

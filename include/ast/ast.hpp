#ifndef __AST_H__
#define __AST_H__

#include <map>
#include <variant>
#include <sstream>
#include <memory>
#include <queue>
#include "../location.hh"
#include "../structs.hpp"
#include "../lang/data_types.hpp"
#include "../lang/data.hpp"
#include "../lang/operators.hpp"

namespace cpl::ast {
/* Exceptions */
// AST
class AstNode {
public:
    mutable std::optional<Error> opt_error { std::nullopt };
    yy::location loc;
    AstNode(yy::location loc);
};

class Statement : public AstNode {
public:
    Statement(yy::location loc);
    virtual void interpret() const = 0;
};

class Expression : public Statement {
public:
    Expression(yy::location loc);

    virtual lang::DataType check_type() const = 0;
    virtual lang::Data evaluate() const = 0;
    void interpret() const;
};

class Literal : public Expression {
private:
    lang::Data m_data;
public:
    Literal(yy::location loc, lang::Data data);
    lang::DataType check_type() const override;
    lang::Data evaluate() const override;
};

class BinOp : public Expression {
private:
    lang::BinOpId m_op_id;
    std::unique_ptr<Expression> m_lhs, m_rhs;
public:
    BinOp(yy::location loc, lang::BinOpId op_id, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);
    lang::DataType check_type() const override;
    lang::Data evaluate() const override;
};

} // end ns
#endif

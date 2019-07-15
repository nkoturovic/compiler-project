#ifndef __AST_H__
#define __AST_H__

#include <map>
#include <variant>
#include <sstream>
#include <memory>
#include "data_types.hpp"
#include "../location.hh"


namespace ast {
/* Exceptions */
class AstException : public std::exception {
    yy::location expr_location;
public:
    AstException(yy::location);
    const char* what() const throw();
    virtual const char* err_msg() const = 0;
};

class InvalidExpressionException : public AstException {
    InvalidExpressionException(yy::location);
    const char* err_msg() const override;
};

// AST
class AstNode {
public:
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

    virtual DataType check_type() const = 0;
    virtual Data evaluate() const = 0;
    void interpret() const;
};

class Literal : public Expression {
private:
    Data m_data;

public:
    Literal(yy::location loc, Data data);
    DataType check_type() const override;
    Data evaluate() const override;
};

class BinOp : public Expression {
protected:
    std::unique_ptr<Expression> m_lhs, m_rhs;
    BinOp(yy::location loc, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);
public:
    DataType check_type() const override;
};

class Plus : public BinOp {
public:
   Plus(yy::location loc, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);
   Data evaluate() const override;
};

}

#endif

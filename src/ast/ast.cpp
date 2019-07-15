#include "../../include/ast/ast.hpp"

namespace ast {

AstException::AstException(yy::location loc) : expr_location(loc) { }

const char* AstException::what() const throw() { 
    std::stringstream ss;
    ss << this->expr_location << this->err_msg();
    return  ss.str().c_str();
}

InvalidExpressionException::InvalidExpressionException(yy::location loc) : AstException(loc) { }

AstNode::AstNode(yy::location loc) : loc(loc) {}

Statement::Statement(yy::location loc) : AstNode(loc) { }

Expression::Expression(yy::location loc) : Statement(loc) {}

void Expression::interpret() const {
    std::cout << this->evaluate();
}

const char* InvalidExpressionException::err_msg() const {
    return "Invalid expression";
}

Literal::Literal(yy::location loc, Data data) : Expression(loc), m_data(data) {}

DataType Literal::check_type() const {
    return m_data.type;
}

Data Literal::evaluate() const {
    return m_data;
}

BinOp::BinOp(yy::location loc, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs) 
    : Expression(loc), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

DataType BinOp::check_type() const {
    return std::max(m_lhs->check_type(), m_rhs->check_type());
}

Plus::Plus(yy::location loc, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs) : BinOp(loc, std::move(lhs), std::move(rhs)) {}
Data Plus::evaluate() const {
    return Data::plus(this->check_type(), m_lhs->evaluate(), m_rhs->evaluate());
}

}

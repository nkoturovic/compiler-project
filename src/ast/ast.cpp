#include "../../include/ast/ast.hpp"
#include "../../include/lang/semantic.hpp"
#include "../../include/driver.hpp"

using namespace cpl::lang;

namespace cpl::ast {

AstNode::AstNode(yy::location loc) : loc(loc) {}

Statement::Statement(yy::location loc) : AstNode(loc) { }

Expression::Expression(yy::location loc) : Statement(loc) {}

void Expression::interpret() const {
    Data eval = this->evaluate();
    if (!this->opt_error.has_value()) {
        std::cout << eval << std::endl;
    } else {
        const Error &err = opt_error.value();
        const Driver &driver = Driver::get_active_instance();
        driver.error(err.loc, err.msg);
    }
}

Literal::Literal(yy::location loc, Data data) : Expression(loc), m_data(data) {}

DataType Literal::check_type() const {
    if (this->m_data.type == DataType::INVALID)
        this->opt_error.emplace(Error(this->loc, std::string("Invalid type of literal")));
    return m_data.type;
}

Data Literal::evaluate() const {
    return m_data;
}

BinOp::BinOp(yy::location loc, BinOpId op_id, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs) 
    : Expression(loc), m_op_id(op_id), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

DataType BinOp::check_type() const { 
    DataType lhs_type = m_lhs->check_type(), rhs_type = m_rhs->check_type();

    if (lhs_type == DataType::INVALID) {
        if(m_lhs->opt_error.has_value())
            this->opt_error.emplace(m_lhs->opt_error.value());
        return DataType::INVALID;
    } else if (rhs_type == DataType::INVALID) {
        if (m_rhs->opt_error.has_value())
            this->opt_error.emplace(m_rhs->opt_error.value());
        return DataType::INVALID;
    }

    DataType res = Semantic::binop_check(m_op_id, lhs_type, rhs_type);

    if (res == DataType::INVALID) {
        std::stringstream err_msg_ss;
        err_msg_ss << "Invalid operands to BinOp " << BinOpInfo::binop_id_to_string(m_op_id)
            << " (" << TypeInfo::type_to_string(lhs_type) << ", " << TypeInfo::type_to_string(rhs_type) << ")";
        this->opt_error.emplace(Error(loc, err_msg_ss.str()));
    }

    return res;
}

Data BinOp::evaluate() const {
    return Data::do_bin_op(m_op_id, m_lhs->evaluate(), m_rhs->evaluate(), this->check_type());
}

UnOp::UnOp(yy::location loc, UnOpId op_id, std::unique_ptr<Expression> expr) 
    : Expression(loc), m_op_id(op_id), m_expr(std::move(expr)) {}

DataType UnOp::check_type() const { 
    DataType expr_type = m_expr->check_type();

    if (expr_type == DataType::INVALID) {
        if(m_expr->opt_error.has_value())
            this->opt_error.emplace(m_expr->opt_error.value());
        return DataType::INVALID;
    }

    DataType res = Semantic::unop_check(m_op_id, expr_type);

    if (res == DataType::INVALID) {
        std::stringstream err_msg_ss;
        err_msg_ss << "Invalid operands to UnOp " << UnOpInfo::unop_id_to_string(m_op_id)
            << " (" << TypeInfo::type_to_string(expr_type) << ")";
        this->opt_error.emplace(Error(loc, err_msg_ss.str()));
    }

    return res;
}

Data UnOp::evaluate() const {
    return Data::do_un_op(m_op_id, m_expr->evaluate(), this->check_type());
}


} // end ns

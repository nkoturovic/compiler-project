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
    if (this->errors.empty()) {
        std::cout << eval << std::endl;
    } else {
        while (!errors.empty()) {
            Error &err = errors.top();
            Driver::get_active_instance().error(err.loc, err.msg);
            errors.pop();
        }
    }
}

Literal::Literal(yy::location loc, Data data) : Expression(loc), m_data(data) {}

DataType Literal::check_type() const {
    return m_data.type;
}

Data Literal::evaluate() const {
    return m_data;
}

BinOp::BinOp(yy::location loc, BinOpId op_id, std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs) 
    : Expression(loc), m_op_id(op_id), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

DataType BinOp::check_type() const { 
    DataType lhs_type = m_lhs->check_type(), rhs_type = m_rhs->check_type();
    DataType res = Semantic::binop_check(m_op_id, lhs_type, rhs_type);

    if (res == DataType::INVALID) {
        std::stringstream err_msg_ss;
        err_msg_ss << "Invalid operands to BinOp " << BinOpInfo::binop_id_to_string(m_op_id)
            << ": (" << TypeInfo::type_to_string(lhs_type) << ", " << TypeInfo::type_to_string(rhs_type) << ")";
        errors.push(Error(loc, err_msg_ss.str()));
    }
    return res;
}

Data BinOp::evaluate() const {
    return Data::do_bin_op(m_op_id, m_lhs->evaluate(), m_rhs->evaluate(), this->check_type());
}

} // end ns

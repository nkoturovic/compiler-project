#include <sstream>
#include <string>

#include "../../include/ast/ast.hpp"
#include "../../include/driver.hpp"
#include "../../include/lang/semantic.hpp"

using namespace compiler::lang;
using namespace jbcoe;

namespace compiler::ast {

AstNode::AstNode(yy::location loc) : loc(std::move(loc)) {}
std::ostream& operator<<(std::ostream &out, const AstNode& node) {
    return out << node.str();
}

Statement::Statement(yy::location loc) : AstNode(std::move(loc)) {}

Expression::Expression(yy::location loc) : Statement(std::move(loc)) {}

polymorphic_value<types::Type> Literal::check_type() const {
    if (this->m_type->id == types::TypeId::INVALID)
        this->opt_error.emplace(
            structs::Error{this->loc, std::string("Invalid type of literal")});
    return this->m_type;
}

Literal::Literal(yy::location loc,
                 jbcoe::polymorphic_value<lang::types::Type> type)
    : Expression(std::move(loc)), m_type(type) {}

CharLiteral::CharLiteral(yy::location loc, char c)
    : Literal(std::move(loc), poly_type(types::CharType())), m_val(c) {}

llvm::Value* CharLiteral::CharLiteral::codegen() const {
    return nullptr; /* TODO */ 
}

std::string CharLiteral::CharLiteral::str() const {
    std::stringstream ss; 
    ss << "CharLiteral(" << this->m_val << ")";
    return ss.str();
}

IntLiteral::IntLiteral(yy::location loc, int i)
    : Literal(std::move(loc), poly_type(types::IntType())), m_val(i) {}

llvm::Value* IntLiteral::IntLiteral::codegen() const {
    return nullptr; /* TODO */
}

std::string IntLiteral::IntLiteral::str() const {
    std::stringstream ss; 
    ss << "IntLiteral(" << this->m_val << ")";
    return ss.str();
}

DoubleLiteral::DoubleLiteral(yy::location loc, double d)
    : Literal(std::move(loc), poly_type(types::IntType())), m_val(d) {}

llvm::Value* DoubleLiteral::DoubleLiteral::codegen() const {
    return nullptr; /* TODO */
}

std::string DoubleLiteral::DoubleLiteral::str() const {
    std::stringstream ss; 
    ss << "DoubleLiteral(" << this->m_val << ")";
    return ss.str();
}


BinOp::BinOp(yy::location loc, operators::BinOpId op_id,
             polymorphic_value<Expression> lhs,
             polymorphic_value<Expression> rhs)
    : Expression(std::move(loc)),
      m_op_id(op_id),
      m_lhs(std::move(lhs)),
      m_rhs(std::move(rhs)) {}

polymorphic_value<types::Type> BinOp::check_type() const {
    const polymorphic_value<types::Type>&lhs_type = m_lhs->check_type(),
          &rhs_type = m_rhs->check_type();

    if (lhs_type->id == types::TypeId::INVALID) {
        if (m_lhs->opt_error.has_value())
            this->opt_error.emplace(m_lhs->opt_error.value());
        return polymorphic_value<types::Type>(types::InvalidType());
    } else if (rhs_type->id == types::TypeId::INVALID) {
        if (m_rhs->opt_error.has_value())
            this->opt_error.emplace(m_rhs->opt_error.value());
        return polymorphic_value<types::Type>(types::InvalidType());
    }

    polymorphic_value<types::Type> res =
        semantic::binop_check(m_op_id, lhs_type, rhs_type);

    if (res->id == types::TypeId::INVALID) {
        std::stringstream err_msg_ss;
        err_msg_ss << "Invalid operands to BinOp "
                   << operators::BinOpInfo::binop_id_to_string(m_op_id) << " ("
                   << lhs_type->str() << ", " << rhs_type->str() << ")";
        this->opt_error.emplace(structs::Error{loc, err_msg_ss.str()});
    }

    return res;
}

llvm::Value* BinOp::codegen() const {
    /* TODO */
     m_lhs->codegen();

    std::cout << "BinOp:" + lang::operators::BinOpInfo::binop_id_to_string(
                                this->m_op_id);
    std::cout << std::endl;
    m_rhs->codegen();
    std::cout << std::endl;

    return {};  // Data::do_bin_op(m_op_id, m_lhs->evaluate(),
                // m_rhs->evaluate(), this->check_type());
}

std::string BinOp::str() const {
    /* TODO */
    std::stringstream ss;
    ss << "BinOp" 
       << lang::operators::BinOpInfo::binop_id_to_string(this->m_op_id)
       << "(" << m_lhs->str() << "," << m_rhs->str() << ")";
    return ss.str();
}

UnOp::UnOp(yy::location loc, operators::UnOpId op_id,
           polymorphic_value<Expression> expr)
    : Expression(std::move(loc)), m_op_id(op_id), m_expr(std::move(expr)) {}

polymorphic_value<types::Type> UnOp::check_type() const {
    const polymorphic_value<types::Type>& expr_type = m_expr->check_type();

    if (expr_type->id == types::TypeId::INVALID) {
        if (m_expr->opt_error.has_value())
            this->opt_error.emplace(m_expr->opt_error.value());
        return polymorphic_value<types::Type>(types::InvalidType());
    }

    polymorphic_value<types::Type> res =
        semantic::unop_check(m_op_id, expr_type);

    if (res->id == types::TypeId::INVALID) {
        std::stringstream err_msg_ss;
        err_msg_ss << "Invalid operands to UnOp "
                   << operators::UnOpInfo::unop_id_to_string(m_op_id) << " ("
                   << expr_type->str() << ")";
        this->opt_error.emplace(structs::Error{loc, err_msg_ss.str()});
    }

    return res;
}

llvm::Value* UnOp::codegen() const {
    /* TODO */
    std::cout << "UnOp: " +
                     lang::operators::UnOpInfo::unop_id_to_string(this->m_op_id)
              << std::endl;
    this->m_expr->codegen();
    return {};  // Data::do_un_op(m_op_id, m_expr->evaluate(),
                // this->check_type());
}

std::string UnOp::str() const {
    std::stringstream ss;
    ss << "UnOp" 
       << lang::operators::UnOpInfo::unop_id_to_string(this->m_op_id)
       << '(' << this->m_expr->str() << ')';
    return ss.str();
}


Block::Block(yy::location loc,
             std::vector<jbcoe::polymorphic_value<Statement>> statements)
    : Statement(std::move(loc)), m_statements(std::move(statements)) {}

llvm::Value* Block::codegen() const {
    /* TODO */
    std::cout << "Block" << std::endl;

    for (auto& s : m_statements) s->codegen();

    return nullptr;
}

std::string Block::str() const {
    std::stringstream ss;
    ss << "Block(";
    for (auto& stmt : m_statements) 
        ss << stmt->str();
    ss << ')';

    return ss.str();
}

OuterDecl::OuterDecl(yy::location loc) : AstNode(std::move(loc)) {} 


FuncDecl::FuncDecl(std::string name, std::vector<structs::FuncArg> args,
                   jbcoe::polymorphic_value<lang::types::Type> retval_t)
    : OuterDecl(std::move(loc)),
      m_name(std::move(name)),
      m_args(std::move(args)),
      m_retval_t(std::move(retval_t)) {}

llvm::Value* FuncDecl::codegen() const {
    /* TODO */
    std::cout << "FuncDeclaration: ";
    std::cout << m_retval_t->str() << " " << m_name << std::endl;
    for (auto& [t, n] : m_args) std::cout << t->str() << " " << n << std::endl;
    std::cout << ")" << std::endl;

    return nullptr;
}

std::string FuncDecl::str() const {
    /* TODO */
    std::stringstream ss;
    ss << "FuncDeclaration("
       << "FName("<< m_name << "),Args(";

    for (auto& [t, n] : m_args) 
        ss << t->str() << " " << n << ',';
    ss << "),"
       << "ReturnType(" << m_retval_t->str() << "))";
    return ss.str();
}

FuncDef::FuncDef(FuncDecl prototype, Block body)
    : OuterDecl(std::move(loc)),
      m_prototype(std::move(prototype)),
      m_body(std::move(body)) {}

llvm::Value* FuncDef::codegen() const {
    /* TODO */
    std::cout << "Function def: ";
    m_prototype.codegen();
    m_body.codegen();

    return nullptr;
}

std::string FuncDef::str() const {
    std::stringstream ss;
    ss << "FunctionDef("
       << m_prototype.str() << ','
       << m_body.str()
       << ')';
    return ss.str();
}


}  // namespace compiler::ast

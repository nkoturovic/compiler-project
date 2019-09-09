#include <sstream>
#include <string>

#include "../../include/ast/ast.hpp"
#include "../../include/driver.hpp"
#include "../../include/lang/semantic.hpp"
#include "../../include/codegen/codegen.hpp"

using namespace compiler::lang;
using namespace jbcoe;

namespace compiler::ast {

AstNode::AstNode(yy::location loc) : loc(std::move(loc)) {
    AstNode::m_node_instances.push_back(this);
}

std::vector<structs::Error> AstNode::collect_errors() {
    std::vector <structs::Error> errors;
    for (const AstNode * n : m_node_instances) {
        if (n->opt_error.has_value())
            errors.push_back(n->opt_error.value());
    }
    return errors;
}

bool AstNode::has_errors() {
    for (const AstNode * n : m_node_instances) {
        if (!n->opt_error.has_value()) 
            return true;
    }
    return false;
}

AstNode::~AstNode() { 
    auto it = std::find(m_node_instances.begin(), m_node_instances.end(), this);
    if (it != m_node_instances.end())
        AstNode::m_node_instances.erase(it);
}

std::ostream& operator<<(std::ostream &out, const AstNode& node) {
    return out << node.str();
}

Statement::Statement(yy::location loc) : AstNode(std::move(loc)) {}

Expression::Expression(yy::location loc) : Statement(std::move(loc)) {}

llvm::Value* Expression::codegen() const {
    structs::TypeValuePair tv = this->evaluate();

    if (tv.type->id == lang::types::TypeId::INVALID || tv.value == nullptr) {
        return nullptr;
    }

    return tv.value;
}

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

structs::TypeValuePair CharLiteral::evaluate() const {
    polymorphic_value<lang::types::Type> type = polymorphic_value<lang::types::Type>(lang::types::CharType());
    llvm::Value * val = llvm::ConstantInt::get(codegen::llvm_type(type), m_val);
    return {type, val};
}

std::string CharLiteral::CharLiteral::str() const {
    std::stringstream ss; 
    ss << "CharLiteral(" << this->m_val << ")";
    return ss.str();
}

IntLiteral::IntLiteral(yy::location loc, int i)
    : Literal(std::move(loc), poly_type(types::IntType())), m_val(i) {}

structs::TypeValuePair IntLiteral::evaluate() const {
    polymorphic_value<lang::types::Type> type = polymorphic_value<lang::types::Type>(lang::types::IntType());
    llvm::Value * val = llvm::ConstantInt::get(codegen::llvm_type(type), m_val);
    return {type, val};
}

std::string IntLiteral::IntLiteral::str() const {
    std::stringstream ss; 
    ss << "IntLiteral(" << this->m_val << ")";
    return ss.str();
}

DoubleLiteral::DoubleLiteral(yy::location loc, double d)
    : Literal(std::move(loc), poly_type(types::DoubleType())), m_val(d) {}

structs::TypeValuePair DoubleLiteral::evaluate() const {
    polymorphic_value<lang::types::Type> type = polymorphic_value<lang::types::Type>(lang::types::DoubleType());
    llvm::Value * val = llvm::ConstantFP::get(codegen::llvm_type(type), m_val);
    return {type, val};
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

structs::TypeValuePair BinOp::evaluate() const {

     structs::TypeValuePair lhs_tv = m_lhs->evaluate();
     structs::TypeValuePair rhs_tv = m_rhs->evaluate();

    auto invalid_type = lang::types::InvalidType();

     if (lhs_tv.type->id == invalid_type.id || 
         rhs_tv.type->id == invalid_type.id ||
         lhs_tv.value == nullptr ||
         rhs_tv.value == nullptr) {
         this->opt_error.emplace(structs::Error{this->loc, "Unable to evaluate expression"});
         return {polymorphic_value<lang::types::Type>(invalid_type), nullptr};
     }

     polymorphic_value<lang::types::Type> result_type = this->check_type();

     if (result_type->id == invalid_type.id)
         return {polymorphic_value<lang::types::Type>(invalid_type), nullptr};

     // TODO EXPAND
     llvm::Value * lhs_value = lhs_tv.value;
     llvm::Value * rhs_value = rhs_tv.value;
     llvm::Value * result_value = nullptr;
     if (result_type->id == lang::types::TypeId::INT) {
        if (lhs_tv.type->id == lang::types::TypeId::DOUBLE) 
             lhs_value = codegen::global::builder.CreateFPToSI(lhs_tv.value, codegen::llvm_type(result_type), "fptositmp");
        if (rhs_tv.type->id == lang::types::TypeId::DOUBLE) 
             rhs_value = codegen::global::builder.CreateFPToSI(rhs_tv.value, codegen::llvm_type(result_type), "fptositmp");

         switch(m_op_id) {
             case lang::operators::BinOpId::PLUS :
                 result_value = codegen::global::builder.CreateAdd(lhs_value, rhs_value, "iaddtmp");
                 break;
             case lang::operators::BinOpId::MINUS :
                 result_value = codegen::global::builder.CreateSub(lhs_value, rhs_value, "isubtmp");
                 break;
            case lang::operators::BinOpId::MUL :
                 result_value = codegen::global::builder.CreateMul(lhs_value, rhs_value, "imultmp");
                 break;
            case lang::operators::BinOpId::DIV :
                 result_value = codegen::global::builder.CreateUDiv(lhs_value, rhs_value, "idivtmp");
                 break;
            default:
                 result_value = nullptr;
         }
     } else if (result_type->id == lang::types::TypeId::DOUBLE) {

         if (lhs_tv.type->id == lang::types::TypeId::INT) 
              lhs_value = codegen::global::builder.CreateSIToFP(lhs_tv.value, codegen::llvm_type(result_type), "sitofptmp");
         if (rhs_tv.type->id == lang::types::TypeId::INT) 
              rhs_value = codegen::global::builder.CreateSIToFP(rhs_tv.value, codegen::llvm_type(result_type), "sitofptmp");

         switch(m_op_id) {
             case lang::operators::BinOpId::PLUS :
                 result_value = codegen::global::builder.CreateFAdd(lhs_value, rhs_value, "faddtmp");
                 break;
             case lang::operators::BinOpId::MINUS :
                 result_value = codegen::global::builder.CreateFSub(lhs_value, rhs_value, "fsubmp");
                 break;
            case lang::operators::BinOpId::MUL :
                 result_value = codegen::global::builder.CreateFMul(lhs_value, rhs_value, "fmultmp");
                 break;
            case lang::operators::BinOpId::DIV :
                 result_value = codegen::global::builder.CreateFDiv(lhs_value, rhs_value, "fdivtmp");
                 break;
           default:
                 result_value = nullptr;
         }
     }
    return {result_type, result_value};
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

structs::TypeValuePair UnOp::evaluate() const {
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
    for (auto& s : m_statements) 
        s->codegen();
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


FuncDecl::FuncDecl(yy::location loc, structs::FuncProto prototype) :
    OuterDecl(std::move(loc)),
    m_prototype(prototype) {}

llvm::Function* FuncDecl::codegen() const {
    // define retType @fname(args) { entrypoint: }
    
    std::vector<llvm::Type*> arg_types;
    std::transform(m_prototype.args.begin(), m_prototype.args.end(), std::back_inserter(arg_types), 
            [](const structs::FuncArg &arg){
                return codegen::llvm_type(arg.type);
            });

    llvm::FunctionType *funcType =
        llvm::FunctionType::get(codegen::llvm_type(m_prototype.retval_t), arg_types, false);

    llvm::Function *func =
      llvm::Function::Create(funcType, llvm::Function::ExternalLinkage, m_prototype.name, *codegen::global::module);

  { 
      unsigned i = 0;
      for (auto it = func->arg_begin(); it != func->arg_end(); it++) {
          if (!m_prototype.args[i].name.empty())
            it->setName(m_prototype.args[i].name);
        i++;
      }
  }
    return func;
}

std::string FuncDecl::str() const {
    /* TODO */
    std::stringstream ss;
    ss << "FuncDeclaration("
       << "FName("<< m_prototype.name << "),Args(";

    for (auto& [t, n] : m_prototype.args) 
        ss << t->str() << " " << n << ',';
    ss << "),"
       << "ReturnType(" << m_prototype.retval_t->str() << "))";
    return ss.str();
}

FuncDef::FuncDef(yy::location loc, FuncDecl prototype, Block body)
    : OuterDecl(std::move(loc)),
      m_prototype(std::move(prototype)),
      m_body(std::move(body)) {}

llvm::Function* FuncDef::codegen() const {
    sm_current_codegen_instance = const_cast<FuncDef*>(this);
    llvm::Function * func = m_prototype.codegen();
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(codegen::global::context, "entrypoint", func);
    codegen::global::builder.SetInsertPoint(entry);

    m_body.codegen();

  return func;
}

structs::FuncProto FuncDef::current_codegen_proto() {
    if (sm_current_codegen_instance == nullptr)
        return structs::FuncProto{};
    else
        return sm_current_codegen_instance->m_prototype.m_prototype;
}

std::string FuncDef::str() const {
    std::stringstream ss;
    ss << "FunctionDef("
       << m_prototype.str() << ','
       << m_body.str()
       << ')';
    return ss.str();
}

ReturnStmt::ReturnStmt(yy::location loc, jbcoe::polymorphic_value<Expression> expr)
    : Statement(std::move(loc)), m_expr(std::move(expr)) { }

llvm::Value* ReturnStmt::codegen() const {
    polymorphic_value<lang::types::Type> return_type = FuncDef::current_codegen_proto().retval_t;
    structs::TypeValuePair type_value_pair = m_expr->evaluate();
    llvm::Value * retval = type_value_pair.value;
    if (*return_type != *type_value_pair.type) {
        if (return_type->id == lang::types::TypeId::DOUBLE && type_value_pair.type->id <= lang::types::TypeId::INT) {
            retval = codegen::global::builder.CreateSIToFP(retval, codegen::llvm_type(return_type), "retvalsitofp");
        } else if (return_type->id <= lang::types::TypeId::INT && type_value_pair.type->id == lang::types::TypeId::DOUBLE) {
            retval = codegen::global::builder.CreateFPToSI(retval, codegen::llvm_type(return_type), "retvalfptosi");
        } else {
            retval = nullptr;
        }
    }
    return codegen::global::builder.CreateRet(retval);
}

std::string ReturnStmt::str() const {
    std::stringstream ss;
    ss << "ReturnStmt("
       << ')';
    return ss.str();
}


}  // namespace compiler::ast

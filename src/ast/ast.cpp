#include <sstream>
#include <string>

#include "../../include/ast/ast.hpp"
#include "../../include/driver.hpp"
#include "../../include/lang/semantic.hpp"
#include "../../include/codegen/codegen.hpp"

using namespace compiler::lang;
using namespace jbcoe;

namespace compiler::ast {

AstNode::AstNode(yy::location loc) : loc(std::move(loc)) { }

AstNode::~AstNode() { }

std::vector<structs::Error> AstNode::get_errors() {
    return AstNode::errors;
}

std::vector<structs::Error> AstNode::get_warnings() {
    return AstNode::warnings;
}


std::ostream& operator<<(std::ostream &out, const AstNode& node) {
    return out << node.str();
}

Statement::Statement(yy::location loc) : AstNode(std::move(loc)) {}

IfElse::IfElse(yy::location loc, jbcoe::polymorphic_value<Expression> condition, jbcoe::polymorphic_value<Statement> if_branch, std::optional<jbcoe::polymorphic_value<Statement>> opt_else_branch) : Statement(std::move(loc)), m_condition(std::move(condition)), m_if_branch(std::move(if_branch)), m_opt_else_branch(std::move(opt_else_branch)) { }


 llvm::Value* IfElse::codegen() const { 
     structs::TypeValuePair tv_cond_eval = m_condition->evaluate();
     poly_type poly_int_t = poly_type(lang::types::IntType());
     structs::TypeCodegenFuncPair type_fcdg = semantic::convert(tv_cond_eval, poly_int_t);

     if (type_fcdg.type->id != lang::types::TypeId::INT) {
         errors.push_back({m_condition->loc, "Invalid condition"});
         return nullptr;
     }

     llvm::Value * cmp_val = type_fcdg.codegen_func();

     llvm::Constant * zero_val = llvm::ConstantInt::get(codegen::llvm_type(type_fcdg.type), 0, true);
     llvm::Value * cond_val = codegen::global::builder.CreateICmpNE(cmp_val, zero_val, "condition");

     llvm::Function *curr_func = codegen::global::builder.GetInsertBlock()->getParent();

     llvm::BasicBlock * if_branch_BB = llvm::BasicBlock::Create(codegen::global::context, "if_branch", curr_func);
     llvm::BasicBlock * else_branch_BB = llvm::BasicBlock::Create(codegen::global::context, "else_branch");
     llvm::BasicBlock * merge_BB = llvm::BasicBlock::Create(codegen::global::context, "merge");
     codegen::global::builder.CreateCondBr(cond_val, if_branch_BB, else_branch_BB);
     codegen::global::builder.SetInsertPoint(if_branch_BB);
     m_if_branch->codegen();

     codegen::global::builder.CreateBr(merge_BB);
     if_branch_BB = codegen::global::builder.GetInsertBlock();

     curr_func->getBasicBlockList().push_back(else_branch_BB);
     codegen::global::builder.SetInsertPoint(else_branch_BB);

     llvm::Value * else_br_val = nullptr;
     if (m_opt_else_branch.has_value())
        else_br_val = m_opt_else_branch.value()->codegen();

     codegen::global::builder.CreateBr(merge_BB);
     else_branch_BB = codegen::global::builder.GetInsertBlock();

     curr_func->getBasicBlockList().push_back(merge_BB);
     codegen::global::builder.SetInsertPoint(merge_BB);


     return cond_val;
 }

 std::string IfElse::str() const {
    std::stringstream ss;
    ss << "IfElse(" << "Condition(" << m_condition->str() << ')'
       << "ifBranch(" << m_if_branch->str() << ")";

    if (m_opt_else_branch.has_value())
        ss << "ElseBranch(" << m_opt_else_branch.value()->str() << ")";

    ss << ')';
    return ss.str();
}


While::While(yy::location loc, jbcoe::polymorphic_value<Expression> condition, jbcoe::polymorphic_value<Statement> body) : Statement(std::move(loc)), m_condition(std::move(condition)), m_body(std::move(body)) {}


 llvm::Value* While::codegen() const { 
     poly_type poly_int_t = poly_type(lang::types::IntType());
     llvm::Constant * zero_val = llvm::ConstantInt::get(codegen::llvm_type(poly_int_t), 0, true);
     llvm::Function *curr_func = codegen::global::builder.GetInsertBlock()->getParent();

     llvm::BasicBlock * cond_BB = llvm::BasicBlock::Create(codegen::global::context, "loop_cond", curr_func);
     llvm::BasicBlock * body_BB = llvm::BasicBlock::Create(codegen::global::context, "loop_body");
     llvm::BasicBlock * exit_BB = llvm::BasicBlock::Create(codegen::global::context, "loop_exit");

     codegen::global::builder.CreateBr(cond_BB);
     //curr_func->getBasicBlockList().push_back(cond_BB);
     codegen::global::builder.SetInsertPoint(cond_BB); 

     structs::TypeValuePair tv_cond_eval = m_condition->evaluate();
     structs::TypeCodegenFuncPair type_fcdg = semantic::convert(tv_cond_eval, poly_int_t);

     if (type_fcdg.type->id != lang::types::TypeId::INT) {
         errors.push_back({m_condition->loc, "Invalid condition"});
         return nullptr;
     }

     llvm::Value * cmp_val = type_fcdg.codegen_func();
     llvm::Value * cond_val = codegen::global::builder.CreateICmpNE(cmp_val, zero_val, "condition");

     codegen::global::builder.CreateCondBr(cond_val, body_BB, exit_BB);
     cond_BB = codegen::global::builder.GetInsertBlock();

     curr_func->getBasicBlockList().push_back(body_BB);
     codegen::global::builder.SetInsertPoint(body_BB); 
     m_body->codegen();
     codegen::global::builder.CreateBr(cond_BB);
     body_BB = codegen::global::builder.GetInsertBlock();

     curr_func->getBasicBlockList().push_back(exit_BB);
     codegen::global::builder.SetInsertPoint(exit_BB); 

     return cond_val;
 }

 std::string While::str() const {
    std::stringstream ss;
    ss << "While(" << "Condition(" << m_condition->str() << ')'
       << "Body(" << m_body->str() << ")"
       << ')';

    return ss.str();
 }




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
        AstNode::errors.push_back(
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
    llvm::Value * val = llvm::ConstantInt::get(codegen::llvm_type(type), m_val, true);
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
    llvm::Value * val = llvm::ConstantInt::get(codegen::llvm_type(type), m_val, true);
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

StringLiteral::StringLiteral(yy::location loc, std::string str)
    : Literal(std::move(loc), poly_type(types::StringType())), m_val(std::move(str)) {}

structs::TypeValuePair StringLiteral::evaluate() const {
    polymorphic_value<lang::types::Type> type = polymorphic_value<lang::types::Type>(lang::types::StringType());
    llvm::Constant * val = llvm::ConstantDataArray::getString(codegen::global::module->getContext(), m_val, true);
    return {type, val};
}

std::string StringLiteral::StringLiteral::str() const {
    std::stringstream ss; 
    ss << "StringLiteral(" << this->m_val << ")";
    return ss.str();
}


Variable::Variable(yy::location loc, std::string id) : Expression(std::move(loc)), m_id(std::move(id)) { }

jbcoe::polymorphic_value<lang::types::Type> Variable::check_type() const {
    std::optional<structs::TypeValuePair> opt_var = symbols.get_variable(m_id);
    lang::types::InvalidType invalid_t;
    if (!opt_var.has_value()) {
        return poly_type(invalid_t);
    } else if (opt_var->type->id == invalid_t.id || opt_var->value == nullptr) {
        return poly_type(invalid_t);
    }
    return opt_var->type;
}

structs::TypeValuePair Variable::evaluate() const { 
    auto type = this->check_type();
    if (type->id == lang::types::TypeId::INVALID) {
        AstNode::errors.push_back(structs::Error{this->loc, "Undefined variable " + m_id});
        return {type, nullptr}; 
    }

    std::optional<structs::TypeValuePair> opt_tv = symbols.get_variable(m_id);
    llvm::Value * val = codegen::global::builder.CreateLoad(codegen::llvm_type(opt_tv->type), opt_tv.value().value, "tmp"); 
    return {opt_tv->type, val};
}


std::string Variable::str() const {
    return "Variable(" + m_id + ")";
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

    if (lhs_type->id == types::TypeId::INVALID || 
            rhs_type->id == types::TypeId::INVALID) {
        //AstNode::errors.push_back(structs::Error{this->loc, "Unable to evaluate expression " + this->str()});
        return polymorphic_value<types::Type>(types::InvalidType());
    }

    polymorphic_value<types::Type> res =
        semantic::binop_check(m_op_id, lhs_type, rhs_type);

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
         //AstNode::errors.push_back(structs::Error{this->loc, "Unable to evaluate expression"});
         return {polymorphic_value<lang::types::Type>(invalid_type), nullptr};
     }

     polymorphic_value<lang::types::Type> result_type = this->check_type();

    if (result_type->id == invalid_type.id) {
        std::stringstream err_msg_ss;
        err_msg_ss << "Invalid operands to BinOp"
                   << operators::BinOpInfo::binop_id_to_string(m_op_id) << "("
                   << lhs_tv.type->str() << "," << rhs_tv.type->str() << ")";
        AstNode::errors.push_back(structs::Error{this->loc, err_msg_ss.str()});
        return {polymorphic_value<lang::types::Type>(invalid_type), nullptr};
    }

     // TODO EXPAND
     llvm::Value * lhs_value = lhs_tv.value;
     llvm::Value * rhs_value = rhs_tv.value;
     llvm::Value * result_value = nullptr;
     if (result_type->id <= lang::types::TypeId::INT) {
        if (lhs_tv.type->id == lang::types::TypeId::DOUBLE) 
             lhs_value = codegen::global::builder.CreateFPToSI(lhs_tv.value, codegen::llvm_type(result_type), "fptosi");
        if (rhs_tv.type->id <= lang::types::TypeId::DOUBLE) 
             rhs_value = codegen::global::builder.CreateFPToSI(rhs_tv.value, codegen::llvm_type(result_type), "fptosi");

         switch(m_op_id) {
             case lang::operators::BinOpId::PLUS :
                 result_value = codegen::global::builder.CreateAdd(lhs_value, rhs_value, "iadd");
                 break;
             case lang::operators::BinOpId::MINUS :
                 result_value = codegen::global::builder.CreateSub(lhs_value, rhs_value, "isub");
                 break;
            case lang::operators::BinOpId::MUL :
                 result_value = codegen::global::builder.CreateMul(lhs_value, rhs_value, "imul");
                 break;
            case lang::operators::BinOpId::DIV :
                 result_value = codegen::global::builder.CreateSDiv(lhs_value, rhs_value, "idiv");
                 break;
            case lang::operators::BinOpId::EQ :
                 result_value = codegen::global::builder.CreateICmpEQ(lhs_value, rhs_value, "ieq");
                 result_value = codegen::global::builder.CreateIntCast(result_value, codegen::llvm_type(result_type), 1, "i1toi32");
                 break;
            case lang::operators::BinOpId::NEQ :
                 result_value = codegen::global::builder.CreateICmpNE(lhs_value, rhs_value, "ineq");
                 result_value = codegen::global::builder.CreateIntCast(result_value, codegen::llvm_type(result_type), 1, "i1toi32");
                 break;
            case lang::operators::BinOpId::LT :
                 result_value = codegen::global::builder.CreateICmpSLT(lhs_value, rhs_value, "ilt");
                 result_value = codegen::global::builder.CreateIntCast(result_value, codegen::llvm_type(result_type), 1, "i1toi32");
                 break;
            case lang::operators::BinOpId::GT :
                 result_value = codegen::global::builder.CreateICmpSGT(lhs_value, rhs_value, "igt");
                 result_value = codegen::global::builder.CreateIntCast(result_value, codegen::llvm_type(result_type), 1, "i1toi32");
                 break;
            case lang::operators::BinOpId::LEQ :
                 result_value = codegen::global::builder.CreateICmpSLE(lhs_value, rhs_value, "ileq");
                 result_value = codegen::global::builder.CreateIntCast(result_value, codegen::llvm_type(result_type), 1, "i1toi32");
                 break;
            case lang::operators::BinOpId::GEQ :
                 result_value = codegen::global::builder.CreateICmpSGE(lhs_value, rhs_value, "igeq");
                 result_value = codegen::global::builder.CreateIntCast(result_value, codegen::llvm_type(result_type), 1, "i1toi32");
                 break;
           
            default:
                 result_value = nullptr;
         }
     } else if (result_type->id == lang::types::TypeId::DOUBLE) {

         if (lhs_tv.type->id <= lang::types::TypeId::INT) 
              lhs_value = codegen::global::builder.CreateSIToFP(lhs_tv.value, codegen::llvm_type(result_type), "sitofp");
         if (rhs_tv.type->id <= lang::types::TypeId::INT) 
              rhs_value = codegen::global::builder.CreateSIToFP(rhs_tv.value, codegen::llvm_type(result_type), "sitofp");

         switch(m_op_id) {
             case lang::operators::BinOpId::PLUS :
                 result_value = codegen::global::builder.CreateFAdd(lhs_value, rhs_value, "fadd");
                 break;
             case lang::operators::BinOpId::MINUS :
                 result_value = codegen::global::builder.CreateFSub(lhs_value, rhs_value, "fsubmp");
                 break;
            case lang::operators::BinOpId::MUL :
                 result_value = codegen::global::builder.CreateFMul(lhs_value, rhs_value, "fmul");
                 break;
            case lang::operators::BinOpId::DIV :
                 result_value = codegen::global::builder.CreateFDiv(lhs_value, rhs_value, "fdiv");
                 break;
            case lang::operators::BinOpId::EQ :
                 result_value = codegen::global::builder.CreateFCmpUEQ(lhs_value, rhs_value, "feq");
                 result_value = codegen::global::builder.CreateIntCast(result_value, codegen::llvm_type(result_type), 1, "i1toi32");
                 break;
            case lang::operators::BinOpId::NEQ :
                 result_value = codegen::global::builder.CreateFCmpUNE(lhs_value, rhs_value, "fneq");
                 result_value = codegen::global::builder.CreateIntCast(result_value, codegen::llvm_type(result_type), 1, "i1toi32");
                 break;
            case lang::operators::BinOpId::LT :
                 result_value = codegen::global::builder.CreateFCmpULT(lhs_value, rhs_value, "flt");
                 result_value = codegen::global::builder.CreateIntCast(result_value, codegen::llvm_type(result_type), 1, "i1toi32");
                 break;
            case lang::operators::BinOpId::GT :
                 result_value = codegen::global::builder.CreateFCmpUGT(lhs_value, rhs_value, "fgt");
                 result_value = codegen::global::builder.CreateIntCast(result_value, codegen::llvm_type(result_type), 1, "i1toi32");
                 break;
            case lang::operators::BinOpId::LEQ :
                 result_value = codegen::global::builder.CreateFCmpULE(lhs_value, rhs_value, "fleq");
                 result_value = codegen::global::builder.CreateIntCast(result_value, codegen::llvm_type(result_type), 1, "i1toi32");
                 break;
            case lang::operators::BinOpId::GEQ :
                 result_value = codegen::global::builder.CreateFCmpUGE(lhs_value, rhs_value, "fgeq");
                 result_value = codegen::global::builder.CreateIntCast(result_value, codegen::llvm_type(result_type), 1, "i1toi32");
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
        AstNode::errors.push_back(structs::Error{this->loc, "Unable to apply UnOp" + operators::UnOpInfo::unop_id_to_string(m_op_id) });
        return polymorphic_value<types::Type>(types::InvalidType());
    }

    polymorphic_value<types::Type> res =
        semantic::unop_check(m_op_id, expr_type);

    if (res->id == types::TypeId::INVALID) {
        std::stringstream err_msg_ss;
        err_msg_ss << "Invalid operands to UnOp "
                   << operators::UnOpInfo::unop_id_to_string(m_op_id) << " ("
                   << expr_type->str() << ")";
        AstNode::errors.push_back(structs::Error{loc, err_msg_ss.str()});
    }

    return res;
}

structs::TypeValuePair UnOp::evaluate() const {
    /* TODO */
    return { poly_type(lang::types::InvalidType()), nullptr };
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
    AstNode::symbols.begin_scope();
    llvm::Value * retval = nullptr;
    for (auto& s : m_statements) 
        retval = s->codegen();
    AstNode::symbols.end_scope();
    return retval;
}

std::string Block::str() const {
    std::stringstream ss;
    ss << "Block(";
    for (const auto& stmt : m_statements) 
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
    symbols.declare_function(m_prototype.name, {this->loc, m_prototype, func});
    return func;
}

std::string FuncDecl::str() const {
    std::stringstream ss;
    ss << "FuncDeclaration("
       << "FName("<< m_prototype.name << ")Args(";

    for (auto& [t, n] : m_prototype.args) 
        ss << t->str() << " " << n; // << ',';
    ss << ")"
       << "ReturnType(" << m_prototype.retval_t->str() << "))";
    return ss.str();
}

const structs::FuncProto& FuncDecl::prototype() const {
    return m_prototype;
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
    symbols.define_function(m_prototype.m_prototype.name, {this->loc, m_prototype.m_prototype, func});

    if (symbols.num_of_scopes() > 0)
        symbols.push_scope();

    symbols.begin_scope();
        std::vector <llvm::AllocaInst*> arg_values;
        for (llvm::Argument &arg : func->args()) {
            llvm::AllocaInst *ai = codegen::global::builder.CreateAlloca(arg.getType(), 0, nullptr,arg.getName());
            arg_values.push_back(ai);
            codegen::global::builder.CreateStore(&arg, ai);
        }

        for (unsigned i = 0; i < m_prototype.m_prototype.args.size(); i++) {
            symbols.define_variable(m_prototype.m_prototype.args[i].name, { m_prototype.m_prototype.args[i].type, arg_values[i]});
        }

        m_body.codegen();
    symbols.end_scope();

    if (symbols.num_of_scopes() > 0)
        symbols.pop_scope();

  return func;
}

structs::FuncProto FuncDef::current_codegen_proto() {
    if (sm_current_codegen_instance == nullptr)
        return structs::FuncProto{};
    else
        return sm_current_codegen_instance->m_prototype.m_prototype;
}


const structs::FuncProto& FuncDef::prototype() const {
    return m_prototype.m_prototype;
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

    if (type_value_pair.type->id == lang::types::TypeId::INVALID ||
        type_value_pair.value == nullptr) 
        return nullptr;

    structs::TypeCodegenFuncPair converted = semantic::convert(std::move(type_value_pair), return_type);

    if (converted.type->id == lang::types::TypeId::INVALID) {
        AstNode::errors.push_back(
                structs::Error{this->loc, "returning " + type_value_pair.type->str() + ", but return type is " + return_type->str()});
        return nullptr;
    }
    return codegen::global::builder.CreateRet(converted.codegen_func());
}

std::string ReturnStmt::str() const {
    std::stringstream ss;
    ss << "ReturnStmt("
       << ')';
    return ss.str();
}

FuncCall::FuncCall(yy::location loc, std::string fname, std::vector<polymorphic_value<Expression>> arguments) 
                        : Expression(std::move(loc)), m_fname(std::move(fname)) { m_args = std::move(arguments); }

polymorphic_value<lang::types::Type> FuncCall::check_type() const {
    std::optional<structs::LocProtoFuncTriple> lpf = AstNode::symbols.get_function(m_fname);
    lang::types::InvalidType invalid_type;
    if (!lpf.has_value()) {
        AstNode::errors.push_back({this->loc, "Function " + m_fname + " is not defined"});
        return poly_type(invalid_type);
    }

    if (lpf->proto.args.size() != m_args.size()) {
        std::stringstream ss;
        ss << "Function " << m_fname << "expects "
           << lpf->proto.args.size() << " args but you've called it with " << m_args.size(); 
        AstNode::errors.push_back({this->loc, ss.str()});
        return poly_type(invalid_type);
    }

    for (unsigned i=0; i < m_args.size(); i++) {
        auto exp_t = m_args[i]->check_type();

        if (exp_t->id == lang::types::TypeId::INVALID)
            return poly_type(invalid_type);

        structs::TypeCodegenFuncPair type_cdg = semantic::convert({exp_t, nullptr}, lpf->proto.args[i].type);
        if (type_cdg.type->id == lang::types::TypeId::INVALID) {
            return poly_type(invalid_type);
        }
    }
    return lpf->proto.retval_t;
}

structs::TypeValuePair FuncCall::evaluate() const {

    lang::types::InvalidType invalid_type;
    if (this->check_type()->id == lang::types::TypeId::INVALID) 
        return {poly_type(invalid_type), nullptr};

    std::optional<structs::LocProtoFuncTriple> lpf = AstNode::symbols.get_function(m_fname);
    std::vector<llvm::Value *> converted_args;
    std::vector<llvm::Type *> converted_types;


    for (unsigned i=0; i < m_args.size(); i++) {
        structs::TypeValuePair exp_eval = m_args[i]->evaluate();

        if (exp_eval.type->id == lang::types::TypeId::INVALID || exp_eval.value == nullptr)
            return { poly_type(invalid_type), nullptr };

        structs::TypeCodegenFuncPair type_cdg = semantic::convert({exp_eval.type, exp_eval.value}, lpf->proto.args[i].type);
        converted_args.push_back(type_cdg.codegen_func());
        converted_types.push_back(codegen::llvm_type(type_cdg.type));
    }

    llvm::FunctionType *ftype = llvm::FunctionType::get(codegen::llvm_type(lpf->proto.retval_t), converted_types, false);
    return { lpf->proto.retval_t, codegen::global::builder.CreateCall(ftype, lpf->func, converted_args, "call_" + lpf->proto.name) };
}

std::string FuncCall::str() const {
    std::stringstream ss;
    ss << "FuncCall("
       << "name(" << m_fname << ')'
       << "args(";

    for (const polymorphic_value<Expression> &p_arg :  m_args)
        ss << p_arg->str();

    ss << "))";
    return ss.str();
}

VariableDecl::VariableDecl(yy::location loc, poly_type type, std::vector<structs::StrOptExprPair> var_decl_list)
                : Statement(std::move(loc)), m_type(std::move(type)), m_var_decl_list(move(var_decl_list)) {}

llvm::Value* VariableDecl::codegen() const {
    llvm::Value * ai = nullptr;
    for (const auto& [n, opt_e] : m_var_decl_list) {
        std::optional<structs::TypeValuePair> opt_tv = symbols.get_variable(n);
        if (opt_tv.has_value()) {
            errors.push_back({this->loc, "Variable " + n + " already defined"});
            return nullptr;
        } else {
            ai = codegen::global::builder.CreateAlloca(codegen::llvm_type(m_type), 0, n);
            if (opt_e.has_value()) {
                structs::TypeCodegenFuncPair type_cdg = semantic::convert(opt_e.value()->evaluate(), m_type);
                codegen::global::builder.CreateStore(type_cdg.codegen_func(), ai, true);
                if (type_cdg.type->id == lang::types::TypeId::INVALID) {
                    errors.push_back({this->loc, "Invalid type of expression "});
                    return nullptr;
                }
            }
            symbols.define_variable(n, {m_type, ai});
            return ai;
        }
    }
    return ai;
}

std::string VariableDecl::str() const {
    std::stringstream ss;
    ss << "VariableDecl(" << "Type(" << m_type->str() << "),";

    for (const auto& [n, opt_e] : m_var_decl_list) {
        ss << "name(" << n << ')'; // << ',';
        if (opt_e.has_value())
            ss << opt_e.value()->str();
    }
    ss << ')';

    return ss.str();
}

VariableAssign::VariableAssign(yy::location loc, std::string id, jbcoe::polymorphic_value<Expression> expr) 
                        : Expression(std::move(loc)), m_id(std::move(id)), m_expr(std::move(expr)) {}


poly_type VariableAssign::check_type() const {
    std::optional<structs::TypeValuePair> opt_tv = symbols.get_variable(m_id);
    auto invalid_type = poly_type(lang::types::InvalidType());
    if (!opt_tv.has_value()) {
        errors.push_back({loc, "Undefined variable " + m_id});
        return invalid_type;
    }

    if (opt_tv.value().type->id == invalid_type->id ||
                                    opt_tv->value == nullptr) {
         errors.push_back({loc, "Invalid variable " + m_id});
         return invalid_type;
    }
    return opt_tv->type;
}

structs::TypeValuePair VariableAssign::evaluate() const {
    auto invalid_type = poly_type(lang::types::InvalidType());
    poly_type tc = this->check_type();

    if (tc->id == lang::types::TypeId::INVALID)
        return {invalid_type, nullptr};

    std::optional<structs::TypeValuePair> opt_tv = symbols.get_variable(m_id);
    structs::TypeValuePair expr_eval = m_expr->evaluate();

    structs::TypeCodegenFuncPair t_cdg = semantic::convert(expr_eval, tc);
    if (t_cdg.type->id != tc->id) {
         errors.push_back({loc, "Can't assign value of type " + tc->str() + " to variable of type " + opt_tv->type->str()});
         return {invalid_type, nullptr};
    }

    codegen::global::builder.CreateStore(t_cdg.codegen_func(), opt_tv->value, true);

    return {tc, opt_tv->value};
}

std::string VariableAssign::str() const {
    return "VariableAssign(" + std::string("name(") + m_id + ")" + "value(" + m_expr->str() + "))";
}

}  // namespace compiler::ast

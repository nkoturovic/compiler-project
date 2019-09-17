#ifndef __AST_H__
#define __AST_H__

#include <optional>

#include "../../include/codegen/codegen.hpp"
#include "../../include/codegen/symbol.hpp"
#include "../lang/operators.hpp"
#include "../lang/types.hpp"
#include "../location.hh"
#include "../structs.hpp"
#include "../third_party/polymorphic_value.h"

namespace compiler::ast {
// AST
class AstNode {
   protected:
    AstNode(yy::location loc);
    inline static codegen::SymbolTable symbols = codegen::SymbolTable();
    inline static std::vector<structs::Error> errors{};
    inline static std::vector<structs::Error> warnings{};

   public:
    yy::location loc;
    virtual llvm::Value* codegen() const = 0;
    virtual std::string str() const = 0;
    virtual ~AstNode();

    static std::vector<structs::Error> get_errors();
    static std::vector<structs::Error> get_warnings();

    friend std::ostream& operator<<(std::ostream& out, const AstNode& node);
};
std::ostream& operator<<(std::ostream& out, const AstNode& node);

class Statement : public AstNode {
   protected:
    bool p_is_terminator = false;

   public:
    Statement(yy::location loc);
    virtual llvm::Value* codegen() const = 0;
    bool is_terminator() const;
};

class IfElse : public Statement {
   private:
    jbcoe::polymorphic_value<Expression> m_condition;
    jbcoe::polymorphic_value<Statement> m_if_branch;
    std::optional<jbcoe::polymorphic_value<Statement>> m_opt_else_branch;

   public:
    IfElse(yy::location loc, jbcoe::polymorphic_value<Expression> condition,
           jbcoe::polymorphic_value<Statement> if_branch,
           std::optional<jbcoe::polymorphic_value<Statement>> opt_else_branch);
    virtual llvm::Value* codegen() const override;
    virtual std::string str() const override;
};

class While : public Statement {
   private:
    jbcoe::polymorphic_value<Expression> m_condition;
    jbcoe::polymorphic_value<Statement> m_body;

   public:
    While(yy::location loc, jbcoe::polymorphic_value<Expression> condition,
          jbcoe::polymorphic_value<Statement> body);
    virtual llvm::Value* codegen() const override;
    virtual std::string str() const override;

   private:
    While(yy::location loc);

    friend class For;
};

class For : public Statement {
   private:
    jbcoe::polymorphic_value<Statement> m_init_block;
    While m_while;

   public:
    For(yy::location loc, jbcoe::polymorphic_value<Statement> init_block,
        jbcoe::polymorphic_value<Expression> condition,
        jbcoe::polymorphic_value<Statement> after_expr,
        jbcoe::polymorphic_value<Statement> body);

    virtual llvm::Value* codegen() const override;
    virtual std::string str() const override;
};

class Expression : public Statement {
   public:
    Expression(yy::location loc);

    virtual jbcoe::polymorphic_value<lang::types::Type> check_type() const = 0;
    virtual llvm::Value* codegen() const override;
    virtual structs::TypeValuePair evaluate() const = 0;
};

class Literal : public Expression {
   private:
    jbcoe::polymorphic_value<lang::types::Type> m_type;

   public:
    virtual jbcoe::polymorphic_value<lang::types::Type> check_type()
        const override;

   protected:
    Literal(yy::location loc, jbcoe::polymorphic_value<lang::types::Type>);
};

class CharLiteral : public Literal {
   private:
    char m_val;

   public:
    CharLiteral(yy::location loc, char c);
    virtual structs::TypeValuePair evaluate() const override;
    virtual std::string str() const override;
};

class IntLiteral : public Literal {
   private:
    int m_val;

   public:
    IntLiteral(yy::location loc, int i);
    virtual structs::TypeValuePair evaluate() const override;
    virtual std::string str() const override;
};

class DoubleLiteral : public Literal {
   private:
    double m_val;

   public:
    DoubleLiteral(yy::location loc, double d);
    virtual structs::TypeValuePair evaluate() const override;
    virtual std::string str() const override;
};

class StringLiteral : public Literal {
   private:
    std::string m_val;

   public:
    StringLiteral(yy::location loc, std::string str);
    virtual structs::TypeValuePair evaluate() const override;
    virtual std::string str() const override;
};

class Variable : public Expression {
   private:
    std::string m_id;

   public:
    Variable(yy::location loc, std::string id);

    virtual jbcoe::polymorphic_value<lang::types::Type> check_type()
        const override;
    virtual structs::TypeValuePair evaluate() const override;
    virtual std::string str() const override;
};

class VariableDecl : public Statement {
   private:
    jbcoe::polymorphic_value<lang::types::Type> m_type;
    std::vector<structs::StrOptExprPair> m_var_decl_list;

   public:
    VariableDecl(yy::location loc,
                 jbcoe::polymorphic_value<lang::types::Type> type,
                 std::vector<structs::StrOptExprPair> var_decl_list);

    virtual llvm::Value* codegen() const override;
    virtual std::string str() const override;
};

class VariableAssign : public Expression {
   private:
    std::string m_id;
    jbcoe::polymorphic_value<Expression> m_expr;

   public:
    VariableAssign(yy::location loc, std::string id,
                   jbcoe::polymorphic_value<Expression> expr);

    virtual jbcoe::polymorphic_value<lang::types::Type> check_type()
        const override;
    virtual structs::TypeValuePair evaluate() const override;
    virtual std::string str() const override;
};

class BinOp : public Expression {
   private:
    lang::operators::BinOpId m_op_id;
    jbcoe::polymorphic_value<Expression> m_lhs, m_rhs;

   public:
    BinOp(yy::location loc, lang::operators::BinOpId op_id,
          jbcoe::polymorphic_value<Expression> lhs,
          jbcoe::polymorphic_value<Expression> rhs);
    virtual jbcoe::polymorphic_value<lang::types::Type> check_type()
        const override;
    virtual structs::TypeValuePair evaluate() const override;
    virtual std::string str() const override;
};

class UnOp : public Expression {
   private:
    lang::operators::UnOpId m_op_id;
    jbcoe::polymorphic_value<Expression> m_expr;

   public:
    UnOp(yy::location loc, lang::operators::UnOpId op_id,
         jbcoe::polymorphic_value<Expression> expr);
    virtual jbcoe::polymorphic_value<lang::types::Type> check_type()
        const override;
    virtual structs::TypeValuePair evaluate() const override;
    virtual std::string str() const override;
};

class Block : public Statement {
   public:
    std::vector<jbcoe::polymorphic_value<Statement>> m_statements;

    Block(yy::location loc,
          std::vector<jbcoe::polymorphic_value<Statement>> statements);
    virtual llvm::Value* codegen() const override;
    virtual std::string str() const override;
    std::optional<jbcoe::polymorphic_value<Statement>> get_last_statement()
        const;
};

class Empty : public Expression {
   public:
    Empty(yy::location loc);
    virtual jbcoe::polymorphic_value<lang::types::Type> check_type()
        const override;
    virtual structs::TypeValuePair evaluate() const override;
    virtual std::string str() const override;
};

class OuterDecl : public AstNode {
   public:
    OuterDecl(yy::location loc);
    virtual llvm::Value* codegen() const = 0;
    virtual std::string str() const = 0;
};

class FuncDecl : public OuterDecl {
   private:
    compiler::structs::FuncProto m_prototype;

   public:
    FuncDecl(yy::location loc, structs::FuncProto prototype);
    virtual llvm::Function* codegen() const override;
    virtual std::string str() const override;

    friend class FuncDef;
    const structs::FuncProto& prototype() const;
};

class Terminator : public Statement {
   public:
    Terminator(yy::location loc);
};

class ReturnStmt : public Terminator {
   public:
    ReturnStmt(yy::location loc, jbcoe::polymorphic_value<Expression> expr);
    virtual llvm::Value* codegen() const override;
    virtual std::string str() const override;

   private:
    jbcoe::polymorphic_value<Expression> m_expr;
};

class FuncDef : public OuterDecl {
   private:
    static inline FuncDef* sm_current_codegen_instance = nullptr;
    FuncDecl m_prototype;
    Block m_body;

   public:
    static structs::FuncProto current_codegen_proto();

    FuncDef(yy::location loc, FuncDecl prototype, Block body);
    virtual llvm::Function* codegen() const override;
    virtual std::string str() const override;
    const structs::FuncProto& prototype() const;
};

class FuncCall : public Expression {
   private:
    std::string m_fname;
    std::vector<jbcoe::polymorphic_value<Expression>> m_args;

   public:
    FuncCall(yy::location loc, std::string fname,
             std::vector<jbcoe::polymorphic_value<Expression>> arguments);

    virtual jbcoe::polymorphic_value<lang::types::Type> check_type()
        const override;
    virtual structs::TypeValuePair evaluate() const override;
    virtual std::string str() const override;
};

}  // namespace compiler::ast

#endif

#ifndef __AST_H__
#define __AST_H__

#include <optional>

#include "../lang/operators.hpp"
#include "../lang/types.hpp"
#include "../location.hh"
#include "../structs.hpp"
#include "../third_party/polymorphic_value.h"
#include "../../include/codegen/codegen.hpp"
#include "../../include/codegen/symbol.hpp"

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

    friend std::ostream& operator<<(std::ostream &out, const AstNode& node);
};
std::ostream& operator<<(std::ostream &out, const AstNode& node);


class Statement : public AstNode {
   public:
    Statement(yy::location loc);
    virtual llvm::Value* codegen() const = 0;
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

    virtual jbcoe::polymorphic_value<lang::types::Type> check_type() const override;
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
};


class ReturnStmt : public Statement {
public:
    ReturnStmt(yy::location loc, jbcoe::polymorphic_value<Expression> expr);
    virtual llvm::Value* codegen() const override;
    virtual std::string str() const override;
private:
    jbcoe::polymorphic_value<Expression> m_expr;
};


class FuncDef : public OuterDecl {
   private:
    static inline FuncDef * sm_current_codegen_instance = nullptr;
    FuncDecl m_prototype;
    Block m_body;

   public:
    static structs::FuncProto current_codegen_proto();

    FuncDef(yy::location loc, FuncDecl prototype, Block body);
    virtual llvm::Function* codegen() const override;
    virtual std::string str() const override;
};

class FuncCall : public Expression {
    private:
        std::string m_fname;
        std::vector<jbcoe::polymorphic_value<Expression>> m_args;

    public:
        FuncCall(yy::location loc, std::string fname, std::vector<jbcoe::polymorphic_value<Expression>> arguments);

        virtual jbcoe::polymorphic_value<lang::types::Type> check_type() const override;
        virtual structs::TypeValuePair evaluate() const override;
        virtual std::string str() const override;
};

}  // namespace compiler::ast

#endif

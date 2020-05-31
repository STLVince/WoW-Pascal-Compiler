#ifndef _AST_STATEMENT_HPP
#define _AST_STATEMENT_HPP

#include "node.hpp"
#include "identifier.hpp"
#include "declation.hpp"
#include "expression.hpp"
#include "../codegen/CodeGenContext.h"

namespace ast
{
    class CaseStmt;
    class Identifier;
    using CaseList = std::vector<std::shared_ptr<CaseStmt>>;

    class AssignmentStmt : public Statement
    {
    public:
        std::shared_ptr<Identifier> lhs;
        std::shared_ptr<Expression> rhs;

        AssignmentStmt(std::shared_ptr<Identifier> lhs, std::shared_ptr<Expression> rhs) : lhs(lhs), rhs(rhs) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class IfStmt : public Statement
    {
    public:
        std::shared_ptr<Expression> condition;
        std::shared_ptr<StatementList> then_stmt;
        std::shared_ptr<StatementList> else_stmt;

        IfStmt(std::shared_ptr<Expression> condition, std::shared_ptr<StatementList> then_stmt) : condition(condition), then_stmt(then_stmt), else_stmt(nullptr){};
        IfStmt(std::shared_ptr<Expression> condition, std::shared_ptr<StatementList> then_stmt, std::shared_ptr<StatementList> else_stmt) : condition(condition), then_stmt(then_stmt), else_stmt(else_stmt){};
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class WhileStmt : public Statement
    {
    public:
        std::shared_ptr<Expression> condition;
        std::shared_ptr<StatementList> loop_stmt;

        WhileStmt(std::shared_ptr<Expression> condition, std::shared_ptr<StatementList> loop_stmt) : condition(condition), loop_stmt(loop_stmt) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class RepeatStmt : public Statement
    {
    public:
        std::shared_ptr<Expression> condition;
        std::shared_ptr<StatementList> loop_stmt;

        RepeatStmt(std::shared_ptr<Expression> condition, std::shared_ptr<StatementList> loop_stmt) : condition(condition), loop_stmt(loop_stmt) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class ForStmt : public Statement
    {
    public:
        int direct; // 1 to; -1 downto
        std::shared_ptr<Identifier> loop_var;
        std::shared_ptr<Expression> start_val;
        std::shared_ptr<Expression> end_val;
        std::shared_ptr<StatementList> loop_stmt;

        ForStmt(int direct, std::shared_ptr<Identifier> loop_var, std::shared_ptr<Expression> start_val, std::shared_ptr<Expression> end_val, std::shared_ptr<StatementList> loop_stmt) : direct(direct), loop_var(loop_var), start_val(start_val), end_val(end_val), loop_stmt(loop_stmt) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class CaseStmt : public Statement
    {
    public:
        std::shared_ptr<Expression> condition;
        std::shared_ptr<StatementList> then_stmt;
        llvm::BasicBlock *bblock, *bexit;
        CaseStmt(std::shared_ptr<Expression> condition, std::shared_ptr<StatementList> then_stmt) : condition(condition), then_stmt(then_stmt) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };
    
    class SwitchStmt : public Statement
    {
    public:
        std::shared_ptr<Expression> expression;
        std::shared_ptr<CaseList> list;
        SwitchStmt(std::shared_ptr<Expression> expression, std::shared_ptr<CaseList> list) : expression(expression), list(list) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class LabelStmt : public Statement
    {
    public:
        int label;
        std::shared_ptr<Statement> statement;
        LabelStmt(int label, std::shared_ptr<Statement> statement) : label(label), statement(statement) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class GotoStmt : public Statement
    {
    public:
        int label;
        GotoStmt(int label) : label(label) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };
} // namespace ast

#endif
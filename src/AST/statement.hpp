#ifndef _AST_STATEMENT_HPP
#define _AST_STATEMENT_HPP

#include "node.hpp"
#include "identifier.hpp"
#include "declation.hpp"
#include "expression.hpp"

namespace ast
{
    class AssignmentStmt : public Statement
    {
    public:
        std::shared_ptr<Identifier> lhs; 
        std::shared_ptr<Expression> rhs;

        AssignmentStmt(std::shared_ptr<Identifier> lhs, std::shared_ptr<Expression> rhs) : lhs(lhs), rhs(rhs) {}
        /*virtual std::vector<std::shared_ptr<Node>> getChildren()
        {
            std::vector<std::shared_ptr<Node>> list;
            list.push_back((std::shared_ptr<Node>)lhs);
            list.push_back((std::shared_ptr<Node>)rhs);
            return list;
        }*/
        virtual llvm::Value *code_gen(CodeGenContext &context);
    };

    class IfStmt : public Statement
    {
    //private:
        //int instance_count;
    public:
        std::shared_ptr<Expression> condition;
        std::shared_ptr<Statement> then_stmt;
        std::shared_ptr<Statement> else_stmt;

        IfStmt(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> then_stmt, std::shared_ptr<Statement> else_stmt) : condition(condition), then_stmt(then_stmt), else_stmt(else_stmt){};
        virtual llvm::Value *code_gen(CodeGenContext &context);
    };

    class WhileStmt : public Statement
    {
    public:
        std::shared_ptr<Expression> condition;
        std::shared_ptr<Statement> loop_stmt;

        WhileStmt(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> loop_stmt) : condition(condition), loop_stmt(loop_stmt) {}
        virtual llvm::Value *code_gen(CodeGenContext &context);
    };

    class RepeatStmt : public Statement
    {
    public:
        std::shared_ptr<Expression> condition;
        std::shared_ptr<Statement> loop_stmt;

        RepeatStmt(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> loop_stmt) : condition(condition), loop_stmt(loop_stmt) {}
        virtual llvm::Value *code_gen(CodeGenContext &context);
    };

    class ForStmt : public Statement
    {
    public:
        int direct; // 1 to; -1 downto
        std::shared_ptr<Identifier> loop_var;
        std::shared_ptr<Expression> start_val;
        std::shared_ptr<Expression> end_val;
        std::shared_ptr<Statement> loop_stmt;

        ForStmt(int direct, std::shared_ptr<Identifier> loop_var, std::shared_ptr<Expression> start_val, std::shared_ptr<Expression> end_val,std::shared_ptr<Statement> loop_stmt) : direct(direct), loop_var(loop_var), start_val(start_val),end_val(end_val),loop_stmt(loop_stmt) {}
        virtual llvm::Value *code_gen(CodeGenContext &context);
    };
} // namespace ast

#endif
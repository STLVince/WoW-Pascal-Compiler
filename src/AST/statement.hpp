#ifndef _AST_STATEMENT_HPP
#define _AST_STATEMENT_HPP

#include "node.hpp"
#include "identifier.hpp"
#include "declation.hpp"
#include "expression.hpp"
#include <fstream>

extern std::ofstream astDot;
namespace ast
{
    class CaseStmt;
    using CaseList = std::vector<std::shared_ptr<CaseStmt>>;

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
        void printSelf(std::string nodeName)
        {
            astDot << nodeName << "->" << nodeName + "_" + lhs->name << std::endl;
            std::string childName = nodeName + "_rhs";
            astDot << nodeName << "->" << childName << std::endl;
            rhs->printSelf(childName);
        }
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
        void printSelf(std::string nodeName)
        {
            std::string condName = nodeName + "_condition";
            astDot << nodeName << "->" << condName << std::endl;
            condition->printSelf(condName);
            std::string thenName = nodeName + "_then";
            astDot << nodeName << "->" << thenName << std::endl;
            then_stmt->printSelf(thenName);
            std::string elseName = nodeName + "_then";
            astDot << nodeName << "->" << elseName << std::endl;
            else_stmt->printSelf(elseName);
        }
        virtual llvm::Value *code_gen(CodeGenContext &context);
    };

    class WhileStmt : public Statement
    {
    public:
        std::shared_ptr<Expression> condition;
        std::shared_ptr<Statement> loop_stmt;

        WhileStmt(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> loop_stmt) : condition(condition), loop_stmt(loop_stmt) {}
        void printSelf(std::string nodeName)
        {
            std::string condName = nodeName + "_condition";
            astDot << nodeName << "->" << condName << std::endl;
            condition->printSelf(condName);
            std::string loopName = nodeName + "_loop";
            astDot << nodeName << "->" << loopName << std::endl;
            loop_stmt->printSelf(loopName);
        }
        virtual llvm::Value *code_gen(CodeGenContext &context);
    };

    class RepeatStmt : public Statement
    {
    public:
        std::shared_ptr<Expression> condition;
        std::shared_ptr<Statement> loop_stmt;

        RepeatStmt(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> loop_stmt) : condition(condition), loop_stmt(loop_stmt) {}
        void printSelf(std::string nodeName)
        {
            std::string condName = nodeName + "_condition";
            astDot << nodeName << "->" << condName << std::endl;
            condition->printSelf(condName);
            std::string loopName = nodeName + "_loop";
            astDot << nodeName << "->" << loopName << std::endl;
            loop_stmt->printSelf(loopName);
        }
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
        void printSelf(std::string nodeName)
        {
            std::string loop_varName = nodeName + "_loop_var";
            astDot << nodeName << "->" << loop_varName << std::endl;
            loop_var->printSelf(loop_varName);
            std::string start_valName = nodeName + "_start_val";
            astDot << nodeName << "->" << start_valName << std::endl;
            start_val->printSelf(start_valName);
            std::string start_valName = nodeName + "_start_val";
            astDot << nodeName << "->" << start_valName << std::endl;
            start_val->printSelf(start_valName);
            std::string end_valName = nodeName + "_end_val";
            astDot << nodeName << "->" << end_valName << std::endl;
            end_val->printSelf(end_valName);
            std::string loopName = nodeName + "_loop";
            astDot << nodeName << "->" << loopName << std::endl;
            loop_stmt->printSelf(loopName);
        }
        virtual llvm::Value *code_gen(CodeGenContext &context);
    };

    class CaseStmt : public Statement {
    public:
        std::shared_ptr<Expression> condition;
        std::shared_ptr<Statement> then_stmt;
        llvm::BasicBlock *bblock, *bexit;
        CaseStmt(std::shared_ptr<Expression> condition, std::shared_ptr<Statement> then_stmt):condition(condition),then_stmt(then_stmt){}
        void printSelf(std::string nodeName)
        {
            std::string condName = nodeName + "_condition";
            astDot << nodeName << "->" << condName << std::endl;
            condition->printSelf(condName);
            std::string thenName = nodeName + "_then";
            astDot << nodeName << "->" << thenName << std::endl;
            then_stmt->printSelf(thenName);
        }
        virtual llvm::Value *code_gen(CodeGenContext &context);
    };

    class SwitchStmt : public Statement {
    public:
        std::shared_ptr<Expression> expression;
        std::shared_ptr<CaseList> list;
        SwitchStmt(std::shared_ptr<Expression> expression,std::shared_ptr<CaseList> list):expression(expression),list(list){}
        void printSelf(std::string nodeName)
        {
            std::string exprName = nodeName + "_expression";
            astDot << nodeName << "->" << exprName << std::endl;
            expression->printSelf(exprName);
            for (int i = 0; i < list.get()->size(); i++)
            {
                std::string caseName = nodeName + "_case" + std::to_string(i);
                astDot << nodeName << "->" << caseName << std::endl;
                (list.get())[i]->printSelf(caseName);
            }            
        }
        virtual llvm::Value *code_gen(CodeGenContext &context);
    };

    class LabelStmt : public Statement {
    public:
        int label;
        std::shared_ptr<Statement> statement;
        LabelStmt(int label,std::shared_ptr<Statement> statement):label(label),statement(statement){}
        void printSelf(std::string nodeName)
        {
            std::string stmtName = nodeName + "_statement";
            astDot << nodeName << "->" << stmtName << std::endl;
            statement->printSelf(stmtName);
        }
        virtual llvm::Value *code_gen(CodeGenContext &context);
    };

    class GotoStmt : public Statement {
    public:
        int label;
        GotoStmt(int label):label(label){}
        void printSelf(std::string nodeName)
        {
            std::string labelName = nodeName + "_label" + std::to_string(label);
            astDot << nodeName << "->" << labelName << std::endl;
        }
        virtual llvm::Value *code_gen(CodeGenContext &context);
    };
} // namespace ast

#endif
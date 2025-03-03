#ifndef _AST_EXPRESSION_HPP
#define _AST_EXPRESSION_HPP

#include "node.hpp"
#include "identifier.hpp"
#include "declation.hpp"
#include "../codegen/CodeGenContext.h"

namespace ast
{
    class Identifier;
    using ExpressionList = std::vector<std::shared_ptr<Expression>>;

    class ArrayAccess : public Expression
    {
    public:
        std::shared_ptr<Identifier> name = nullptr;
        std::shared_ptr<Expression> index = nullptr;
        ArrayAccess(std::string &ID, std::shared_ptr<Expression> &index) : name(make_node<Identifier>(ID)), index(index) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
        llvm::Value *GetPtr(CodeGenContext &context);
    };

    class FuncCall : public Expression, public Statement
    {
    public:
        std::shared_ptr<Identifier> id = nullptr;
        std::shared_ptr<ExpressionList> arg_list = nullptr;

        FuncCall(std::shared_ptr<Identifier> id) : id(id), arg_list(nullptr) {}
        FuncCall(std::shared_ptr<Identifier> id, std::shared_ptr<ExpressionList> arg_list) : id(id), arg_list(arg_list) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class ProcCall : public Statement
    {
    public:
        std::shared_ptr<Identifier> id = nullptr;
        std::shared_ptr<ExpressionList> arg_list = nullptr;

        ProcCall(std::shared_ptr<Identifier> id) : id(id), arg_list(nullptr) {}
        ProcCall(std::shared_ptr<Identifier> id, std::shared_ptr<ExpressionList> arg_list) : id(id), arg_list(arg_list) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class SysFuncCall : public FuncCall
    {
    public:
        SysFuncCall(std::shared_ptr<Identifier> id) : FuncCall(id) {}
        SysFuncCall(std::shared_ptr<Identifier> id, std::shared_ptr<ExpressionList> arg_list) : FuncCall(id, arg_list) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class SysProcCall : public ProcCall
    {
    public:
        SysProcCall(std::shared_ptr<Identifier> id) : ProcCall(id) {}
        SysProcCall(std::shared_ptr<Identifier> id, std::shared_ptr<ExpressionList> arg_list) : ProcCall(id, arg_list) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };

    enum class OpType
    {
        PLUS,
        MINUS,
        MUL,
        DIV,
        MOD,
        AND,
        OR,
        XOR,
        SLASH,
        EQ,
        NE,
        LT,
        GT,
        LE,
        GE
    };

    class BinaryOp : public Expression
    {
    public:
        std::shared_ptr<Expression> op1, op2;
        OpType op;

        BinaryOp(std::shared_ptr<Expression> op1, OpType op, std::shared_ptr<Expression> op2) : op1(op1), op2(op2), op(op) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };
} // namespace ast

#endif
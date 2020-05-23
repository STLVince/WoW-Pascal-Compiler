#ifndef _AST_NODE_HPP_
#define _AST_NODE_HPP_
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <memory>
#include "llvm/IR/Value.h"
#include "llvm/IR/Instruction.h"

namespace ast
{
    class Node
    {
    public:
        virtual ~Node(){};
        //virtual llvm::Value *code_gen(CodeGenContext &context) = 0;
    };

    class Expression : public Node
    {
    public:
        Expression(){};
        ~Expression(){};
        //virtual llvm::Value *code_gen(CodeGenContext &context) = 0;
    };

    class Statement : public Node
    {
    public:
        Statement(){};
        ~Statement(){};
        //virtual llvm::Value *code_gen(CodeGenContext &context) = 0;
        virtual std::vector<Statement *> *get_list() {}
    };

    class StatementList : public Statement
    {
    public:
        std::vector<Statement *> list;
        //virtual llvm::Value *code_gen(CodeGenContext& context) {
        //    for (auto stmt: list){
        //        stmt->code_gen(context);
        //    }
        //}
        virtual std::vector<Statement *> *get_list() { return &list; }
    };
} // namespace ast

#endif
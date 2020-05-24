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
        //virtual std::vector<std::shared_ptr<Node>> getChildren() { return *(new std::vector<std::shared_ptr<Node>>()); }
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
    };
} // namespace ast

#endif
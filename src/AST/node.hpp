#ifndef _AST_NODE_HPP_
#define _AST_NODE_HPP_
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <memory>
#include "llvm/IR/Value.h"
#include "llvm/IR/Instruction.h"

class CodeGenContext;
extern std::ofstream astDot;

namespace ast
{
    class Node
    {
    public:
        ~Node(){};
        virtual void printSelf(std::string nodeName) = 0;
        virtual llvm::Value *code_gen(CodeGenContext &context) = 0;
    };

    class Expression : public Node
    {
    public:
        Expression(){};
        ~Expression(){};
        virtual void printSelf(std::string nodeName) = 0;
        virtual llvm::Value *code_gen(CodeGenContext &context) = 0;
    };

    class Statement : public Node
    {
    public:
        Statement(){};
        ~Statement(){};
        virtual void printSelf(std::string nodeName) = 0;
        virtual llvm::Value *code_gen(CodeGenContext &context) = 0;
    };

    class StatementList : public Statement{
    public:
        std::vector<std::shared_ptr<Statement>> list;        
        std::vector<std::shared_ptr<Statement>> *get_list(){ return &list;}
        void printSelf(std::string nodeName)
        {
            for (auto stmt : list)
            {
                stmt->printSelf(nodeName);
            }
        }
        llvm::Value *code_gen(CodeGenContext &context)
        {
            for (auto stmt : list)
            {
                stmt->code_gen(context);
            }
            return nullptr;
        }
    };

    template<typename T, typename... Args>
    std::shared_ptr<T> make_node(Args&&... args) {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
} // namespace ast

#endif
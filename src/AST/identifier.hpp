#ifndef _AST_IDENTIFIER_HPP_
#define _AST_IDENTIFIER_HPP_

#include "node.hpp"

namespace ast
{
    class Identifier : public Expression
    {
    public:
        std::string name;
        
        Identifier(const std::string &name) : name(name) {}
        //Identifier(const char* ptr) : name(*(new std::string(ptr))) {}
        void printSelf(std::string nodeName) {}
        llvm::Value *GetPtr(CodeGenContext &context);
        llvm::Value *code_gen(CodeGenContext &context);
    };
} // namespace ast

#endif
#ifndef _AST_IDENTIFIER_HPP_
#define _AST_IDENTIFIER_HPP_

#include "node.hpp"
#include "../codegen/CodeGenContext.h"

namespace ast
{
    class Identifier : public Expression
    {
    public:
        std::string name;
        
        Identifier(const std::string &name) : name(name) {}
        ~Identifier() {};
        llvm::Value *GetPtr(CodeGenContext &context);
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };
} // namespace ast

#endif
#ifndef _AST_DECLATION_HPP
#define _AST_DECLATION_HPP

#include "node.hpp"
#include "identifier.hpp"
#include "type.hpp"
#include "../codegen/CodeGenContext.h"

namespace ast
{
    class Identifier;
    class TypeDecl;
    class ConstType;

    class TypeDef : public Statement
    {
    public:
        std::shared_ptr<Identifier> name = nullptr;
        std::shared_ptr<TypeDecl> type = nullptr;

        TypeDef(std::shared_ptr<Identifier> name, std::shared_ptr<TypeDecl> td) : name(name), type(td) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class ConstDecl : public Statement
    {
    public:
        std::shared_ptr<Identifier> name = nullptr;
        std::shared_ptr<TypeDecl> type = nullptr;
        std::shared_ptr<ConstType> val = nullptr;

        ConstDecl(std::shared_ptr<Identifier> name, std::shared_ptr<ConstType> val);
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class VarDecl : public Statement
    {
    public:
        std::shared_ptr<Identifier> name = nullptr;
        std::shared_ptr<TypeDecl> type = nullptr;
        bool is_global;

        VarDecl(std::shared_ptr<Identifier> name, std::shared_ptr<TypeDecl> type) : name(name), type(type) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };
} // namespace ast

#endif
#ifndef _AST_DECLATION_HPP
#define _AST_DECLATION_HPP

#include "node.hpp"
#include "identifier.hpp"
#include "type.hpp"
#include <fstream>

extern std::ofstream astDot;

namespace ast
{
    class TypeDef : public Statement
    {
    public:
        std::shared_ptr<Identifier> name = nullptr;
        std::shared_ptr<TypeDecl> type = nullptr;

        TypeDef(std::shared_ptr<Identifier> name, std::shared_ptr<TypeDecl> td) : name(name), type(td) {}
        /*virtual std::vector<std::shared_ptr<Node>> getChildren()
        {
            std::vector<std::shared_ptr<Node>> list;
            list.push_back(name);
            list.push_back(type);
            return list;
        }*/
        void printSelf(std::string nodeName)
        {
            astDot << nodeName << "->" << nodeName + "_TypeDecl_" << static_cast<std::underlying_type<TypeName>::type>(type->type) << std::endl;
        }
        virtual llvm::Value *code_gen(CodeGenContext &context) = 0;
    };

    class ConstDecl : public Statement
    {
    public:
        std::shared_ptr<Identifier> name = nullptr;
        std::shared_ptr<TypeDecl> type = nullptr;
        std::shared_ptr<ConstType> val = nullptr;

        ConstDecl(std::shared_ptr<Identifier> name, std::shared_ptr<ConstType> val) : name(name), val(val), type(new TypeDecl(val->getConstType())) {}
        /*virtual std::vector<std::shared_ptr<Node>> getChildren()
        {
            std::vector<std::shared_ptr<Node>> list;
            list.push_back(name);
            list.push_back(type);
            list.push_back(val);
            return list;
        }*/
        void printSelf(std::string nodeName)
        {
            astDot << nodeName << "->" << nodeName + "_TypeDecl_" << static_cast<std::underlying_type<TypeName>::type>(type->type) << "_" << static_cast<std::underlying_type<TypeName>::type>(val->type) << std::endl;
        }
        virtual llvm::Value *code_gen(CodeGenContext &context);
    };

    class VarDecl : public Statement
    {
    public:
        std::shared_ptr<Identifier> name = nullptr;
        std::shared_ptr<TypeDecl> type = nullptr;
        bool is_global;

        VarDecl(Identifier *name, TypeDecl *type) : name(name), type(type) {}
        /*virtual std::vector<std::shared_ptr<Node>> getChildren()
        {
            std::vector<std::shared_ptr<Node>> list;
            list.push_back(name);
            list.push_back(type);
            return list;
        }*/
        void printSelf(std::string nodeName)
        {
            astDot << nodeName << "->" << nodeName + "_TypeDecl_" << static_cast<std::underlying_type<TypeName>::type>(type->type);
            astDot << "_" << (is_global ? "global" : "not_global") << std::endl;
        }
        virtual llvm::Value *code_gen(CodeGenContext &context);
    };
} // namespace ast

#endif
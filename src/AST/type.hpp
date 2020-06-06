#ifndef _AST_TYPE_HPP_
#define _AST_TYPE_HPP_

#include "node.hpp"
#include "identifier.hpp"
#include "../codegen/CodeGenContext.h"

namespace ast
{
    enum class TypeName
    {
        UNKNOWN,
        INTEGER,
        REAL,
        CHARACTER,
        STRING,
        BOOLEAN,
        ARRAY,
        RECORD
    };

    class IntegerType;

    class TypeDecl : public Statement
    {
    public:
        TypeName type = TypeName::UNKNOWN;

        TypeDecl(){};
        TypeDecl(TypeName type) : type(type) {}
        ~TypeDecl(){};
        void printSelf(std::string nodeName) {}
        llvm::Type *getType(CodeGenContext &context);
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class ArrayType : public TypeDecl
    {
    public:
        int start, end;
        std::shared_ptr<TypeDecl> array_type;
        ArrayType(int start, int end, std::shared_ptr<TypeDecl> &array_type) : start(start), end(end), array_type(array_type) { this->type = TypeName::ARRAY; }
        void printSelf(std::string nodeName) {}
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class ConstType : public Expression
    {
    public:
        TypeName type = TypeName::UNKNOWN;

        virtual TypeName getConstType() = 0;
        llvm::Type *getType(CodeGenContext &context);
        ConstType(){};
        ~ConstType(){};
        void printSelf(std::string nodeName) {}
    };

    class IntegerType : public ConstType
    {
    public:
        int val;

        IntegerType(int val) : val(val) {}
        TypeName getConstType()
        {
            return TypeName::INTEGER;
        }
        void printSelf(std::string nodeName) {}
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class RealType : public ConstType
    {
    public:
        double val;

        RealType(double val) : val(val) {}
        TypeName getConstType()
        {
            return TypeName::REAL;
        }

        void printSelf(std::string nodeName) {}
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class BooleanType : public ConstType
    {
    public:
        bool val;

        BooleanType(bool val) : val(val) {}
        TypeName getConstType()
        {
            return TypeName::BOOLEAN;
        }
        void printSelf(std::string nodeName) {}
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class StringType : public ConstType
    {
    public:
        std::string val;

        StringType(const std::string val) : val(val)
        {
            this->val.erase(this->val.begin());
            this->val.pop_back();
        }
        TypeName getConstType()
        {
            return TypeName::STRING;
        }
        void printSelf(std::string nodeName) {}
        llvm::Value *code_gen(CodeGenContext &context);
    };

    class CharType : public ConstType
    {
    public:
        char val;

        CharType(const char val) : val(val) {}
        CharType(const char *val) : val(val[0]) {}
        TypeName getConstType()
        {
            return TypeName::CHARACTER;
        }
        void printSelf(std::string nodeName) {}
        llvm::Value *code_gen(CodeGenContext &context);
    };
} // namespace ast

#endif
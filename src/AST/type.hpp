#ifndef _AST_TYPE_HPP_
#define _AST_TYPE_HPP_

#include "node.hpp"
#include "identifier.hpp"
#include <fstream>

extern std::ofstream astDot;
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
        RANGE,
        ARRAY,
        RECORD
    };

    class TypeDecl : public Statement
    {
    public:
        TypeName type = TypeName::UNKNOWN;

        TypeDecl(){};
        TypeDecl(TypeName type) : type(type) {}
        ~TypeDecl(){};
        void printSelf(std::string nodeName) {}
        llvm::Type *getType();
        virtual llvm::Value *code_gen(CodeGenContext &context);
    };

    class ArrayType : public TypeDecl
    {
    };

    class RecordType : public TypeDecl
    {
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
        virtual TypeName getConstType()
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
        virtual TypeName getConstType()
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
        virtual TypeName getConstType()
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

        StringType(const char *val) : val(val) {}
        StringType(const std::string val) : val(val) {}
        virtual TypeName getConstType()
        {
            return TypeName::STRING;
        }
        void printSelf(std::string nodeName) {}
        llvm::Value *code_gen(CodeGenContext &context);
    };
} // namespace ast

#endif
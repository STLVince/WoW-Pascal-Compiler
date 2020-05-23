#ifndef _AST_TYPE_HPP_
#define _AST_TYPE_HPP_

#include "node.hpp"
#include "identifier.hpp"

namespace ast
{
    enum class TypeName
    {
        UNKNOWN,
        INTEGER,
        REAL,
        STRING,
        BOOLEAN,
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
        //virtual llvm::Value *code_gen(CodeGenContext &context) = 0;
        //llvm::Type *get_type(CodeGenContext &context);
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
        ConstType(){};
        ~ConstType(){};
    };

    class IntegerType : public ConstType
    {
    public:
        int val;
        IntegerType(int val) : val(val) {}
        virtual TypeName getConstType() { return TypeName::INTEGER; }
        //virtual llvm::Value *code_gen(CodeGenContext &context) = 0;
    };

    class RealType : public ConstType
    {
    public:
        double val;
        RealType(double val) : val(val) {}
        virtual TypeName getConstType() { return TypeName::REAL; }
        //virtual llvm::Value *code_gen(CodeGenContext &context) = 0;
    };

    class BooleanType : public ConstType
    {
    public:
        bool val;
        BooleanType(bool val) : val(val) {}
        virtual TypeName getConstType() { return TypeName::BOOLEAN; }
        //virtual llvm::Value *code_gen(CodeGenContext &context) = 0;
    };

    class StringType : public ConstType
    {
    public:
        std::string val;
        StringType(const char *val) : val(val) {}
        StringType(const std::string val) : val(val) {}
        virtual TypeName getConstType() { return TypeName::STRING; }
        //virtual llvm::Value *code_gen(CodeGenContext &context) = 0;
    };
} // namespace ast

#endif
#ifndef _AST_PROGRAM_HPP
#define _AST_PROGRAM_HPP

#include "node.hpp"
#include "identifier.hpp"
#include "declation.hpp"
#include "expression.hpp"
#include "type.hpp"
#include "statement.hpp"
#include "../codegen/CodeGenContext.h"
namespace ast
{    
    class ConstDecl;
    class TypeDef;
    class VarDecl;
    class Routine;
    class TypeDecl;
    using ConstDeclList = std::vector<std::shared_ptr<ConstDecl>>;
    using TypeDeclList = std::vector<std::shared_ptr<TypeDef>>;
    using VarDeclList = std::vector<std::shared_ptr<VarDecl>>;
    using RoutineList = std::vector<std::shared_ptr<Routine>>;
    using NameList = std::vector<std::string>;

    class Program : public Node
    {
    public:
        std::shared_ptr<ConstDeclList> const_part;
        std::shared_ptr<TypeDeclList> type_part;
        std::shared_ptr<VarDeclList> var_part;
        std::shared_ptr<RoutineList> routine_part;
        std::shared_ptr<StatementList> routine_body;

        Program() {}
        Program(std::shared_ptr<ConstDeclList> const_part, std::shared_ptr<TypeDeclList> type_part, std::shared_ptr<VarDeclList> var_part, std::shared_ptr<RoutineList> routine_part, std::shared_ptr<StatementList> routine_body) : const_part(const_part), type_part(type_part), var_part(var_part), routine_part(routine_part), routine_body(routine_body) {}
        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };

    enum class RoutineType
    {
        UNKNOWN,
        FUNCTION,
        PROCEDURE
    };

    class Routine : public Program
    {
    public:
        std::shared_ptr<Identifier> name;
        std::shared_ptr<TypeDecl> type;
        std::shared_ptr<VarDeclList> arg_list;
        RoutineType routine_type;

        Routine(std::shared_ptr<Identifier> name, std::shared_ptr<TypeDecl> type, std::shared_ptr<VarDeclList> arg_list, RoutineType routine_type) : Program(), name(name), type(type), arg_list(arg_list), routine_type(routine_type) {}
        Routine(std::shared_ptr<Routine> routine_ptr, std::shared_ptr<Program> program_ptr) : Program(*program_ptr), name(routine_ptr->name), type(routine_ptr->type), arg_list(routine_ptr->arg_list), routine_type(routine_ptr->routine_type) {}

        bool isFunction() { return routine_type == RoutineType::FUNCTION; }
        bool isProcedure() { return routine_type == RoutineType::PROCEDURE; }

        void printSelf(std::string nodeName);
        llvm::Value *code_gen(CodeGenContext &context);
    };
} // namespace ast

#endif
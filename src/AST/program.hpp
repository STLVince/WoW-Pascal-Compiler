#ifndef _AST_PROGRAM_HPP
#define _AST_PROGRAM_HPP

#include "node.hpp"
#include "identifier.hpp"
#include "declation.hpp"
#include "expression.hpp"
#include "type.hpp"
#include "statement.hpp"

namespace ast
{
    class Routine;
    using ConstDeclList = std::vector<std::shared_ptr<ConstDecl>>;
    using TypeDeclList = std::vector<std::shared_ptr<TypeDef>>;
    using VarDeclList = std::vector<std::shared_ptr<VarDecl>>;
    using RoutineList = std::vector<std::shared_ptr<Routine>>;
    //using StatementList = std::vector<std::shared_ptr<Statement>>;
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
        /*virtual std::vector<std::shared_ptr<Node>> getChildren()
        {
            std::vector<std::shared_ptr<Node>> list;
            for (auto i : *(const_part))
                list.push_back((std::shared_ptr<Node>)i);
            for (auto i : *(type_part))
                list.push_back((std::shared_ptr<Node>)i);
            for (auto i : *(var_part))
                list.push_back((std::shared_ptr<Node>)i);
            //for (auto i : *(routine_part))
                //list.push_back((std::shared_ptr<Node>)i);
            for (auto i : *(routine_body))
                list.push_back((std::shared_ptr<Node>)i);
            return list;
        }*/
        //virtual llvm::Value *code_gen(CodeGenContext &context);
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

        /*virtual std::vector<std::shared_ptr<Node>> getChildren()
        {
            std::vector<std::shared_ptr<Node>> list;
            list.push_back((std::shared_ptr<Node>)name);
            list.push_back((std::shared_ptr<Node>)type);
            for (auto i : *(arg_list))
                list.push_back((std::shared_ptr<Node>)i);
            for (auto i : *(const_part))
                list.push_back((std::shared_ptr<Node>)i);
            for (auto i : *(var_part))
                list.push_back((std::shared_ptr<Node>)i);
            //for (auto i : *(routine_part))
                //list.push_back((std::shared_ptr<Node>)i);
            for (auto i : *(routine_body))
                list.push_back((std::shared_ptr<Node>)i);
            return list;
        }*/
        //virtual llvm::Value *code_gen(CodeGenContext &context);
    };
} // namespace ast

#endif
#include "../AST/program.hpp"
namespace ast
{
    void Program::printSelf(std::string nodeName)
    {
        astDot << "digraph AST {" << std::endl;
        // std::string nodeName = "ProgramNode";

        // const decl part
        for (auto const_decl : *(this->const_part))
        {
            std::string childName = nodeName + "_const_decl_" + const_decl->name->name;
            astDot << nodeName << "->" << childName << std::endl;
            const_decl->printSelf(childName);
        }

        // deal with variable declaration
        for (auto var_decl : *(this->var_part))
        {
            std::string childName = nodeName + "_var_decl_" + var_decl->name->name;
            astDot << nodeName << "->" << childName << std::endl;
            var_decl->printSelf(childName);
        }

        for (unsigned int i = 0; i < routine_part.get()->size(); i++)
        {
            std::string childName = nodeName + "_routinePart" + std::to_string(i);
            astDot << nodeName << "->" << childName << std::endl;
            ((*(routine_part.get()))[i]).get()->printSelf(childName);
        }

        // deal with program statements
        for (unsigned int i = 0; i < this->routine_body.get()->get_list()->size(); i++)
        {
            std::string childName = nodeName + "_routineBody" + std::to_string(i);
            astDot << nodeName << "->" << childName << std::endl;
            (*(routine_body.get()->get_list()))[i].get()->printSelf(childName);
        }
        astDot << "}" << std::endl;
    }

    void Routine::printSelf(std::string nodeName)
    {
        astDot << nodeName << "->" << nodeName + "_TypeDecl_" << static_cast<std::underlying_type<TypeName>::type>(type->type) << std::endl;
        for (auto arg : *(this->arg_list))
        {
            std::string childName = nodeName + "_VarDecl_" + arg->name->name;
            astDot << nodeName << "->" << childName << std::endl;
            arg->printSelf(childName);
        }
    }

    llvm::Value *Program::code_gen(CodeGenContext &context)
    {
        codegenOutput << "Program::code_gen: inside program" << std::endl;

        // Create the top level interpreter function to call as entry
        std::vector<llvm::Type *> argTypes;
        llvm::FunctionType *ftype = llvm::FunctionType::get(llvm::Type::getVoidTy(GlobalLLVMContext::getGlobalContext()), makeArrayRef(argTypes), false);

        context.mainFunction = llvm::Function::Create(ftype, llvm::GlobalValue::ExternalLinkage, "main", context.module);
        llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "entry", context.mainFunction, 0);
        context.Builder.SetInsertPoint(basicBlock);
        context.currentFunction = context.mainFunction;

        llvm::Value *last = nullptr;

        // const decl part
        for (auto const_decl : *(this->const_part))
        {
            codegenOutput << "Program::code_gen: generating code for const_decl " << const_decl->name->name << std::endl;
            last = const_decl->code_gen(context);
        }

        // deal with variable declaration
        for (auto var_decl : *(this->var_part))
        {
            codegenOutput << "Program::code_gen: generating code for var_decl " << var_decl->name->name << std::endl;
            var_decl->is_global = 1;
            last = var_decl->code_gen(context);
        }

        for (auto routine : *(this->routine_part))
        {
            codegenOutput << "Program::code_gen: generating code for routine " << routine->name->name << std::endl;
            last = routine->code_gen(context);
        }

        context.Builder.SetInsertPoint(basicBlock);
        // deal with program statements
        for (auto body : *(this->routine_body.get()->get_list()))
        {
            codegenOutput << "Program::code_gen: generating code for body" << typeid(body).name() << std::endl;
            last = body->code_gen(context);
        }
        context.Builder.CreateRetVoid();
        codegenOutput << "Program::code_gen: creating program done" << std::endl;
        return last;
    }

    llvm::Value *Routine::code_gen(CodeGenContext &context)
    {
        codegenOutput << "Routine::code_gen: inside routine definition" << std::endl;

        // make the function type
        llvm::FunctionType *func_type;
        std::vector<llvm::Type *> arg_types;
        std::vector<std::string> arg_names;
        for (auto arg : *(this->arg_list))
        {
            arg_types.push_back(arg->type->getType(context));
            arg_names.push_back(arg->name->name);
        }
        if (this->isProcedure())
        {
            func_type = llvm::FunctionType::get(llvm::Type::getVoidTy(GlobalLLVMContext::getGlobalContext()), llvm::makeArrayRef(arg_types), false);
        }
        else
        {
            func_type = llvm::FunctionType::get(this->type->getType(context), llvm::makeArrayRef(arg_types), false);
        }

        // create function
        auto function = llvm::Function::Create(func_type, llvm::GlobalValue::ExternalLinkage, this->name->name.c_str(), context.module);

        codegenOutput << "Routine::code_gen: created function definition" << std::endl;

        if (function->getName() != this->name->name)
        {
            // Delete the one we just made and get the existing one.
            function->eraseFromParent();
            function = context.module->getFunction(this->name->name);
        }
        // If function already has a body, reject this.
        if (!function->empty())
        {
            std::cerr << "Routine::code_gen: redefinition of function " << this->name->name << std::endl;
            return 0;
        }

        // create a new basic block to start insertion into
        auto block = llvm::BasicBlock::Create(context.module->getContext(), "entry", function);

        // record original function and block
        auto oldFunction = context.currentFunction;
        context.currentFunction = function;
        // auto oldBlock = context.Builder.GetInsertBlock();
        context.functionParent[function] = oldFunction;
        context.Builder.SetInsertPoint(block);

        codegenOutput << "Routine::code_gen: start initializing arguments" << std::endl;

        // initialize arguments
        // auto index = 0;
        // for (auto &arg : function->args())
        // {
        //     auto *type = arg.getType(context);
        //     llvm::Constant *constant;
        //     if (type->isIntegerTy(32))
        //     {
        //         constant = llvm::ConstantInt::get(type, 0);
        //     }
        //     else if (type->isDoubleTy())
        //     {
        //         constant = llvm::ConstantFP::get(type, 0.0);
        //     }
        //     else
        //     {
        //         std::cerr << "Routine::code_gen: unknown function argument type" << std::endl;
        //     }
        //     std::string prefix(name->name);
        //     std::cout << prefix + "_" + arg_names[index++] << std::endl;
        //     auto *variable = new llvm::GlobalVariable(*context.module, type, false, llvm::GlobalVariable::ExternalLinkage, constant, prefix + "_" + arg_names[index++]);
        //     context.Builder.CreateStore(&arg, variable);
        //     //auto *local = context.GetBuilder().CreateAlloca(arg.getType());
        //     //context.SetValue(names[index++], local);
        //     //context.GetBuilder().CreateStore(&arg, local);
        // }
        llvm::Value *parameter_value;
        auto parameter_iter = function->arg_begin();
        for (auto arg : *(arg_list))
        {
            codegenOutput << "Routine::code_gen: gencode for argument " << arg->name->name << std::endl;
            arg->code_gen(context);
            parameter_value = parameter_iter++;
            parameter_value->setName(arg->name->name.c_str());
            auto inst = new llvm::StoreInst(parameter_value, context.getValue(arg->name->name), false, block);
        }
        codegenOutput << "Routine::code_gen: argument initializing success!\n";

        // allocate return variable
        // if (this->isFunction())
        // {
        //     codegenOutput << "Routine::code_gen: creating function return value declaration" << std::endl;
        //     // TODO check this
        //     auto *alloc = context.Builder.CreateAlloca(this->type->getType(context));
        //     // context.insert(this->routine_name->name) = alloc;
        // }
        // codegenOutput << "Routine::code_gen: function part success!\n";

        // const decl part
        for (auto const_decl : *(this->const_part))
        {
            codegenOutput << "Routine::code_gen: generating code for const_decl " << const_decl->name->name << std::endl;
            const_decl->code_gen(context);
        }

        // deal with variable declaration
        for (auto var_decl : *(this->var_part))
        {
            codegenOutput << "Routine::code_gen: generating code for var_decl " << var_decl->name->name << std::endl;
            var_decl->is_global = 0;
            var_decl->code_gen(context);
        }

        // set the return variable
        if (this->isFunction())
        {
            // auto *type = this->type->getType(context);
            // llvm::Constant *constant;
            // if (type->isIntegerTy(32))
            // {
            //     constant = llvm::ConstantInt::get(type, 0);
            // }
            // else if (type->isDoubleTy())
            // {
            //     constant = llvm::ConstantFP::get(type, 0.0);
            // }
            // else
            // {
            //     std::cerr << "Routine::code_gen: unknown function return type" << std::endl;
            // }
            // std::string prefix(name->name);
            // // std::cout << prefix << std::endl;
            // // context.GetTrace().push_back(prefix);
            // auto *variable = new llvm::GlobalVariable(*context.module, type, false, llvm::GlobalVariable::ExternalLinkage, constant, prefix + "_" + name->name);
            codegenOutput << "Routine::code_gen: generating code for return variable " << name->name << std::endl;
            context.Builder.CreateAlloca(type->getType(context), 0, name->name);

            //auto *ret = context.GetBuilder().CreateAlloca(proghead->type->GetType(context));
            //context.SetValue(proghead->name->GetName(), ret);
        }

        for (auto routine : *(this->routine_part))
        {
            codegenOutput << "Routine::code_gen: generating code for routine " << routine->name->name << std::endl;
            routine->code_gen(context);
        }

        context.Builder.SetInsertPoint(block);

        // deal with program statements
        for (auto body : *(this->routine_body.get()->get_list()))
        {
            codegenOutput << "Routine::code_gen: generating code for body" << std::endl;
            body->code_gen(context);
        }

        // return value
        if (this->isFunction())
        {
            codegenOutput << "Routine::code_gen: generating return value for function" << std::endl;
            auto *local = context.getValue(this->name->name);
            auto *ret = context.Builder.CreateLoad(local);
            context.Builder.CreateRet(ret);
        }
        else if (this->isProcedure())
        {
            codegenOutput << "Routine::code_gen: generating return void for procedure" << std::endl;
            context.Builder.CreateRetVoid();
        }

        // restore current function and block
        // context.Builder.SetInsertPoint(oldBlock);
        context.currentFunction = oldFunction;

        // verify the function
        llvm::verifyFunction(*function, &llvm::errs());

        // perform optimization
        // context.fpm->run(*function);

        return function;
    }
} // namespace ast

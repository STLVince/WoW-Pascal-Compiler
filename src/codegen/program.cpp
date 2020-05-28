#include "../AST/program.hpp"
namespace ast
{
    llvm::Value *Program::code_gen(CodeGenContext &context)
    {
        std::cerr << "Program::code_gen: inside program" << std::endl;
        
        llvm::Value *last = nullptr;

        // const decl part
        for (auto const_decl : *(this->const_part))
        {
            std::cerr << "Program::code_gen: generating code for " << typeid(const_decl).name() << std::endl;
            last = const_decl->code_gen(context);
        }

        // deal with variable declaration
        for (auto var_decl : *(this->var_part))
        {
            std::cerr << "Program::code_gen: generating code for " << typeid(var_decl).name() << std::endl;
            var_decl->is_global = 1;
            last = var_decl->code_gen(context);
        }

        for (auto routine : *(this->routine_part))
        {
            std::cerr << "Program::code_gen: generating code for " << typeid(routine).name() << std::endl;
            last = routine->code_gen(context);
        }

        // deal with program statements
        for (auto body : *(this->routine_body))
        {
            std::cerr << "Program::code_gen: generating code for " << typeid(body).name() << std::endl;
            last = body->code_gen(context);
        }
        std::cerr << "Program::code_gen: creating program" << std::endl;
        return last;
    }

    llvm::Value *Routine::code_gen(CodeGenContext &context)
    {
        std::cerr << "Routine::code_gen: inside rountine definition" << std::endl;

        // make the function type
        llvm::FunctionType *f_type;
        std::vector<llvm::Type *> arg_types;
        for (auto arg : *(this->arg_list))
        {
            arg_types.push_back(arg->type->getType());
        }
        if (this->isProcedure())
        {
            f_type = llvm::FunctionType::get(llvm::Type::getVoidTy(GlobalLLVMContext::getGlobalContext()), llvm::makeArrayRef(arg_types), false);
        }
        else
        {
            f_type = llvm::FunctionType::get(this->type->getType(), llvm::makeArrayRef(arg_types), false);
        }

        // create function
        auto function = llvm::Function::Create(f_type, llvm::GlobalValue::ExternalLinkage, this->name->name.c_str(), context.module);

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
        auto block = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "entry", function, NULL);

        // record original function and block
        auto oldFunction = context.currentFunction;
        context.currentFunction = function;
        auto oldBlock = context.currentBlock();
        context.functionParent[function] = oldFunction;
        // push block and start routine
        context.pushBlock(block);

        // initialize arguments
        llvm::Value *arg_value;
        auto args_values = function->arg_begin();
        for (auto arg : *(this->arg_list))
        {
            arg->code_gen(context);
            arg_value = args_values++;
            arg_value->setName(arg->name->name.c_str());
            auto inst = new llvm::StoreInst(arg_value, context.getValue(arg->name->name), false, block);
        }
        std::cerr << "Routine::code_gen: argument part suc!\n";

        // allocate return variable
        if (this->isFunction())
        {
            std::cerr << "Routine::code_gen: creating function return value declaration" << std::endl;
            // TODO check this
            auto alloc = new llvm::AllocaInst(this->type->getType(), 4, this->name->name.c_str(), context.currentBlock());
            // context.insert(this->routine_name->name) = alloc;
        }
        std::cerr << "Routine::code_gen: function part success!\n";

        // deal with variable declaration
        for (auto var_decl : *(this->var_part))
        {
            std::cerr << "Routine::code_gen: generating code for variable declaration " << typeid(var_decl).name() << std::endl;
            var_decl->code_gen(context);
        }

        // deal with routine declaration
        for (auto routine : *(this->routine_part))
        {
            std::cerr << "Routine::code_gen: generating code for " << typeid(routine).name() << std::endl;
            routine->code_gen(context);
        }

        // deal with program statements
        std::cerr << "Routine::code_gen: var part suc!\n";
        for (auto body : *(this->routine_body))
        {
            std::cerr << "Routine::code_gen: generating code for " << typeid(body).name() << std::endl;
            body->code_gen(context);
        }

        // return value
        if (this->isFunction())
        {
            std::cerr << "Routine::code_gen: generating return value for function" << std::endl;
            auto load_ret = new llvm::LoadInst(context.getValue(this->name->name), "", false, context.currentBlock());
            llvm::ReturnInst::Create(GlobalLLVMContext::getGlobalContext(), load_ret, context.currentBlock());
        }
        else if (this->isProcedure())
        {
            std::cerr << "Routine::code_gen: generating return void for procedure" << std::endl;
            llvm::ReturnInst::Create(GlobalLLVMContext::getGlobalContext(), context.currentBlock());
        }

        // restore current function and block
        while (context.currentBlock() != oldBlock)
        {
            context.popBlock();
        }
        context.currentFunction = oldFunction;

        // verify the function
        llvm::verifyFunction(*function, &llvm::errs());

        // perform optimization
        context.fpm->run(*function);

        return function;
    }
} // namespace ast

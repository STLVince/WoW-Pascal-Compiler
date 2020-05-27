#include "../AST/program.hpp"

namespace ast
{
    llvm::Value *Program::code_gen(CodeGenContext &context)
    {
        llvm::Value *last = nullptr;

        /* const decl part */
        for (auto const_decl : *(this->const_part))
        {
            std::cout << "Generating code for " << typeid(const_decl).name() << std::endl;
            last = const_decl->code_gen(context);
        }
        // deal with variable declaration

        for (auto var_decl : *(this->var_part))
        {
            std::cout << "Generating code for " << typeid(var_decl).name() << std::endl;
            var_decl->is_global = 1;
            last = var_decl->code_gen(context);
        }
        for (auto routine : *(this->routine_part))
        {
            std::cout << "Generating code for " << typeid(routine).name() << std::endl;
            last = routine->code_gen(context);
        }
        // deal with program statements
        for (auto body : *(this->routine_body))
        {
            std::cout << "Generating code for " << typeid(body).name() << std::endl;
            last = body->code_gen(context);
        }
        std::cout << "Creating program" << std::endl;
        return last;
    }

    llvm::Value *ast::Routine::code_gen(CodeGenContext &context)
    {
        std::vector<llvm::Type *> arg_types;
        for (auto it : *(this->arg_list))
            arg_types.push_back(it->type->getType());
        auto f_type = llvm::FunctionType::get(this->isProcedure() ? llvm::Type::getVoidTy(GlobalLLVMContext::getGlobalContext()) : this->type->getType(), llvm::makeArrayRef(arg_types), false);
        auto function = llvm::Function::Create(f_type, llvm::GlobalValue::InternalLinkage, this->name->name.c_str(), context.module);
        auto block = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "entry", function, NULL);
        auto oldFunc = context.currentFunction;
        context.currentFunction = function;
        auto oldBlock = context.currentBlock();
        context.functionParent[function] = oldFunc;
        // push block and start routine
        context.pushBlock(block);

        // deal with arguments
        llvm::Value *arg_value;
        auto args_values = function->arg_begin();
        for (auto arg : *(this->arg_list))
        {
            arg->code_gen(context);
            arg_value = args_values++;
            arg_value->setName(arg->name->name.c_str());
            auto inst = new llvm::StoreInst(arg_value, context.getValue(arg->name->name), false, block);
        }

        std::cout << "is func?" << this->isFunction() << " part suc!\n";
        // add function return variable
        if (this->isFunction())
        {
            std::cout << "Creating function return value declaration" << std::endl;
            auto alloc = new llvm::AllocaInst(this->type->getType(), 4,this->name->name.c_str(), context.currentBlock());
            // context.insert(this->routine_name->name) = alloc;
        }
        std::cout << "func part suc!\n";
        // deal with variable declaration
        for (auto var_decl : *(this->var_part))
        {
            std::cout << "Generating code for decl " << typeid(var_decl).name() << std::endl;
            var_decl->code_gen(context);
        }
        for (auto routine : *(this->routine_part))
        {
            std::cout << "Generating code for " << typeid(routine).name() << std::endl;
            routine->code_gen(context);
        }
        // deal with program statements
        std::cout << "var part suc!\n";
        for (auto body : *(this->routine_body))
        {
            std::cout << "Generating code for " << typeid(body).name() << std::endl;
            body->code_gen(context);
        }
        // routine_body->code_gen(context);
        // return value

        if (this->isFunction())
        {
            std::cout << "Generating return value for function" << std::endl;
            auto load_ret = new llvm::LoadInst(context.getValue(this->name->name), "", false, context.currentBlock());
            llvm::ReturnInst::Create(GlobalLLVMContext::getGlobalContext(), load_ret, context.currentBlock());
        }
        else if (this->isProcedure())
        {
            std::cout << "Generating return void for procedure" << std::endl;
            llvm::ReturnInst::Create(GlobalLLVMContext::getGlobalContext(), context.currentBlock());
        }

        // pop block and finsh
        while (context.currentBlock() != oldBlock)
            context.popBlock();
        context.currentFunction = oldFunc;
        // std::cout << "Creating " << this->toString() << ":" << this->routine_name->name << std::endl;
        return function;
    }
} // namespace ast

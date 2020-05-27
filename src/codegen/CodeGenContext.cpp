#include <utility>
#include <iostream>

#include "CodeGenContext.h"

CodeGenContext::CodeGenContext() : Builder(GlobalLLVMContext::getGlobalContext())
{
    module = new llvm::Module("Pascal", GlobalLLVMContext::getGlobalContext());
}

std::map<std::string, llvm::Value *> &CodeGenContext::locals()
{
    return blocks.top()->locals;
}

llvm::Value *CodeGenContext::getValue(std::string name)
{
    llvm::Function *nowFunction = currentFunction;

    std::cout << "Start getValue for " << name << std::endl;
    std::cout << "found:" << currentFunction->getValueSymbolTable()->lookup(name) << std::endl;
    std::cout << "main:" << mainFunction << std::endl;
    std::cout << "current:" << currentFunction << std::endl;
    if ((nowFunction->getValueSymbolTable()->lookup(name)) == NULL)
    {
        if (module->getGlobalVariable(name) == NULL)
        {
            throw std::logic_error("Undeclared variable " + name);
            return nullptr;
        }
        return module->getGlobalVariable(name);
    }
    return nowFunction->getValueSymbolTable()->lookup(name);
}

void CodeGenContext::putValue(std::string name, llvm::Value *value)
{
    blocks.top()->locals.insert(make_pair(name, value));
}

llvm::Type *CodeGenContext::getAlias(std::string key)
{
    auto V = aliases[key];
    if (!V)
    {
        return nullptr;
    }
    return V;
}
bool CodeGenContext::setAlias(std::string key, llvm::Type *value)
{
    if (getAlias(key))
    {
        return false;
    }
    aliases[key] = value;
    return true;
}

llvm::BasicBlock *CodeGenContext::currentBlock()
{
    return blocks.top()->block;
};

void CodeGenContext::pushBlock(llvm::BasicBlock *block)
{
    CodeGenBlock *newblock = new CodeGenBlock();
    blocks.push(newblock);
    blocks.top()->returnValue = nullptr;
    blocks.top()->block = block;
}

void CodeGenContext::popBlock()
{
    CodeGenBlock *top = blocks.top();
    blocks.pop();
    delete top;
}

llvm::Function *CodeGenContext::getPrintfPrototype()
{
    llvm::LLVMContext &context = GlobalLLVMContext::getGlobalContext();
    std::vector<llvm::Type *> printf_arg_types;
    printf_arg_types.push_back(llvm::Type::getInt8PtrTy(context));

    llvm::FunctionType *printf_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), printf_arg_types, true);

    llvm::Function *func = llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, llvm::Twine("printf"), this->module);
    func->setCallingConv(llvm::CallingConv::C);
    return func;
}

/* Compile the AST into a module */
void CodeGenContext::generateCode(ast::Program &root)
{

    std::cout << "Generating code...\n";

    /* Create the top level interpreter function to call as entry */
    std::vector<llvm::Type *> argTypes;
    llvm::FunctionType *ftype = llvm::FunctionType::get(llvm::Type::getVoidTy(GlobalLLVMContext::getGlobalContext()), makeArrayRef(argTypes), false);
    // change GlobalValue::InternalLinkage into ExternalLinkage
    mainFunction = llvm::Function::Create(ftype, llvm::GlobalValue::ExternalLinkage, "main", module);
    llvm::BasicBlock *bblock = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "entry", mainFunction, 0);

    CodeGenContext::printf = getPrintfPrototype();

    /* Push a new variable/block context */
    pushBlock(bblock);
    currentFunction = mainFunction;
    for (auto label : labels)
    {
        labelBlock[label] = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "label", mainFunction, 0);
    }
    root.code_gen(*this); /* emit bytecode for the toplevel block */
    llvm::ReturnInst::Create(GlobalLLVMContext::getGlobalContext(), currentBlock());
    popBlock();

    /* Print the bytecode in a human-readable format 
	   to see if our program compiled properly
	 */
    std::cout << "Code is generated." << std::endl;
    llvm::legacy::PassManager pm;
    pm.add(llvm::createPrintModulePass(llvm::outs()));
    //pm.run(*module);

    // write IR to stderr
    std::cout << "code is gen~~~" << std::endl;
    module->dump();
    std::cout << "code is gen~!~" << std::endl;
}

/* Executes the AST by running the main function */
llvm::GenericValue CodeGenContext::runCode()
{
    // std::cout << "Running begining...\n";
    // std::cout << "========================================" << std::endl;
    // llvm::ExecutionEngine *ee = llvm::EngineBuilder(module).create();
    // std::vector<llvm::GenericValue> noargs;
    // llvm::GenericValue v = ee->runFunction(mainFunction, noargs);
    // std::cout << "========================================" << std::endl;
    // std::cout << "Running end.\n";
    // return v;
}
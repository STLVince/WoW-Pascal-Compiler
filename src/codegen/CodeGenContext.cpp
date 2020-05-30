#include <utility>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Support/TargetRegistry.h>
#include <llvm/Transforms/IPO.h>
#include <llvm/Transforms/Scalar.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Utils.h>

#include "CodeGenContext.h"

CodeGenContext::CodeGenContext() : Builder(GlobalLLVMContext::getGlobalContext())
{
    module = new llvm::Module("Pascal", GlobalLLVMContext::getGlobalContext());
    fpm = std::make_unique<llvm::legacy::FunctionPassManager>(module);

    // createPromoteMemoryToRegister - Provide an entry point to create this pass.
    fpm->add(llvm::createPromoteMemoryToRegisterPass());

    // Do simple "peephole" optimizations and bit-twiddling optzns.
    fpm->add(llvm::createInstructionCombiningPass());

    // Reassociate expressions.
    fpm->add(llvm::createReassociatePass());

    // Eliminate Common SubExpressions.
    fpm->add(llvm::createGVNPass());

    // Simplify the control flow graph (deleting unreachable blocks, etc).
    fpm->add(llvm::createCFGSimplificationPass());
    fpm->doInitialization();

    mpm = std::make_unique<llvm::legacy::PassManager>();
    // createConstantMergePass - This function returns a new pass that merges
    // duplicate global constants together into a single constant that is shared.
    // This is useful because some passes (ie TraceValues) insert a lot of string
    // constants into the program, regardless of whether or not they duplicate an
    // existing string.
    mpm->add(llvm::createConstantMergePass());

    // createFunctionInliningPass - Return a new pass object that uses a heuristic
    // to inline direct function calls to small functions.
    mpm->add(llvm::createFunctionInliningPass());
}

std::map<std::string, llvm::Value *> &CodeGenContext::locals()
{
    return blocks.top()->locals;
}

llvm::Value *CodeGenContext::getValue(std::string name)
{
    llvm::Function *nowFunction = currentFunction;

    codegenOutput << "Start getValue for " << name << std::endl;
    codegenOutput << "found:" << currentFunction->getValueSymbolTable()->lookup(name) << std::endl;
    codegenOutput << "main:" << mainFunction << std::endl;
    codegenOutput << "current:" << currentFunction << std::endl;
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

// llvm::BasicBlock *CodeGenContext::currentBlock()
// {
//     return blocks.top()->block;
// };

// void CodeGenContext::pushBlock(llvm::BasicBlock *block)
// {
//     CodeGenBlock *newblock = new CodeGenBlock();
//     blocks.push(newblock);
//     blocks.top()->returnValue = nullptr;
//     blocks.top()->block = block;
// }

// void CodeGenContext::popBlock()
// {
//     CodeGenBlock *top = blocks.top();
//     blocks.pop();
//     delete top;
// }

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

void CodeGenContext::generateCode(ast::Program &root)
{

    std::cout << "Generating code...\n";

    // Create the top level interpreter function to call as entry
    std::vector<llvm::Type *> argTypes;
    llvm::FunctionType *ftype = llvm::FunctionType::get(llvm::Type::getVoidTy(GlobalLLVMContext::getGlobalContext()), makeArrayRef(argTypes), false);

    mainFunction = llvm::Function::Create(ftype, llvm::GlobalValue::ExternalLinkage, "main", module);
    llvm::BasicBlock *basicBlock = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "entry", mainFunction, 0);

    CodeGenContext::printf = getPrintfPrototype();

    // Push a new variable/block context
    // pushBlock(basicBlock);
    Builder.SetInsertPoint(basicBlock);
    currentFunction = mainFunction;
    for (auto label : labels)
    {
        labelBlock[label] = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "label", mainFunction, 0);
    }
    root.code_gen(*this);
    // llvm::ReturnInst::Create(GlobalLLVMContext::getGlobalContext(), currentBlock());
    // popBlock();

    // verify the main function
    llvm::verifyFunction(*mainFunction, &llvm::errs());

    // perform optimization
    fpm->run(*mainFunction);
    mpm->run(*module);

    // Print the bytecode in a human-readable format
    // to see if the program compiled properly
    llvm::legacy::PassManager pm;
    pm.add(llvm::createPrintModulePass(llvm::outs()));
    //pm.run(*module);

    // write IR to stderr
    module->dump();
    std::cout << "Code is generated!" << std::endl;
}

void CodeGenContext::outputCode(std::string filename)
{
    std::error_code ec;
    llvm::raw_fd_ostream dest(filename, ec, llvm::sys::fs::F_None);
    if (ec)
    {
        llvm::errs() << "Could not open file: " << ec.message();
        exit(1);
    }

    int pos = filename.rfind('/');
    std::string ext = filename.substr(pos + 1);
    llvm::TargetMachine::CodeGenFileType type;
    if (ext == "ll")
    {
        module->print(dest, nullptr);
    }
    else if (ext == "s")
    {
        type = llvm::TargetMachine::CGFT_AssemblyFile;
    }
    else if (ext == "o")
    {
        type = llvm::TargetMachine::CGFT_ObjectFile;
    }

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    auto target_triple = llvm::sys::getDefaultTargetTriple();
    module->setTargetTriple(target_triple);

    std::string error;
    auto target = llvm::TargetRegistry::lookupTarget(target_triple, error);
    if (!target)
    {
        llvm::errs() << error;
        return;
    }

    auto cpu = "generic";
    auto features = "";
    llvm::TargetOptions opt;
    auto rm = llvm::Optional<llvm::Reloc::Model>();
    auto target_machine = target->createTargetMachine(target_triple, cpu, features, opt, rm);
    module->setDataLayout(target_machine->createDataLayout());

    llvm::legacy::PassManager pass;
    if (target_machine->addPassesToEmitFile(pass, dest, nullptr, type))
    {
        llvm::errs() << "The target machine cannot emit an object file";
        exit(1);
    }

    llvm::verifyModule(*module, &llvm::errs());
    pass.run(*module);

    dest.flush();
}
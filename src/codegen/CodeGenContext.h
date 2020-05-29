#ifndef __CODEGENCONTEXT_H__
#define __CODEGENCONTEXT_H__

#include <map>
#include <stack>
#include <vector>
#include <iostream>
#include <llvm/ADT/STLExtras.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/CallingConv.h>
#include <llvm/IR/IRPrintingPasses.h>
#include <llvm/IR/IRBuilder.h>
#include "llvm/IR/Verifier.h"
#include <llvm/Bitcode/BitcodeReader.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/ValueSymbolTable.h>

#include "../utils/ast.hpp"

class GlobalLLVMContext
{
public:
    static llvm::LLVMContext &getGlobalContext()
    {
        static llvm::LLVMContext instance;
        return instance;
    }

private:
    GlobalLLVMContext(){};

public:
    GlobalLLVMContext(GlobalLLVMContext const &) = delete;
    void operator=(GlobalLLVMContext const &) = delete;
};

class CodeGenBlock
{
public:
    llvm::BasicBlock *block;
    llvm::Value *returnValue;
    CodeGenBlock *parent;
    std::map<std::string, llvm::Value *> locals;
};

class CodeGenContext
{
private:
    std::stack<CodeGenBlock *> blocks;

public:
    llvm::Module *module;
    llvm::Function *currentFunction;
    llvm::Function *mainFunction;
    llvm::IRBuilder<> Builder;    
    llvm::Function *printf;
    std::map<llvm::Function *, llvm::Function *> functionParent;    
    std::map<std::string, llvm::Type *> aliases;
    // std::vector<std::string> traces;
    static std::vector<int> labels;
    llvm::BasicBlock* labelBlock[10000];
    // bool is_subroutine = false;
    std::unique_ptr<llvm::legacy::FunctionPassManager> fpm;
    std::unique_ptr<llvm::legacy::PassManager> mpm;

    CodeGenContext();
    std::map<std::string, llvm::Value *> &locals();
    llvm::Value *getValue(std::string name);
    void putValue(std::string name, llvm::Value *value);
    llvm::Type *getAlias(std::string key);
    bool setAlias(std::string key, llvm::Type *value);
    llvm::BasicBlock *currentBlock();
    void pushBlock(llvm::BasicBlock *block);
    void popBlock();
    llvm::Function *getPrintfPrototype();
    
    void generateCode(ast::Program& root);
    void outputCode(std::string filename);
};

#endif
#include "../AST/identifier.hpp"

namespace ast
{
    void Identifier::printSelf(std::string nodeName) 
    {
    }

    llvm::Value *Identifier::GetPtr(CodeGenContext &context)
    {
        // first find in local context
        codegenOutput << "Identifier::GetPtr: finding " << name << std::endl;
        auto *value = context.getValue(name);
        if (!value)
        {
            value = context.module->getGlobalVariable(name);
        }
        if (!value)
        {
            std::cerr << "Identifier" << name << "not found" << std::endl;
        }
        return value;
    }

    llvm::Value *Identifier::code_gen(CodeGenContext &context)
    {
        codegenOutput << "Identifier::code_gen: gen_code for " << name << std::endl;
        return context.Builder.CreateLoad(GetPtr(context));
    }
} // namespace ast

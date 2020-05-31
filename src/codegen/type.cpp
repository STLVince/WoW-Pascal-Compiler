#include "../AST/type.hpp"
namespace ast
{
    llvm::Value *TypeDecl::code_gen(CodeGenContext &context)
    {
        return nullptr;
    }

    llvm::Type *TypeDecl::getType()
    {
        switch (this->type)
        {
        case TypeName::INTEGER:
            return llvm::Type::getInt32Ty(GlobalLLVMContext::getGlobalContext());
            break;
        case TypeName::REAL:
            return llvm::Type::getDoubleTy(GlobalLLVMContext::getGlobalContext());
            break;
        case TypeName::CHARACTER:
            return llvm::Type::getInt8Ty(GlobalLLVMContext::getGlobalContext());
            break;
        case TypeName::BOOLEAN:
            return llvm::Type::getInt1Ty(GlobalLLVMContext::getGlobalContext());
            break;
        case TypeName::RANGE:
            return llvm::Type::getInt32Ty(GlobalLLVMContext::getGlobalContext());
            break;
        default:
            return llvm::Type::getVoidTy(GlobalLLVMContext::getGlobalContext());
            break;
        }
    }

    llvm::Type *ConstType::getType(CodeGenContext &context)
    {
        switch (getConstType())
        {
        case TypeName::INTEGER:
            return context.Builder.getInt32Ty();
        case TypeName::REAL:
            return context.Builder.getDoubleTy();
        case TypeName::BOOLEAN:
            return context.Builder.getInt1Ty();
        default:
            std::cerr << "Unsupported type3" << std::endl;
        }
        return nullptr;
    }

    // llvm::Value *ArrayType::code_gen(CodeGenContext &context)
    // {

    // }

    // llvm::Value *RecordType::code_gen(CodeGenContext &context)
    // {

    // }

    // llvm::Value *ConstType::code_gen(CodeGenContext &context)
    // {

    // }

    llvm::Value *IntegerType::code_gen(CodeGenContext &context)
    {
        auto *type = context.Builder.getInt32Ty();
        return llvm::ConstantInt::getSigned(type, val);
    }

    llvm::Value *RealType::code_gen(CodeGenContext &context)
    {
        auto *type = context.Builder.getDoubleTy();
        return llvm::ConstantFP::get(type, val);
    }

    llvm::Value *BooleanType::code_gen(CodeGenContext &context)
    {
        return val ? context.Builder.getTrue() : context.Builder.getFalse();
    }

    llvm::Value *StringType::code_gen(CodeGenContext &context)
    {
        llvm::Module *M = context.module;
        llvm::LLVMContext &ctx = M->getContext();
        llvm::Constant *strConstant = llvm::ConstantDataArray::getString(ctx, val);
        llvm::Type *t = strConstant->getType();
        llvm::GlobalVariable *GVStr = new llvm::GlobalVariable(*M, t, true, llvm::GlobalValue::ExternalLinkage, strConstant, "");
        llvm::Constant *zero = llvm::Constant::getNullValue(llvm::IntegerType::getInt32Ty(ctx));
        llvm::Constant *indices[] = {zero, zero};

        llvm::Constant *strVal = llvm::ConstantExpr::getGetElementPtr(t, GVStr, indices[0], true);

        return strVal;
    }
} // namespace ast

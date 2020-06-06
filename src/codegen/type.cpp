#include "../AST/type.hpp"
namespace ast
{
    llvm::Value *TypeDecl::code_gen(CodeGenContext &context)
    {
        return nullptr;
    }

    llvm::Type *TypeDecl::getType(CodeGenContext &context)
    {
        codegenOutput << "TypeDecl::getType" << std::endl;
        
        if (auto* real_type = dynamic_cast<const ArrayType *>(this))
        {
            codegenOutput << "TypeDecl::getType: array type" << std::endl;
            llvm::ArrayType *int_3 = llvm::ArrayType::get(real_type->array_type->getType(context), real_type->end + 1);
            std::vector<llvm::Constant *> InitVector;
            codegenOutput << "TypeDecl::getType: here3" << std::endl;
            llvm::Constant* variable = llvm::ConstantArray::get(int_3, InitVector);
            codegenOutput << "TypeDecl::getType: here4" << std::endl;
            return variable->getType();
        }
        
        codegenOutput << "TypeDecl::getType: before switch type" << std::endl;
        switch (this->type)
        {
        case TypeName::INTEGER:
            return context.Builder.getInt32Ty();
            break;
        case TypeName::REAL:
            return context.Builder.getDoubleTy();
            break;
        case TypeName::CHARACTER:
            return context.Builder.getInt8Ty();
            break;
        case TypeName::STRING:
            return llvm::ConstantDataArray::getString(GlobalLLVMContext::getGlobalContext(), "", true)->getType();
        case TypeName::BOOLEAN:
            return context.Builder.getInt1Ty();
            break;
        // case TypeName::ARRAY:
            // real_type = std::dynamic_pointer_cast<ArrayType>(std::shared_ptr<TypeDecl>(this));
            // codegenOutput << "TypeDecl::getType: array type" << std::endl;
            
            // break;
        // case TypeName::RECORD:
        // TODO
        default:
            return llvm::Type::getVoidTy(GlobalLLVMContext::getGlobalContext());
            break;
        }
    }

    llvm::Type *ConstType::getType(CodeGenContext &context)
    {
        codegenOutput << "ConstType::getType" << std::endl;
        
        switch (getConstType())
        {
        case TypeName::INTEGER:
            return context.Builder.getInt32Ty();
        case TypeName::REAL:
            return context.Builder.getDoubleTy();
        case TypeName::CHARACTER:
            return context.Builder.getInt8Ty();
        case TypeName::STRING:
            return llvm::ConstantDataArray::getString(GlobalLLVMContext::getGlobalContext(), "", true)->getType();
        case TypeName::BOOLEAN:
            return context.Builder.getInt1Ty();
        // case TypeName::ARRAY:
        // TODO
        // case TypeName::RECORD:
        // TODO
        default:
            std::cerr << "Unsupported type3" << std::endl;
        }
        return nullptr;
    }

    llvm::Value *ArrayType::code_gen(CodeGenContext &context)
    {
        codegenOutput << "ArrayType::code_gen: inside ArrayType" << std::endl;

        llvm::Constant *array_element;
        switch (array_type->type)
        {
        case TypeName::REAL:
            array_element = llvm::ConstantFP::get(llvm::Type::getDoubleTy(GlobalLLVMContext::getGlobalContext()), 0);
            break;
        case TypeName::INTEGER:
        default:
            array_element = llvm::ConstantInt::get(llvm::Type::getInt32Ty(GlobalLLVMContext::getGlobalContext()), 0, true);
            break;
        }

        auto array_content = std::vector<llvm::Constant *>();
        for (int i = 0; i < end - start; i++)
        {
            array_content.push_back(array_element);
        }
        auto arr_type = (llvm::ArrayType *)array_type->getType(context);
        auto arr_const = llvm::ConstantArray::get(arr_type, array_content);

        return new llvm::GlobalVariable(*context.module, array_type->getType(context), false, llvm::GlobalValue::ExternalLinkage, arr_const);
    }

    // llvm::Value *RecordType::code_gen(CodeGenContext &context)
    // {

    // }

    // llvm::Value *ConstType::code_gen(CodeGenContext &context)
    // {

    // }

    llvm::Value *IntegerType::code_gen(CodeGenContext &context)
    {
        codegenOutput << "IntegerType::code_gen: inside IntegerType, val = " << val << std::endl;
        auto *type = context.Builder.getInt32Ty();
        return llvm::ConstantInt::getSigned(type, val);
    }

    llvm::Value *RealType::code_gen(CodeGenContext &context)
    {
        codegenOutput << "RealType::code_gen: inside RealType, val = " << val << std::endl;
        auto *type = context.Builder.getDoubleTy();
        return llvm::ConstantFP::get(type, val);
    }

    llvm::Value *BooleanType::code_gen(CodeGenContext &context)
    {
        codegenOutput << "BooleanType::code_gen: inside BooleanType, val = " << val << std::endl;
        return val ? context.Builder.getTrue() : context.Builder.getFalse();
    }

    llvm::Value *StringType::code_gen(CodeGenContext &context)
    {
        codegenOutput << "StringType::code_gen: inside StringType, val = " << val << std::endl;
        llvm::LLVMContext &ctx = context.module->getContext();
        llvm::Constant *strConstant = llvm::ConstantDataArray::getString(ctx, val);
        llvm::Type *string_type = strConstant->getType();
        llvm::GlobalVariable *GVStr = new llvm::GlobalVariable(*context.module, string_type, true, llvm::GlobalValue::ExternalLinkage, strConstant, "");
        llvm::Constant *zero = llvm::Constant::getNullValue(llvm::IntegerType::getInt32Ty(ctx));
        llvm::Constant *indices[] = {zero, zero};

        llvm::Constant *strVal = llvm::ConstantExpr::getGetElementPtr(string_type, GVStr, indices[0], true);
        codegenOutput << "StringType::code_gen: StringType done" << std::endl;
        return strVal;
    }

    llvm::Value *CharType::code_gen(CodeGenContext &context)
    {
        codegenOutput << "CharType::code_gen: inside CharType, val = " << val << std::endl;
        auto *type = context.Builder.getInt8Ty();
        return llvm::ConstantInt::getSigned(type, val);
    }
} // namespace ast

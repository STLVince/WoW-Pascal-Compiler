#include "../AST/type.hpp"
namespace ast
{
    llvm::Value *TypeDecl::code_gen(CodeGenContext &context)
    {
        return nullptr;
    }

    llvm::Type *TypeDecl::getType(CodeGenContext &context)
    {
        std::shared_ptr<ArrayType> real_type;
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
        case TypeName::ARRAY:
            real_type = std::dynamic_pointer_cast<ArrayType>(std::shared_ptr<TypeDecl>(this));
            return llvm::ArrayType::get(real_type->array_type->getType(context), real_type->end - real_type->start);
            break;
        // case TypeName::RECORD:
        // TODO
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

        //0. Defs
        // auto str = this->val;
        // auto charType = llvm::IntegerType::get(GlobalLLVMContext::getGlobalContext(), 8);

        // //1. Initialize chars vector
        // std::vector<llvm::Constant *> chars(str.length());
        // for (unsigned int i = 0; i < str.size(); i++)
        // {
        //     chars[i] = llvm::ConstantInt::get(charType, str[i]);
        // }

        // //1b. add a zero terminator too
        // chars.push_back(llvm::ConstantInt::get(charType, 0));

        // //2. Initialize the string from the characters
        // auto stringType = llvm::ArrayType::get(charType, chars.size());

        // //3. Create the declaration statement
        // auto globalDeclaration = (llvm::GlobalVariable *)context.module->getOrInsertGlobal(".str", stringType);
        // globalDeclaration->setInitializer(llvm::ConstantArray::get(stringType, chars));
        // globalDeclaration->setConstant(true);
        // globalDeclaration->setLinkage(llvm::GlobalValue::LinkageTypes::PrivateLinkage);
        // globalDeclaration->setUnnamedAddr(llvm::GlobalValue::UnnamedAddr::Global);
        // // llvm::GlobalVariable * v = new llvm::GlobalVariable(context.module, stringType, true, llvm::GlobalVariable::ExternalLinkage, llvm::ConstantArray::get(stringType, chars));

        // //4. Return a cast to an i8*
        // return llvm::ConstantExpr::getBitCast(globalDeclaration, charType->getPointerTo());
        // auto charType = llvm::IntegerType::get(GlobalLLVMContext::getGlobalContext(), 8);
        // std::vector<llvm::Constant *> chars(val.size());
        // for (unsigned int i = 0; i < val.size(); i++)
        // {
        //     chars[i] = llvm::ConstantInt::get(charType, val[i]);
        // }
        // chars.push_back(llvm::ConstantInt::get(charType, 0));
        // auto init = llvm::ConstantArray::get(llvm::ArrayType::get(charType, chars.size()), chars);
        // llvm::GlobalVariable *v = new llvm::GlobalVariable(*context.module, init->getType(), true, llvm::GlobalVariable::ExternalLinkage, init, val);
        // return llvm::ConstantExpr::getBitCast(v, charType->getPointerTo());
    }

    llvm::Value *CharType::code_gen(CodeGenContext &context)
    {
        codegenOutput << "CharType::code_gen: inside CharType, val = " << val << std::endl;
        auto *type = context.Builder.getInt8Ty();
        return llvm::ConstantInt::getSigned(type, val);
    }
} // namespace ast

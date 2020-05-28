#include "../AST/declation.hpp"

namespace ast
{
    llvm::Value *TypeDef::code_gen(CodeGenContext &context)
    {
        auto success = context.setAlias(name->name, type->getType());
        if (!success)
        {
            // throw CodegenException("duplicate type alias");
            std::cerr << "duplicate type alias" << std::endl;
        }
        return nullptr;
    }

    llvm::Value *ConstDecl::code_gen(CodeGenContext &context)
    {        
        auto IfString = std::dynamic_pointer_cast<StringType>(val);
        if (IfString)
        {
            auto *constant = llvm::ConstantDataArray::getString(GlobalLLVMContext::getGlobalContext(), IfString->val, true);
            llvm::Value *result = new llvm::GlobalVariable(*context.module, constant->getType(), true, llvm::GlobalVariable::ExternalLinkage, constant, name->name);
            return result;
        }
        auto *constant = llvm::cast<llvm::Constant>(val->code_gen(context));
        return new llvm::GlobalVariable(*context.module, val->getType(context), true, llvm::GlobalVariable::ExternalLinkage, constant, name->name);        
    }

    llvm::Value *VarDecl::code_gen(CodeGenContext &context)
    {
        llvm::Type *type = this->type->getType();
        llvm::Constant *constant;

        // TODO: new type support
        switch (this->type->type)
        { 
        case TypeName::INTEGER:
            constant = llvm::ConstantInt::get(type, 0);
            break;
        case TypeName::REAL:
            constant = llvm::ConstantFP::get(type, 0.0);
            break;
        case TypeName::BOOLEAN:
            constant = llvm::ConstantInt::get(type, 0);
            break;
        case TypeName::STRING:
            constant = llvm::ConstantDataArray::getString(GlobalLLVMContext::getGlobalContext(), "", true);
            break;
        // case TypeName::ARRAY:
        // {
        //     auto array_it = std::dynamic_pointer_cast<ArrayType>(this->type);
        //     auto *array_type = array_it->ItemType->GetType(context);
        //     //llvm::IntegerType* i32 = llvm::IntegerType::get(context.GetModule()->getContext(), 32);
        //     llvm::Constant *constant;
        //     if (array_type->isIntegerTy(32))
        //     {
        //         constant = llvm::ConstantInt::get(array_type, 0);
        //     }
        //     else if (array_type->isIntegerTy(1))
        //     {
        //         constant = llvm::ConstantInt::get(array_type, 0);
        //     }
        //     else if (array_type->isDoubleTy())
        //     {
        //         constant = llvm::ConstantFP::get(type, 0.0);
        //     }
        //     else
        //     {
        //         std::cerr << "element type for array not supported" << std::endl;
        //     }
        //     llvm::Constant *one = llvm::ConstantInt::get(array_type, 0);

        //     int number_value = 0;
        //     if (llvm::ConstantInt *CI = llvm::dyn_cast<llvm::ConstantInt>(array_it->EndValue->codegen(context)))
        //     {
        //         if (CI->getBitWidth() <= 32)
        //         {
        //             number_value = CI->getSExtValue() + 1; // add 1 since the index starts from 0
        //         }
        //     }
        //     else
        //     {
        //         // throw CodegenException("Initial array index not valid");
        //         std::cerr << "Initial array index not valid" << std::endl;
        //     }
        //     llvm::ArrayType *int_3 = llvm::ArrayType::get(array_type, number_value);
        //     std::vector<llvm::Constant *> InitVector;
        //     for (int i = 0; i < number_value; i++)
        //     {
        //         InitVector.push_back(one);
        //     }
        //     auto *variable = llvm::ConstantArray::get(int_3, InitVector);
        //     return new llvm::GlobalVariable(*context.module, variable->getType(), false, llvm::GlobalVariable::ExternalLinkage, variable, name->name);

        //     //auto * variable = llvm::ArrayType::get(context.Builder.getInt32Ty(), 5);

        //     //return new llvm::GlobalVariable(*context.GetModule(), constant->getType(), false, llvm::GlobalVariable::ExternalLinkage, constant, name->GetName());
        // }
        default:
            // throw CodegenException("unsupported type2");  // TODO String support
            std::cerr << "unsupported type2" << std::endl;
        }
        return new llvm::GlobalVariable(*context.module, constant->getType(), false, llvm::GlobalVariable::ExternalLinkage, constant, name->name);
    };    
} // namespace ast
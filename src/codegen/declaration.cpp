#include "AST/declation.hpp"
#include <iostream>

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
        if (context.is_subroutine)
        { // not main function
            auto IfString = std::dynamic_pointer_cast<ast::StringType>(val);
            if (IfString)
            {
                std::string prefix(std::move(context.traces.back()));
                std::cout << prefix << std::endl;
                context.traces.push_back(prefix);
                return context.Builder.CreateGlobalStringPtr(IfString->val, prefix + "_" + this->name->name);
            }
            auto *constant = llvm::cast<llvm::Constant>(val->code_gen(context));
            std::string prefix(std::move(context.traces.back()));
            std::cout << prefix << std::endl;
            context.traces.push_back(prefix);
            return new llvm::GlobalVariable(*context.module, val->getType(context), true, llvm::GlobalVariable::ExternalLinkage, constant, prefix + "_" + name->name);
        }
        else
        { // main function
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
    }

    llvm::Value *VarDecl::code_gen(CodeGenContext &context)
    {
        if (context.is_subroutine)
        {
            llvm::Type *type = this->type->getType();
            llvm::Constant *constant;

            std::string prefix(std::move(context.traces.back()));
            std::cout << prefix << std::endl;
            context.traces.push_back(prefix);
            switch (this->type->type)
            { // TODO: new type support
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
            //case Type::ARRAY: {llvm::ArrayRef<uint32_t> a(std::vector<uint32_t>(5, 2)); constant = llvm::ConstantDataArray::get(llvm_context, a); break;}
            // case TypeName::ARRAY:
            // {
            //     llvm::IntegerType *i32 = llvm::IntegerType::get(context.module->getContext(), 32);
            //     llvm::Constant *one = llvm::ConstantInt::get(i32, 1);
            //     llvm::ArrayType *int_3 = llvm::ArrayType::get(i32, 3);
            //     auto *variable = llvm::ConstantArray::get(int_3, std::vector<llvm::Constant *>{one, one, one});
            //     return new llvm::GlobalVariable(*context.module, variable->getType(), false, llvm::GlobalVariable::ExternalLinkage, variable, prefix + "_" + name->name);
            //     //return new llvm::GlobalVariable(*context.GetModule(), variable->getPointerElementType(), false, llvm::GlobalVariable::ExternalLinkage, variable, prefix + "_" + name->GetName());
            // }
                // default: throw CodegenException("unsupported type1");  // TODO String support
                std::cerr << "unsupported type1" << std::endl;
            }
            return new llvm::GlobalVariable(*context.module, constant->getType(), false, llvm::GlobalVariable::ExternalLinkage, constant, prefix + "_" + name->name);
        }
        else
        { // main function
            llvm::Type *type = this->type->getType();
            llvm::Constant *constant;

            switch (this->type->type)
            { // TODO: new type support            
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
    }
} // namespace ast
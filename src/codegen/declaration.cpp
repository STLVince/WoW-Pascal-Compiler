#include "../AST/declation.hpp"
namespace ast
{
    ConstDecl::ConstDecl(std::shared_ptr<Identifier> name, std::shared_ptr<ConstType> val) : name(name), type(new TypeDecl(val->getConstType())), val(val) {}

    void TypeDef::printSelf(std::string nodeName)
    {
        astDot << nodeName << "->" << nodeName + "_TypeDecl_" << static_cast<std::underlying_type<TypeName>::type>(type->type) << std::endl;
    }

    void ConstDecl::printSelf(std::string nodeName)
    {
        astDot << nodeName << "->" << nodeName + "_TypeDecl_" << static_cast<std::underlying_type<TypeName>::type>(type->type) << "_" << static_cast<std::underlying_type<TypeName>::type>(val->type) << std::endl;
    }

    void VarDecl::printSelf(std::string nodeName)
    {
        astDot << nodeName << "->" << nodeName + "_TypeDecl_" << static_cast<std::underlying_type<TypeName>::type>(type->type);
        astDot << "_" << (is_global ? "global" : "not_global") << std::endl;
    }

    llvm::Value *TypeDef::code_gen(CodeGenContext &context)
    {
        codegenOutput << "TypeDef::code_gen: inside TypeDef ast" << std::endl;
        auto success = context.setAlias(name->name, type->getType(context));
        if (!success)
        {
            std::cerr << "duplicate type alias" << std::endl;
        }
        return nullptr;
    }

    llvm::Value *ConstDecl::code_gen(CodeGenContext &context)
    {
        codegenOutput << "ConstDecl::code_gen: inside ConstDecl ast" << std::endl;
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
        codegenOutput << "VarDecl::code_gen: VarDecl for " << name->name << std::endl;

        if (is_global)
        {
            codegenOutput << "VarDecl::code_gen: var is global" << std::endl;
            
            llvm::Type *type = this->type->getType(context);
            llvm::Constant *constant;

             codegenOutput << "VarDecl::code_gen: finish getType" << std::endl;

            // TODO: new type support
            switch (this->type->type)
            {
            case TypeName::INTEGER:
            case TypeName::BOOLEAN:
            case TypeName::CHARACTER:
                constant = llvm::ConstantInt::get(type, 0);
                break;
            case TypeName::REAL:
                constant = llvm::ConstantFP::get(type, 0.0);
                break;
            case TypeName::STRING:
                codegenOutput << "VarDecl::code_gen: string type" << std::endl;
                // constant = llvm::ConstantArray::get(stringType, chars);
                constant = llvm::ConstantDataArray::getString(GlobalLLVMContext::getGlobalContext(), "", true);
                break;
            case TypeName::ARRAY:
            {
                codegenOutput << "VarDecl::code_gen: array type" << std::endl;
                auto array_it = std::dynamic_pointer_cast<ArrayType>(this->type);
                auto *array_type = array_it->array_type->getType(context);
                //llvm::IntegerType* i32 = llvm::IntegerType::get(context.GetModule()->getContext(), 32);
                llvm::Constant *constant;
                if (array_type->isIntegerTy(32))
                {
                    constant = llvm::ConstantInt::get(array_type, 0);
                }
                else if (array_type->isIntegerTy(1))
                {
                    constant = llvm::ConstantInt::get(array_type, 0);
                }
                else if (array_type->isDoubleTy())
                {
                    constant = llvm::ConstantFP::get(type, 0.0);
                }
                else
                {
                    std::cerr << "VarDecl::code_gen: element type for array not supported" << std::endl;
                }
                llvm::Constant *one = llvm::ConstantInt::get(array_type, 0);

                int number_value = 0;
                if (llvm::ConstantInt *CI = llvm::dyn_cast<llvm::ConstantInt>(llvm::ConstantInt::get(array_type, array_it->end)))
                {
                    if (CI->getBitWidth() <= 32)
                    {
                        number_value = CI->getSExtValue() + 1; // add 1 since the index starts from 0
                    }
                }
                else
                {
                    std::cerr << "VarDecl::code_gen: Initial array index not valid" << std::endl;
                }
                llvm::ArrayType *int_3 = llvm::ArrayType::get(array_type, number_value);
                std::vector<llvm::Constant *> InitVector;
                for (int i = 0; i < number_value; i++)
                {
                    InitVector.push_back(one);
                }
                auto *variable = llvm::ConstantArray::get(int_3, InitVector);
                codegenOutput << "VarDecl::code_gen: array type done" << std::endl;
                return new llvm::GlobalVariable(*context.module, variable->getType(), false, llvm::GlobalVariable::ExternalLinkage, variable, name->name);
            }
            default:
                std::cerr << "VarDecl::code_gen: unsupported type2" << std::endl;
            }
            codegenOutput << "VarDecl::code_gen: finish type ref" << std::endl;
            return new llvm::GlobalVariable(*context.module, type, false, llvm::GlobalVariable::ExternalLinkage, constant, name->name);
        }
        else
        {
            return context.Builder.CreateAlloca(this->type->getType(context), 0, nullptr, name->name);
        }
    };
} // namespace ast
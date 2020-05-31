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
        auto success = context.setAlias(name->name, type->getType());
        if (!success)
        {
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
        // }
        default:
            std::cerr << "unsupported type2" << std::endl;
        }
        return new llvm::GlobalVariable(*context.module, constant->getType(), false, llvm::GlobalVariable::ExternalLinkage, constant, name->name);
    };
} // namespace ast
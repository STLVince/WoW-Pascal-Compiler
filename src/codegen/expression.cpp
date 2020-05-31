#include "../AST/expression.hpp"
#include "../AST/identifier.hpp"

namespace ast
{
    void FuncCall::printSelf(std::string nodeName)
    {
        for (auto arg : *(this->arg_list))
        {
            std::string childName = nodeName + "_VarDecl";
            astDot << nodeName << "->" << childName << std::endl;
            arg->printSelf(childName);
        }
    }

    void ProcCall::printSelf(std::string nodeName)
    {
        for (auto arg : *(this->arg_list))
        {
            std::string childName = nodeName + "_VarDecl";
            astDot << nodeName << "->" << childName << std::endl;
            arg->printSelf(childName);
        }
    }

    void SysFuncCall::printSelf(std::string nodeName)
    {
        for (auto arg : *(this->arg_list))
        {
            std::string childName = nodeName + "_VarDecl";
            astDot << nodeName << "->" << childName << std::endl;
            arg->printSelf(childName);
        }
    }

    void SysProcCall::printSelf(std::string nodeName)
    {
        for (auto arg : *(this->arg_list))
        {
            std::string childName = nodeName + "_VarDecl";
            astDot << nodeName << "->" << childName << std::endl;
            arg->printSelf(childName);
        }
    }

    void BinaryOp::printSelf(std::string nodeName)
    {
    }

    llvm::Value *FuncCall::code_gen(CodeGenContext &context)
    {
        codegenOutput << "FuncCall::code_gen: inside function call" << std::endl;

        // look up the function name in the module
        auto *func = context.module->getFunction(id->name);

        // check whether arguments match
        if (func->arg_size() != (*arg_list).size())
        {
            std::cerr << "FuncCall::code_gen: number of arguments not match for function " + id->name << std::endl;
        }

        // build argument
        std::vector<llvm::Value *> values;
        for (auto &arg : (*arg_list))
        {
            values.push_back(arg->code_gen(context));
        }
        return context.Builder.CreateCall(func, values);
    }

    llvm::Value *ProcCall::code_gen(CodeGenContext &context)
    {
        codegenOutput << "ProcCall::code_gen: inside procedure call" << std::endl;

        // look up the procedure name in the module
        auto *func = context.module->getFunction(id->name);

        // check whether arguments match
        if (func->arg_size() != (*arg_list).size())
        {
            std::cerr << "ProcCall::code_gen: number of arguments not match for procedure " + id->name << std::endl;
        }

        // build argument
        std::vector<llvm::Value *> values;
        for (auto &arg : (*arg_list))
        {
            values.push_back(arg->code_gen(context));
        }
        return context.Builder.CreateCall(func, values);
    }

    llvm::Value *SysFuncCall::code_gen(CodeGenContext &context)
    {
        codegenOutput << "SysFuncCall::code_gen: inside sysfunc" << std::endl;
        if (id->name == "WRITE" || id->name == "WRITELN")
        {
            for (auto &arg : *(this->arg_list))
            {
                auto *value = arg->code_gen(context);
                auto x = value->getType();
                std::vector<llvm::Value *> func_args;
                if (value->getType()->isIntegerTy())
                {
                    func_args.push_back(context.Builder.CreateGlobalStringPtr("%d"));
                    func_args.push_back(value);
                }
                else if (value->getType()->isDoubleTy())
                {
                    func_args.push_back(context.Builder.CreateGlobalStringPtr("%f"));
                    func_args.push_back(value);
                }
                // TODO: string support
                else
                {
                    std::cerr << "incompatible type for sysfunc call" << std::endl;
                }
                context.Builder.CreateCall(context.getPrintfPrototype(), func_args);
            }
            return nullptr;
        }
        return nullptr;
    }

    llvm::Value *SysProcCall::code_gen(CodeGenContext &context)
    {
        return nullptr;
    }

    llvm::Value *BinaryOp::code_gen(CodeGenContext &context)
    {
        codegenOutput << "BinaryOp::code_gen: generating binary expression" << std::endl;
        auto *op1 = this->op1->code_gen(context);
        auto *op2 = this->op2->code_gen(context);

        // compare operator mapping
        std::map<OpType, llvm::CmpInst::Predicate> BTypeAlt = {
            {OpType::EQ, llvm::CmpInst::ICMP_EQ},
            {OpType::NE, llvm::CmpInst::ICMP_NE},
            {OpType::LT, llvm::CmpInst::ICMP_SLT},
            {OpType::GT, llvm::CmpInst::ICMP_SGT},
            {OpType::LE, llvm::CmpInst::ICMP_SLE},
            {OpType::GE, llvm::CmpInst::ICMP_SGE}};
        std::map<OpType, llvm::CmpInst::Predicate> FTypeAlt = {
            {OpType::EQ, llvm::CmpInst::FCMP_OEQ},
            {OpType::NE, llvm::CmpInst::FCMP_ONE},
            {OpType::LT, llvm::CmpInst::FCMP_OLT},
            {OpType::GT, llvm::CmpInst::FCMP_OGT},
            {OpType::LE, llvm::CmpInst::FCMP_OLE},
            {OpType::GE, llvm::CmpInst::FCMP_OGE}};

        // if either operand is of double type
        if (op1->getType()->isDoubleTy() || op2->getType()->isDoubleTy())
        {
            codegenOutput << "BinaryOp::code_gen: either op1 and op2 is double" << std::endl;
            // convert to double type
            if (!op1->getType()->isDoubleTy())
            {
                op1 = context.Builder.CreateSIToFP(op1, context.Builder.getDoubleTy());
            }
            else if (!op2->getType()->isDoubleTy())
            {
                op2 = context.Builder.CreateSIToFP(op2, context.Builder.getDoubleTy());
            }

            // get compare operator
            auto it = FTypeAlt.find(op);
            if (it != FTypeAlt.end())
            {
                // compare operator
                return context.Builder.CreateFCmp(it->second, op1, op2);
            }

            // numerical operator
            llvm::Instruction::BinaryOps binop;
            switch (op)
            {
            case OpType::PLUS:
                binop = llvm::Instruction::FAdd;
                break;
            case OpType::MINUS:
                binop = llvm::Instruction::FSub;
                break;
            case OpType::MUL:
                binop = llvm::Instruction::FMul;
                break;
            case OpType::DIV: // fall through
            case OpType::SLASH:
                binop = llvm::Instruction::FDiv;
                break;
            default:
                std::cerr << "BinaryOp::code_gen: operation not valid" << std::endl;
            }
            return context.Builder.CreateBinOp(binop, op1, op2);
        }
        // both operands are boolean
        else if (op1->getType()->isIntegerTy(1) && op2->getType()->isIntegerTy(1))
        {
            codegenOutput << "BinaryOp::code_gen: either op1 and op2 are both boolean" << std::endl;

            // get compare operator
            auto it = BTypeAlt.find(op);
            if (it != BTypeAlt.end())
            {
                // compare operator
                return context.Builder.CreateICmp(it->second, op1, op2);
            }

            // numerical operator
            llvm::Instruction::BinaryOps binop;
            switch (op)
            {
            case OpType::AND:
                binop = llvm::Instruction::And;
                break;
            case OpType::OR:
                binop = llvm::Instruction::Or;
                break;
            case OpType::XOR:
                binop = llvm::Instruction::Xor;
                break;
            default:
                std::cerr << "BinaryOp::code_gen: operation not valid" << std::endl;
            }
            return context.Builder.CreateBinOp(binop, op1, op2);
        }
        // both operands are interger
        else if (op1->getType()->isIntegerTy(32) && op2->getType()->isIntegerTy(32))
        {
            codegenOutput << "BinaryOp::code_gen: either op1 and op2 are both integer" << std::endl;

            // get compare operator
            auto it = BTypeAlt.find(op);
            if (it != BTypeAlt.end())
            {
                // compare operator
                return context.Builder.CreateICmp(it->second, op1, op2);
            }

            // numerical and logical operator
            llvm::Instruction::BinaryOps binop;
            switch (op)
            {
            case OpType::PLUS:
                binop = llvm::Instruction::Add;
                break;
            case OpType::MINUS:
                binop = llvm::Instruction::Sub;
                break;
            case OpType::MUL:
                binop = llvm::Instruction::Mul;
                break;
            case OpType::DIV:
                binop = llvm::Instruction::SDiv;
                break;
            case OpType::MOD:
                binop = llvm::Instruction::SRem;
                break;
            case OpType::AND:
                binop = llvm::Instruction::And;
                break;
            case OpType::OR:
                binop = llvm::Instruction::Or;
                break;
            case OpType::XOR:
                binop = llvm::Instruction::Xor;
                break;
            case OpType::SLASH:
                // float division
                op1 = context.Builder.CreateSIToFP(op1, context.Builder.getDoubleTy());
                op2 = context.Builder.CreateSIToFP(op2, context.Builder.getDoubleTy());
                binop = llvm::Instruction::FDiv;
                break;
            default:
                std::cerr << "BinaryOp::code_gen: operation not valid" << std::endl;
            }
            return context.Builder.CreateBinOp(binop, op1, op2);
        }
        return nullptr;
    }
} // namespace ast

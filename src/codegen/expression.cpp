#include "../AST/expression.hpp"
#include <iostream>

namespace ast
{
    llvm::Value *FuncCall::code_gen(CodeGenContext &context)
    {
        std::cout << "inside func" << std::endl;
        auto *func = context.module->getFunction(name->name);
        if (func->arg_size() != args.size())
        {
            // throw CodegenException("Argument not match for function " + name->GetName());
            std::cerr << "Argument not match for function " + name->GetName() << std::endl;
        }
        std::vector<llvm::Value *> values;
        for (auto &arg : args)
        {
            values.push_back(arg->code_gen(context));
        }
        return context.Builder.CreateCall(func, values);
    }

    llvm::Value *ProcCall::code_gen(CodeGenContext &context)
    {
    }

    llvm::Value *SysFuncCall::code_gen(CodeGenContext &context)
    {
        std::cout << "inside sysfunc" << std::endl;
        if (name == SysFunc::WRITE || name == SysFunc::WRITELN)
        {
            for (auto &arg : this->args->get_children())
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
                else if (value->getType()->isArrayTy())
                {
                    func_args.push_back(context.Builder.CreateGlobalStringPtr("%s"));
                    //func_args.push_back(value);
                    auto real_arg = std::dynamic_pointer_cast<IdentifierAST>(arg);
                    auto *value2 = real_arg->GetPtr(context);
                    func_args.push_back(value2);
                    //std::string mystr2 = value2->getName().str();
                    //func_args.push_back(context.Builder.CreateGlobalStringPtr(mystr2));
                }
                //   else if (value->getType()->)
                // TODO: string support
                else
                {
                    // throw CodegenException("incompatible type for sysfunc call");
                    std::cerr << "incompatible type for sysfunc call" << std::endl;
                }
                context.Builder.CreateCall(context.PrintfFunction, func_args);
            }
            // if (name == SysFunc::WRITELN)
            // {
            //     context.Builder.CreateCall(context.PrintfFunction, context.Builder.CreateGlobalStringPtr("\n"));
            // }
            return nullptr;
        }
    }

    llvm::Value *SysProcCall::code_gen(CodeGenContext &context)
    {
    }

    llvm::Value *BinaryOp::code_gen(CodeGenContext &context)
    {
        std::cout << "inside binary expr" << std::endl;
        auto *op1 = this->op1->code_gen(context);
        auto *op2 = this->op2->code_gen(context);
        std::map<OpType, llvm::CmpInst::Predicate> BTypeAlt = {
            {OpType::GT, llvm::CmpInst::ICMP_SGT},
            {OpType::GE, llvm::CmpInst::ICMP_SGE},
            {OpType::LT, llvm::CmpInst::ICMP_SLT},
            {OpType::LE, llvm::CmpInst::ICMP_SLE},
            {OpType::EQ, llvm::CmpInst::ICMP_EQ},
            {OpType::NE, llvm::CmpInst::ICMP_NE}};
        std::map<OpType, llvm::CmpInst::Predicate> FTypeAlt = {
            {OpType::GT, llvm::CmpInst::FCMP_OGT},
            {OpType::GE, llvm::CmpInst::FCMP_OGE},
            {OpType::LT, llvm::CmpInst::FCMP_OLT},
            {OpType::LE, llvm::CmpInst::FCMP_OLE},
            {OpType::EQ, llvm::CmpInst::FCMP_OEQ},
            {OpType::NE, llvm::CmpInst::FCMP_ONE}};

        //lhs = context.Builder.CreateLoad(lhs);
        //return context.Builder.CreateICmp(llvm::CmpInst::ICMP_EQ, rhs, rhs);

        if (op1->getType()->isDoubleTy() || op2->getType()->isDoubleTy())
        {
            std::cout << "debug1" << std::endl;
            if (!op1->getType()->isDoubleTy())
            {
                op1 = context.Builder.CreateSIToFP(op1, context.Builder.getDoubleTy());
            }
            else if (!op2->getType()->isDoubleTy())
            {
                op2 = context.Builder.CreateSIToFP(op2, context.Builder.getDoubleTy());
            }
            auto it = FTypeAlt.find(op);
            if (it != FTypeAlt.end())
            {
                return context.Builder.CreateFCmp(it->second, op1, op2);
            }
            llvm::Instruction::OpTypes binop;
            switch (op)
            {
            case OpType::ADD:
                binop = llvm::Instruction::FAdd;
                break;
            case OpType::SUB:
                binop = llvm::Instruction::FSub;
                break;
            case OpType::SLASH:
                binop = llvm::Instruction::FDiv;
                break;
            case OpType::MUL:
                binop = llvm::Instruction::FMul;
                break;
            default:
                throw CodegenException("operation not valid");
            }
            return context.Builder.CreateBinOp(binop, op1, op2);
        }

        else if (op1->getType()->isIntegerTy(1) && op2->getType()->isIntegerTy(1))
        {
            std::cout << "debug1" << std::endl;
            auto it = BTypeAlt.find(op);
            if (it != BTypeAlt.end())
            {
                return context.Builder.CreateICmp(it->second, op1, op2);
            }
            llvm::Instruction::OpTypes binop;
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
                throw CodegenException("operation not valid");
            }
            return context.Builder.CreateBinOp(binop, op1, op2);
        }

        else if (op1->getType()->isIntegerTy(32) && op2->getType()->isIntegerTy(32))
        {
            std::cout << "debug2" << std::endl;
            std::cout << "inside int32 and int32." << std::endl;
            auto it = BTypeAlt.find(op);
            if (it != BTypeAlt.end())
            {
                return context.Builder.CreateICmp(it->second, op1, op2);
            }
            llvm::Instruction::OpTypes binop;
            switch (op)
            {
            case OpType::ADD:
                binop = llvm::Instruction::Add;
                break;
            case OpType::SUB:
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
                op1 = context.Builder.CreateSIToFP(op1, context.Builder.getDoubleTy());
                op2 = context.Builder.CreateSIToFP(op2, context.Builder.getDoubleTy());
                binop = llvm::Instruction::FDiv;
                break;
            default:
                throw CodegenException("operator not valid");
            }
            return context.Builder.CreateBinOp(binop, op1, op2);
        }
    }
} // namespace ast

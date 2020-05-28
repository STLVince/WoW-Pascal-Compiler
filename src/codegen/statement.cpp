#include "../AST/statement.hpp"

namespace ast
{
    llvm::Value *AssignmentStmt::code_gen(CodeGenContext &context)
    {
        std::cerr << "AssignmentStmt::code_gen: inside assignment ast" << std::endl;
        // get the lhs pointer, first cast
        // TODO handle array type
        // llvm::Value *lhs;
        // auto id_cast = std::dynamic_pointer_cast<IdentifierAST>(this->lhs);
        // if (id_cast) {
        //     lhs = id_cast->GetPtr(context);
        // }
        // else {
        //     auto array_cast = std::dynamic_pointer_cast<ArrayAccessAST>(this->lhs);
        //     if (array_cast) {
        //         lhs = array_cast->GetPtr(context);
        //     }
        //     else
        //         // throw CodegenException("assignment left argument not a identifier.\n");
        //         std::cerr << "assignment left argument not a identifier.\n" << std::endl;
        // }
        auto lhs = this->lhs->GetPtr(context);
        if (std::dynamic_pointer_cast<FuncCall>(this->lhs))
        {
            std::cerr << "It is a custom function call!" << std::endl;
        }

        auto *rhs = this->rhs->code_gen(context);
        //rhs = context.Builder.CreateLoad(rhs);

        auto *lhs_type = lhs->getType()->getPointerElementType();
        auto *rhs_type = rhs->getType();

        if (lhs_type->isDoubleTy() && rhs_type->isIntegerTy(32))
        {
            rhs = context.Builder.CreateSIToFP(rhs, context.Builder.getDoubleTy());
        }
        else if (!((lhs_type->isIntegerTy(1) && rhs_type->isIntegerTy(1))                                                          // bool
                   || (lhs_type->isIntegerTy(32) && rhs_type->isIntegerTy(32))                                                     // int
                   || (lhs_type->isDoubleTy() && rhs_type->isDoubleTy())                                                           // float
                   || (lhs_type->isArrayTy() && rhs_type->isPointerTy()) || (lhs_type->isArrayTy() && rhs_type->isIntegerTy(32)))) // string
        {
            // throw CodegenException("incompatible assignment type");
            std::cerr << "incompatible assignment type" << std::endl;
        }
        auto *lhs_type2 = lhs->getType()->getPointerElementType();
        //assert(rhs->getType() == llvm::cast<llvm::PointerType>(lhs->getType())->getElementType());
        context.Builder.CreateStore(rhs, lhs);
        return nullptr;
    }

    llvm::Value *IfStmt::code_gen(CodeGenContext &context)
    {
        auto *cond = condition->code_gen(context);
        if (!cond->getType()->isIntegerTy(1))
        {
            // throw CodegenException("if statement not boolean");
            std::cerr << "if statement not boolean" << std::endl;
        }

        // create block for the then and else cases
        // insert the then block at the end of the function
        auto *function = context.Builder.GetInsertBlock()->getParent();
        auto *then_stat = llvm::BasicBlock::Create(context.module->getContext(), "then", function);
        auto *else_stat = llvm::BasicBlock::Create(context.module->getContext(), "else");
        auto *end = llvm::BasicBlock::Create(context.module->getContext(), "end");
        context.Builder.CreateCondBr(cond, then_stat, else_stat);

        // emit then code
        context.Builder.SetInsertPoint(then_stat);
        then_stmt->code_gen(context);
        context.Builder.CreateBr(end);

        // emit else code
        function->getBasicBlockList().push_back(else_stat);
        context.Builder.SetInsertPoint(else_stat);
        if (else_stmt != nullptr)
        {
            else_stmt->code_gen(context);
        }        
        context.Builder.CreateBr(end);

        // emit merge block
        function->getBasicBlockList().push_back(end);
        context.Builder.SetInsertPoint(end);
    }

    llvm::Value *WhileStmt::code_gen(CodeGenContext &context)
    {
        auto *function = context.Builder.GetInsertBlock()->getParent();
        auto *while_stat = llvm::BasicBlock::Create(context.module->getContext(), "while", function);
        auto *loop_stat = llvm::BasicBlock::Create(context.module->getContext(), "loop", function);
        auto *end = llvm::BasicBlock::Create(context.module->getContext(), "end");

        context.Builder.CreateBr(while_stat);
        context.Builder.SetInsertPoint(while_stat);

        auto *cond = condition->code_gen(context);
        if (!cond->getType()->isIntegerTy(1))
        { // not boolean
            // throw CodegenException("while statement not boolean");
            std::cerr << "while statement not boolean" << std::endl;
        }
        context.Builder.CreateCondBr(cond, loop_stat, end); // true -> loop_stat, false -> end

        context.Builder.SetInsertPoint(loop_stat);
        loop_stmt->code_gen(context);
        context.Builder.CreateBr(while_stat); // context -> statement

        function->getBasicBlockList().push_back(end); // end label
        context.Builder.SetInsertPoint(end);

        return nullptr;
    }

    llvm::Value *RepeatStmt::code_gen(CodeGenContext &context)
    {
        llvm::BasicBlock *loopStmtB = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "REPEATloopStmt", context.currentFunction);
        llvm::BasicBlock *loopEndB = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "REPEATloopEnd", context.currentFunction);
        llvm::BasicBlock *loopExitB = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "REPEATloopExit", context.currentFunction);

        context.Builder.CreateBr(loopStmtB);

        context.Builder.SetInsertPoint(loopStmtB);
        context.pushBlock(loopStmtB);
        loop_stmt->code_gen(context);
        context.Builder.CreateBr(loopEndB);
        context.popBlock();

        context.pushBlock(loopEndB);
        context.Builder.SetInsertPoint(loopEndB);
        llvm::Value *test = this->condition->code_gen(context);
        llvm::Value *ret = context.Builder.CreateCondBr(test, loopExitB, loopStmtB);
        context.popBlock();

        context.pushBlock(loopExitB);
        context.Builder.SetInsertPoint(loopExitB);

        return ret;
    }

    llvm::Value *ForStmt::code_gen(CodeGenContext &context)
    {
        llvm::BasicBlock *loopEntryB = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "FORloopEntry", context.currentFunction);
        llvm::BasicBlock *loopStmtB = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "FORloopStmt", context.currentFunction);
        llvm::BasicBlock *loopEndB = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "FORloopEnd", context.currentFunction);
        llvm::BasicBlock *loopExitB = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "FORloopExit", context.currentFunction);

        context.Builder.CreateBr(loopEntryB);
        context.pushBlock(loopEntryB);
        context.Builder.SetInsertPoint(loopEntryB);

        // initial for   
        AssignmentStmt *assign = new AssignmentStmt(this->loop_var, this->start_val);
        assign->code_gen(context);
        context.Builder.CreateBr(loopStmtB);
        context.popBlock();
        context.pushBlock(loopStmtB);

        // emit loop body code
        context.Builder.SetInsertPoint(loopStmtB);
        this->loop_stmt->code_gen(context);
        context.Builder.CreateBr(loopEndB);
        context.popBlock();
        context.pushBlock(loopEndB);

        // emit step code
        context.Builder.SetInsertPoint(loopEndB);
        auto int1 = new IntegerType(1);
        BinaryOp *binOP;
        if (this->direct == 1)
        {
            binOP = new BinaryOp(this->loop_var, OpType::PLUS, std::shared_ptr<IntegerType>(int1));
        }
        else
        {
            binOP = new BinaryOp(this->loop_var, OpType::MINUS, std::shared_ptr<IntegerType>(int1));
        }
        AssignmentStmt *assign2 = new AssignmentStmt(this->loop_var, std::shared_ptr<BinaryOp>(binOP));
        assign2->code_gen(context);

        // emit test code
        auto testGE = new BinaryOp(this->loop_var, OpType::GT, this->end_val);
        auto test = testGE->code_gen(context);
        auto ret = context.Builder.CreateCondBr(test, loopExitB, loopStmtB);

        context.popBlock();
        context.pushBlock(loopExitB);
        context.Builder.SetInsertPoint(loopExitB);

        return ret;
    }
} // namespace ast

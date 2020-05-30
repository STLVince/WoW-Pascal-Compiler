#include "../AST/statement.hpp"

namespace ast
{
    llvm::Value *AssignmentStmt::code_gen(CodeGenContext &context)
    {
        codegenOutput << "AssignmentStmt::code_gen: inside assignment ast" << std::endl;
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
            codegenOutput << "It is a custom function call!" << std::endl;
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
        for (auto stmt : (*then_stmt))
        {
            stmt->code_gen(context);
        }
        context.Builder.CreateBr(end);

        // emit else code
        function->getBasicBlockList().push_back(else_stat);
        context.Builder.SetInsertPoint(else_stat);
        if (else_stmt != nullptr)
        {
            for (auto stmt : (*else_stmt))
            {
                stmt->code_gen(context);
            }
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
            std::cerr << "while statement not boolean" << std::endl;
        }
        context.Builder.CreateCondBr(cond, loop_stat, end);

        context.Builder.SetInsertPoint(loop_stat);
        for (auto stmt : (*loop_stmt))
        {
            stmt->code_gen(context);
        }
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
        // context.pushBlock(loopStmtB);
        for (auto stmt : (*loop_stmt))
        {
            stmt->code_gen(context);
        }
        context.Builder.CreateBr(loopEndB);
        // context.popBlock();

        // context.pushBlock(loopEndB);
        context.Builder.SetInsertPoint(loopEndB);
        llvm::Value *test = this->condition->code_gen(context);
        llvm::Value *ret = context.Builder.CreateCondBr(test, loopExitB, loopStmtB);
        // context.popBlock();

        // context.pushBlock(loopExitB);
        context.Builder.SetInsertPoint(loopExitB);

        return ret;
    }

    llvm::Value *ForStmt::code_gen(CodeGenContext &context)
    {
        if (!loop_var->code_gen(context)->getType()->isIntegerTy(32))
        {
            std::err << "ForStmt::code_gen: for loop identifier not integer" << std::endl;
        }
        // create init statement
        auto init = new AssignStmtAST(loop_var, start_val);

        // create condition check
        auto upto = direct == 1;
        auto cond = new BinaryOp(upto ? BinaryOp::LE : BinaryOp::GE, loop_var, end_val);

        // create iteration stmt
        auto iter = new AssignStmtAST(identifier, new BinaryExprAST(upto ? BinaryOp::ADD : BinaryOp::SUB, identifier, new IntegerAST(1)));

        // convert for stmt to while stmt
        auto statementList = new StatementList();
        for (auto stmt : *loop_stmt)
        {
            statementList->push_back(stmt);
        }
        statementList->push_back(iter);
        auto while_stmt = new WhileStmtAST(cond, statementList);

        // generate code
        init->code_gen(context);
        while_stmt->code_gen(context);
        return nullptr;
    }

    llvm::Value *CaseStmt::code_gen(CodeGenContext &context)
    {
        codegenOutput << "CaseStmt::code_gen: in case" << std::endl;
        llvm::Value * ret;
        for (auto stmt : (*then_stmt))
        {
            ret = stmt->code_gen(context);
        }
        return ret;
    }

    llvm::Value *SwitchStmt::code_gen(CodeGenContext &context)
    {
        codegenOutput << "CaseStmt::code_gen: in switch" << std::endl;
        llvm::BasicBlock *bexit = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "exit", context.currentFunction);

        // create basic blocks for each case
        std::vector<llvm::BasicBlock *> basic_blocks;
        for (int i = 0; i < (list->size()); i++)
        {
            auto basic_block = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "caseStmt", context.currentFunction);
            basic_blocks.push_back(basic_block);
        }

        // code gen for each case condition
        for (int i = 0; i < basic_blocks.size() - 1; i++)
        {
            auto cond = new BinaryOp(expression, OpType::EQ, (*list)[i]->condition);
            llvm::BasicBlock *bnext = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "next", context.currentFunction);
            // llvm::BranchInst::Create(basic_blocks[i], bnext, cond->code_gen(context), context.currentBlock());
            context.Builder.CreateCondBr(cond->code_gen(context), basic_blocks[i], bnext);
            // context.pushBlock(bnext);
            context.Builder.SetInsertPoint(bnext);
        }

        auto cond = new BinaryOp(expression, OpType::EQ, (*list)[basic_blocks.size() - 1]->condition);
        // auto ret = llvm::BranchInst::Create(basic_blocks[basic_blocks.size() - 1], bexit, cond->code_gen(context), context.currentBlock());
        auto ret = context.Builder.CreateCondBr(cond->code_gen(context), basic_blocks[basic_blocks.size() - 1], bexit);
        // code gen for each case body
        for (int i = 0; i < basic_blocks.size(); i++)
        {
            auto cst = (*list)[i];
            cst->bexit = bexit;
            cst->code_gen(context);
            context.Builder.CreateBr(basic_blocks[i]);
        }

        // context.pushBlock(bexit);
        context.Builder.SetInsertPoint(bexit);

        return ret;
    }

    llvm::Value *LabelStmt::code_gen(CodeGenContext &context)
    {
        // llvm::BranchInst::Create(context.labelBlock[label], context.currentBlock());
        llvm::CreateBr(context.labelBlock[label]);
        // context.pushBlock(context.labelBlock[label]);
        context.Builder.SetInsertPoint(context.labelBlock[label]);
        return statement->code_gen(context);
    }

    llvm::Value *GotoStmt::code_gen(CodeGenContext &context)
    {
        llvm::Value *test = (new BooleanType("true"))->code_gen(context);
        llvm::BasicBlock *bafter = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "afterGoto", context.currentFunction);
        // auto ret = llvm::BranchInst::Create(context.labelBlock[label], context.currentBlock());
        auto ret = llvm::CreateBr(context.labelBlock[label]);
        // context.pushBlock(bafter);
        context.Builder.SetInsertPoint(bafter);
        return ret;
    }
} // namespace ast

#include "../AST/statement.hpp"
#include "../AST/identifier.hpp"

namespace ast
{
    void AssignmentStmt::printSelf(std::string nodeName)
    {
        if (lhs)
            astDot << nodeName << "->" << nodeName + "_" + lhs->name << std::endl;
        else if (array_lhs)
            astDot << nodeName << "->" << nodeName + "_array_" + array_lhs->name->name << std::endl;
        std::string childName = nodeName + "_rhs";
        astDot << nodeName << "->" << childName << std::endl;
        rhs->printSelf(childName);
    }

    void IfStmt::printSelf(std::string nodeName)
    {
        std::string condName = nodeName + "_condition";
        astDot << nodeName << "->" << condName << std::endl;
        condition->printSelf(condName);
        std::string thenName = nodeName + "_then";
        astDot << nodeName << "->" << thenName << std::endl;
        for (unsigned int i = 0; i < then_stmt.get()->get_list()->size(); i++)
        {
            std::string childName = nodeName + "_then" + std::to_string(i);
            astDot << nodeName << "->" << childName << std::endl;
            (*(then_stmt.get()->get_list()))[i].get()->printSelf(childName);
        }
        for (unsigned int i = 0; i < else_stmt.get()->get_list()->size(); i++)
        {
            std::string childName = nodeName + "_else" + std::to_string(i);
            astDot << nodeName << "->" << childName << std::endl;
            (*(else_stmt.get()->get_list()))[i].get()->printSelf(childName);
        }
    }

    void WhileStmt::printSelf(std::string nodeName)
    {
        std::string condName = nodeName + "_condition";
        astDot << nodeName << "->" << condName << std::endl;
        condition->printSelf(condName);
        for (unsigned int i = 0; i < loop_stmt.get()->get_list()->size(); i++)
        {
            std::string childName = nodeName + "_loop" + std::to_string(i);
            astDot << nodeName << "->" << childName << std::endl;
            (*(loop_stmt.get()->get_list()))[i].get()->printSelf(childName);
        }
    }

    void RepeatStmt::printSelf(std::string nodeName)
    {
        std::string condName = nodeName + "_condition";
        astDot << nodeName << "->" << condName << std::endl;
        condition->printSelf(condName);
        for (unsigned int i = 0; i < loop_stmt.get()->get_list()->size(); i++)
        {
            std::string childName = nodeName + "_loop" + std::to_string(i);
            astDot << nodeName << "->" << childName << std::endl;
            (*(loop_stmt.get()->get_list()))[i].get()->printSelf(childName);
        }
    }

    void ForStmt::printSelf(std::string nodeName)
    {
        std::string loop_varName = nodeName + "_loop_var";
        astDot << nodeName << "->" << loop_varName << std::endl;
        loop_var->printSelf(loop_varName);
        std::string start_valName = nodeName + "_start_val";
        astDot << nodeName << "->" << start_valName << std::endl;
        start_val->printSelf(start_valName);
        std::string end_valName = nodeName + "_end_val";
        astDot << nodeName << "->" << end_valName << std::endl;
        end_val->printSelf(end_valName);
        for (unsigned int i = 0; i < loop_stmt.get()->get_list()->size(); i++)
        {
            std::string childName = nodeName + "_loop" + std::to_string(i);
            astDot << nodeName << "->" << childName << std::endl;
            (*(loop_stmt.get()->get_list()))[i].get()->printSelf(childName);
        }
    }

    void CaseStmt::printSelf(std::string nodeName)
    {
        std::string condName = nodeName + "_condition";
        astDot << nodeName << "->" << condName << std::endl;
        condition->printSelf(condName);
        for (unsigned int i = 0; i < this->then_stmt.get()->get_list()->size(); i++)
        {
            std::string childName = nodeName + "_then" + std::to_string(i);
            astDot << nodeName << "->" << childName << std::endl;
            (*(then_stmt.get()->get_list()))[i].get()->printSelf(childName);
        }
    }

    void SwitchStmt::printSelf(std::string nodeName)
    {
        std::string exprName = nodeName + "_expression";
        astDot << nodeName << "->" << exprName << std::endl;
        expression->printSelf(exprName);
        for (unsigned int i = 0; i < list.get()->size(); i++)
        {
            std::string caseName = nodeName + "_case" + std::to_string(i);
            astDot << nodeName << "->" << caseName << std::endl;
            (*(list.get()))[i].get()->printSelf(caseName);
        }
    }

    void LabelStmt::printSelf(std::string nodeName)
    {
        std::string stmtName = nodeName + "_statement";
        astDot << nodeName << "->" << stmtName << std::endl;
        statement->printSelf(stmtName);
    }

    void GotoStmt::printSelf(std::string nodeName)
    {
        std::string labelName = nodeName + "_label" + std::to_string(label);
        astDot << nodeName << "->" << labelName << std::endl;
    }

    llvm::Value *AssignmentStmt::code_gen(CodeGenContext &context)
    {
        codegenOutput << "AssignmentStmt::code_gen: inside assignment ast" << std::endl;

        llvm::Value *lhs;
        //auto id_cast = std::dynamic_pointer_cast<Identifier>(this->lhs);
        if (this->lhs)
        {
            lhs = this->lhs->GetPtr(context);
        }
        else
        {
            //auto array_cast = std::dynamic_pointer_cast<ArrayAccess>(this->array_lhs);
            if (this->array_lhs)
            {
                lhs = this->array_lhs->GetPtr(context);
            }
            else
            {
                std::cerr << "AssignmentStmt::code_gen: assignment left argument not a identifier." << std::endl;
            }
        }
        
        auto *rhs = this->rhs->code_gen(context);

        auto *lhs_type = lhs->getType()->getPointerElementType();
        auto *rhs_type = rhs->getType();

        if (lhs_type->isDoubleTy() && rhs_type->isIntegerTy(32))
        {
            rhs = context.Builder.CreateSIToFP(rhs, context.Builder.getDoubleTy());
        }
        else if (!((lhs_type->isIntegerTy(1) && rhs_type->isIntegerTy(1))                                                          // bool
                   || (lhs_type->isIntegerTy(32) && rhs_type->isIntegerTy(32))                                                     // int
                   || (lhs_type->isIntegerTy(8) && rhs_type->isIntegerTy(8))                                                       // char
                   || (lhs_type->isDoubleTy() && rhs_type->isDoubleTy())                                                           // float
                   || (lhs_type->isArrayTy() && rhs_type->isPointerTy()) || (lhs_type->isArrayTy() && rhs_type->isIntegerTy(32)))) // string
        {
            std::cerr << "AssignmentStmt::code_gen: incompatible assignment type" << std::endl;
        }
        // auto *lhs_type2 = lhs->getType()->getPointerElementType();
        // codegenOutput << typeid(rhs->getType()).name() << std::endl;
        // codegenOutput << typeid(lhs->getType()).name() << std::endl;
        // assert(rhs->getType() == llvm::cast<llvm::PointerType>(lhs->getType())->getElementType());
        codegenOutput << "AssignmentStmt::code_gen: before create store" << std::endl;
        context.Builder.CreateStore(rhs, lhs);
        codegenOutput << "AssignmentStmt::code_gen: after create store" << std::endl;
        return nullptr;
    }

    llvm::Value *IfStmt::code_gen(CodeGenContext &context)
    {
        codegenOutput << "IfStmt::code_gen: inside IfStmt ast" << std::endl;

        auto *cond = condition->code_gen(context);
        if (!cond->getType()->isIntegerTy(1))
        {
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
        for (auto stmt : *(then_stmt->get_list()))
        {
            stmt->code_gen(context);
        }
        context.Builder.CreateBr(end);

        // emit else code
        function->getBasicBlockList().push_back(else_stat);
        context.Builder.SetInsertPoint(else_stat);
        if (else_stmt != nullptr)
        {
            for (auto stmt : *(else_stmt->get_list()))
            {
                stmt->code_gen(context);
            }
        }
        context.Builder.CreateBr(end);

        // emit merge block
        function->getBasicBlockList().push_back(end);
        context.Builder.SetInsertPoint(end);
        return nullptr;
    }

    llvm::Value *WhileStmt::code_gen(CodeGenContext &context)
    {
        codegenOutput << "WhileStmt::code_gen: inside WhileStmt ast" << std::endl;

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
        for (auto stmt : *(loop_stmt->get_list()))
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
        codegenOutput << "RepeatStmt::code_gen: inside RepeatStmt ast" << std::endl;

        llvm::BasicBlock *loopStmtB = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "REPEATloopStmt", context.currentFunction);
        llvm::BasicBlock *loopEndB = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "REPEATloopEnd", context.currentFunction);
        llvm::BasicBlock *loopExitB = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "REPEATloopExit", context.currentFunction);

        context.Builder.CreateBr(loopStmtB);
        context.Builder.SetInsertPoint(loopStmtB);
        for (auto stmt : *(loop_stmt->get_list()))
        {
            stmt->code_gen(context);
        }
        context.Builder.CreateBr(loopEndB);

        context.Builder.SetInsertPoint(loopEndB);
        llvm::Value *test = this->condition->code_gen(context);
        llvm::Value *ret = context.Builder.CreateCondBr(test, loopExitB, loopStmtB);

        context.Builder.SetInsertPoint(loopExitB);

        return ret;
    }

    llvm::Value *ForStmt::code_gen(CodeGenContext &context)
    {
        codegenOutput << "ForStmt::code_gen: inside ForStmt ast" << std::endl;

        if (!loop_var->code_gen(context)->getType()->isIntegerTy(32))
        {
            std::cerr << "ForStmt::code_gen: for loop identifier not integer" << std::endl;
        }
        // create init statement
        auto init = make_node<AssignmentStmt>(loop_var, start_val);

        // create condition check
        auto upto = direct == 1;
        auto cond = make_node<BinaryOp>(loop_var, upto ? OpType::LE : OpType::GE, end_val);

        // create iteration stmt
        auto iter = make_node<AssignmentStmt>(loop_var, make_node<BinaryOp>(loop_var, upto ? OpType::PLUS : OpType::MINUS, make_node<IntegerType>(1)));

        // convert for stmt to while stmt

        auto statementList = make_node<StatementList>();
        for (auto stmt : *(loop_stmt->get_list()))
        {
            statementList->get_list()->push_back(stmt);
        }
        statementList->get_list()->push_back(iter);
        auto while_stmt = make_node<WhileStmt>(cond, statementList);

        // generate code
        init->code_gen(context);
        while_stmt->code_gen(context);
        return nullptr;
    }

    llvm::Value *CaseStmt::code_gen(CodeGenContext &context)
    {
        codegenOutput << "CaseStmt::code_gen: in case" << std::endl;
        llvm::Value *ret;
        for (auto stmt : *(then_stmt->get_list()))
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
        for (unsigned int i = 0; i < (list->size()); i++)
        {
            auto basic_block = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "caseStmt", context.currentFunction);
            basic_blocks.push_back(basic_block);
        }

        // code gen for each case condition
        for (unsigned int i = 0; i < basic_blocks.size() - 1; i++)
        {
            auto cond = new BinaryOp(expression, OpType::EQ, (*list)[i]->condition);
            llvm::BasicBlock *bnext = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "next", context.currentFunction);
            context.Builder.CreateCondBr(cond->code_gen(context), basic_blocks[i], bnext);
            context.Builder.SetInsertPoint(bnext);
        }

        auto cond = new BinaryOp(expression, OpType::EQ, (*list)[basic_blocks.size() - 1]->condition);
        auto ret = context.Builder.CreateCondBr(cond->code_gen(context), basic_blocks[basic_blocks.size() - 1], bexit);
        // code gen for each case body
        for (unsigned int i = 0; i < basic_blocks.size(); i++)
        {
            auto cst = (*list)[i];
            cst->bexit = bexit;
            cst->code_gen(context);
            context.Builder.CreateBr(basic_blocks[i]);
        }

        context.Builder.SetInsertPoint(bexit);

        return ret;
    }

    llvm::Value *LabelStmt::code_gen(CodeGenContext &context)
    {
        codegenOutput << "LabelStmt::code_gen: inside LabelStmt ast" << std::endl;
        context.Builder.CreateBr(context.labelBlock[label]);
        context.Builder.SetInsertPoint(context.labelBlock[label]);
        return statement->code_gen(context);
    }

    llvm::Value *GotoStmt::code_gen(CodeGenContext &context)
    {
        codegenOutput << "GotoStmt::code_gen: inside GotoStmt ast" << std::endl;
        llvm::Value *test = (new BooleanType("true"))->code_gen(context);
        llvm::BasicBlock *bafter = llvm::BasicBlock::Create(GlobalLLVMContext::getGlobalContext(), "afterGoto", context.currentFunction);
        auto ret = context.Builder.CreateBr(context.labelBlock[label]);
        context.Builder.SetInsertPoint(bafter);
        return ret;
    }
} // namespace ast

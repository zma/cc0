#include "ILGenerator.h"

#include <core/Core.h>

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


ILGenerator::ILGenerator()
{
    Reset();
}

ILGenerator::~ILGenerator()
{

}

ILProgram* ILGenerator::GetILProgram()
{
    return _program;
}

void ILGenerator::Reset()
{
    ExpressionVisitor::Reset();

    _program = NULL;
    while(!_staticChain.empty())
    {
        _staticChain.pop();
    }
    while(!_loopContext.empty())
    {
        _loopContext.pop();
    }
    _tempId = 0;
}

void ILGenerator::Visit(BinaryExpression* node)
{
    Expression *left = node->GetLeftOperand();
    Expression *right = node->GetRightOperand();

    left->Accept(this);
    right->Accept(this);

    Expression::ExpressionAction action = node->GetAction();

    // Note: the type here is the target type.
    // For certain operations, the type of operands may not be
    // the same as target operands type.
    // For example, comparing 2 doubles. The target is bool.
    Type *type = node->GetTag<Type>("Type");

    if(CompilationContext::GetInstance()->Debug)
    {
        std::cout << "DBG: " << "IL generating: comparison: type "
                  << GetOperandType(type) << std::endl;
    }

   
    
    IL::ILOpcode opcode;
    IL::ILOperand op1, op2, op3;
    op1 = GenerateTemp(type);
    op2 = *(left->GetTag<IL::ILOperand>("ILOperand"));
    op3 = *(right->GetTag<IL::ILOperand>("ILOperand"));

    switch(action)
    {
        case Expression::Add:
            opcode = IL::Add;
            goto arithmatic;
        case Expression::Subtract:
            opcode = IL::Sub;
            goto arithmatic;
        case Expression::Multiply:
            opcode = IL::Mul;
            goto arithmatic;
        case Expression::Divide:
            opcode = IL::Div;
            goto arithmatic;
        case Expression::Modulo:
            opcode = IL::Mod;
            goto arithmatic;
        arithmatic:
            op1.OperandType = op2.OperandType = op3.OperandType = GetOperandType(type);
            break;
        case Expression::And:
            opcode = IL::And;
            goto logical;
        case Expression::Or:
            opcode = IL::Or;
            goto logical;;
        case Expression::ExclusiveOr:
            opcode = IL::Xor;
            goto logical;;
        logical:
            type = node->GetTag<Type>("Type");
            op1.OperandType = op2.OperandType = op3.OperandType = GetOperandType(type);
            break;
        case Expression::LeftShift:
            opcode = IL::Shl;
            goto shifting;
        case Expression::RightShift:
            opcode = IL::Shr;
            goto shifting;
        shifting:
            op1.OperandType = op2.OperandType = GetOperandType(type);
            op3.OperandType = IL::U1;
            break;
        case Expression::Equal:
            opcode = IL::CompEq;
            goto comparison;
        case Expression::GreaterThan:
            opcode = IL::CompGt;
            goto comparison;
        case Expression::GreaterThanOrEqual:
            opcode = IL::CompGe;
            goto comparison;
        case Expression::LessThan:
            opcode = IL::CompLt;
            goto comparison;
        case Expression::LessThanOrEqual:
            opcode = IL::CompLe;
            goto comparison;
        case Expression::NotEqual:
            opcode = IL::CompNe;
            goto comparison;
        comparison:
            op1.OperandType = IL::I; // Bool

            // the op2 and op3's types no need to the same as the target
            // op2.OperandType = op3.OperandType = GetOperandType(type);
            op2.OperandType = op3.OperandType = GetOperandType(left->GetTag<Type>("Type"));
            
            if(CompilationContext::GetInstance()->Debug)
            {
                std::cout << "DBG: " << "IL generating: comparison: " << op1.OperandType << ", "
                          << op2.OperandType << ", "
                          << op3.OperandType << std::endl;
            }

            break;
        case Expression::AndAlso:
        case Expression::OrElse:
            assert("Not implemented: short-circuit boolean expression");
            break;
        default:
            abort();
    }

    GenerateIL(opcode, op1, op2, op3);
    node->SetTag<IL::ILOperand>("ILOperand", new IL::ILOperand(op1));
}

void ILGenerator::Visit(BlockExpression* node)
{
    _staticChain.push(node->GetBlockScope());

    std::vector<Expression *> *body = node->GetExpressionList();
    for(std::vector<Expression *>::iterator it = body->begin(); it != body->end(); ++it)
    {
        Expression *expr = *it;
        expr->Accept(this);
    }

    _staticChain.pop();
}

void ILGenerator::Visit(ConditionalExpression* node)
{
    std::map<Expression*, Expression*> *condBrs = node->GetConditionalBranches();
    Expression *defBr = node->GetDefaultBranch();

    IL::ILOperand breakLabel = GenerateTemp(new LabelType());
    node->SetTag<IL::ILOperand>("BreakLabel", new IL::ILOperand(breakLabel));
    
    
    
    if(condBrs->size() == 1 && !node->GoThrough())
    {
        Expression *cond = condBrs->begin()->first;
        Expression *trueBr = condBrs->begin()->second;
        Expression *falseBr = defBr;

        cond->Accept(this);
        IL::ILOperand condOp = *(cond->GetTag<IL::ILOperand>("ILOperand"));

        IL::ILOperand trueLabel = GenerateTemp(new LabelType());
    
        GenerateIL(IL::BrNz, condOp, trueLabel);
        falseBr->Accept(this);
        GenerateIL(IL::Br, breakLabel);

        GenerateLabel(trueLabel);
        trueBr->Accept(this);
        GenerateLabel(breakLabel);
    }
    else
    {
        _loopContext.push(node);
        for(std::map<Expression*, Expression*>::iterator it = condBrs->begin(); it != condBrs->end(); ++it)
        {
            IL::ILOperand branchLabel = GenerateTemp(new LabelType());

            Expression *cond = it->first;
            Expression *body = it->second;

            cond->Accept(this);
            IL::ILOperand condop = *(cond->GetTag<IL::ILOperand>("ILOperand"));

            GenerateIL(IL::BrNz, condop, branchLabel);

            cond->SetTag<IL::ILOperand>("BranchLabel", new IL::ILOperand(branchLabel));
        }

        IL::ILOperand defLabel = GenerateTemp(new LabelType());
        GenerateIL(IL::Br, defLabel);

        

        for(std::map<Expression*, Expression*>::iterator it = condBrs->begin(); it != condBrs->end(); ++it)
        {
            Expression *cond = it->first;
            Expression *body = it->second;

            IL::ILOperand condop = *(cond->GetTag<IL::ILOperand>("ILOperand"));
            IL::ILOperand branchLabel = *(cond->GetTag<IL::ILOperand>("BranchLabel"));
            GenerateLabel(branchLabel);

            body->Accept(this);
            if(!node->GoThrough())
            {
                GenerateIL(IL::Br, breakLabel);
            }
        }

        GenerateLabel(defLabel);
        defBr->Accept(this);
        if(!node->GoThrough())
        {
            GenerateIL(IL::Br, breakLabel);
        }

        GenerateLabel(breakLabel);
        _loopContext.pop();
    }
    
}

void ILGenerator::Visit(ConstantExpression* node)
{
    ConstantValue *value = node->GetValue();
    Type *type = value->GetType();
    IL::ILOperandType optype = GetOperandType(type);
    switch(optype)
    {
        case IL::I:
        case IL::I1:
        case IL::I2:
        case IL::I4:
        case IL::I8:
            node->SetTag<IL::ILOperand>("ILOperand", new IL::ILOperand(IL::Constant, optype, value->GetIntValue()));
            break;
        case IL::U:
        case IL::U1:
        case IL::U2:
        case IL::U4:
        case IL::U8:
            node->SetTag<IL::ILOperand>("ILOperand", new IL::ILOperand(IL::Constant, optype, value->GetUIntValue()));
            break;
        case IL::R4:
            node->SetTag<IL::ILOperand>("ILOperand", new IL::ILOperand(IL::Constant, optype, (float)(value->GetDoubleValue())));
            break;
        case IL::R8:
            node->SetTag<IL::ILOperand>("ILOperand", new IL::ILOperand(IL::Constant, optype, value->GetDoubleValue()));
            break;
        case IL::Str:
            node->SetTag<IL::ILOperand>("ILOperand", new IL::ILOperand(IL::Constant, optype, value->GetStringValue()));
            break;
        case IL::Void:
        default:
            assert("Invalid IL operand type");
    }

}

void ILGenerator::Visit(GotoExpression* node)
{
    IL::ILOperand *pop;
    IL::ILOperand targetop;
    switch(node->GetTargetType())
    {
        case GotoExpression::NamedLabel:
            targetop = IL::ILOperand(IL::Variable, IL::Void, new SymbolRef(_staticChain.top(), node->GetLabelName()));
            break;
        case GotoExpression::Continue:
            if(_loopContext.empty())
            {
                CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "\'continue\' is not valid in this context.");
            }
            pop = _loopContext.top()->GetTag<IL::ILOperand>("ContinueLabel");
            if(pop == NULL)
            {
                CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "\'continue\' is not valid in this context.");
            }
            targetop = *pop;
            break;
        case GotoExpression::Break:
           if(_loopContext.empty())
            {
                CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "\'break\' is not valid in this context.");
            }
            pop = _loopContext.top()->GetTag<IL::ILOperand>("BreakLabel");
            if(pop == NULL)
            {
                CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "\'break\' is not valid in this context.");
            }
            targetop = *pop;
            break;
        default:
            abort();
    }
    GenerateIL(IL::Br, targetop);
}

void ILGenerator::Visit(IndexExpression* node)
{
    node->GetContainer()->Accept(this);
    node->GetIndex()->Accept(this);

    Type *type = node->GetTag<Type>("Type");
    IL::ILOperand *container = node->GetContainer()->GetTag<IL::ILOperand>("ILOperand");
    IL::ILOperand *index = node->GetIndex()->GetTag<IL::ILOperand>("ILOperand");

    IL::ILOperand target = GenerateTemp(type);

    GenerateIL(IL::Ldelem, target, *container, *index);

    node->SetTag<IL::ILOperand>("ILOperand", new IL::ILOperand(target));
}

void ILGenerator::Visit(LabelExpression* node)
{
    GenerateLabel(IL::ILOperand(IL::Variable, IL::Str, node->GetSymbol()));
}

void ILGenerator::Visit(LoopExpression* node)
{
    IL::ILOperand startLabel = GenerateTemp(new LabelType());
    IL::ILOperand contLabel = GenerateTemp(new LabelType());
    IL::ILOperand breakLabel = GenerateTemp(new LabelType());

    node->SetTag<IL::ILOperand>("StartLabel", new IL::ILOperand(startLabel));
    node->SetTag<IL::ILOperand>("ContinueLabel", new IL::ILOperand(breakLabel));
    node->SetTag<IL::ILOperand>("BreakLabel", new IL::ILOperand(breakLabel));

    _loopContext.push(node);

    if(node->GetInitializingExpression() != NULL)
    {
        node->GetInitializingExpression()->Accept(this);
    }

    GenerateLabel(startLabel);

    if(!node->CheckConditionAfter() && node->GetConditionExpression() != NULL)
    {
        node->GetConditionExpression()->Accept(this);
        IL::ILOperand condop = *(node->GetConditionExpression()->GetTag<IL::ILOperand>("ILOperand"));
        GenerateIL(IL::BrZ, condop, breakLabel);
    }

    if(node->GetBodyExpression() != NULL)
    {
        node->GetBodyExpression()->Accept(this);
    }


    GenerateLabel(contLabel);

    if(node->GetIncrementExpression() != NULL)
    {
        node->GetIncrementExpression()->Accept(this);
    }   
    
    if(node->CheckConditionAfter() && node->GetConditionExpression() != NULL)
    {
        node->GetConditionExpression()->Accept(this);
        IL::ILOperand condop = *(node->GetConditionExpression()->GetTag<IL::ILOperand>("ILOperand"));
        GenerateIL(IL::BrNz, condop, startLabel);
    }
    else
    {
        GenerateIL(IL::Br, startLabel);
    }

    GenerateLabel(breakLabel);

    _loopContext.pop();
}

void ILGenerator::Visit(MemberExpression* node)
{
    node->GetContainer()->Accept(this);

    Type *type = node->GetTag<Type>("Type");

    IL::ILOperand container = *(node->GetContainer()->GetTag<IL::ILOperand>("ILOperand"));
    IL::ILOperand field(IL::FieldToken, IL::Str, node->GetFieldName());

    IL::ILOperand target = GenerateTemp(type);

    GenerateIL(IL::Ldfld, target, container, field);

    node->SetTag<IL::ILOperand>("ILOperand", new IL::ILOperand(target));
}

void ILGenerator::Visit(VariableExpression* node)
{
    Type *type = node->GetTag<Type>("Type");

    // printf("debug 9\n");
    IL::ILOperand target = IL::ILOperand(IL::Variable, GetOperandType(type), node->GetSymbol());
    // printf("debug 10\n");
    node->SetTag<IL::ILOperand>("ILOperand", new IL::ILOperand(target));
    // printf("debug 11\n");
}

void ILGenerator::Visit(UnaryExpression* node)
{
    Expression *operand = node->GetOperand();

    if (node->GetAction() != Expression::Reference) {
      operand->Accept(this);
    } else {
      if (operand->GetLValue()->GetAction() != Expression::ArrayIndex) {
	operand->Accept(this);
      }
      else {
	// reference arrayIndex is special
      }
    }

    Expression::ExpressionAction action = node->GetAction();
    Type *type = node->GetTag<Type>("Type");
    Type *operandType = operand->GetTag<Type>("Type");
    IL::ILOpcode opcode;
    IL::ILOperand op1, op2, op3;

    switch(node->GetAction())
    {
        case Expression::Plus:
            op2 = *(operand->GetTag<IL::ILOperand>("ILOperand"));
            break;
        case Expression::Minus:
            opcode = IL::Neg;
            op1 = GenerateTemp(type);
            op2 = *(operand->GetTag<IL::ILOperand>("ILOperand"));
            GenerateIL(opcode, op1, op2);
            break;
        case Expression::Not:
            op1 = GenerateTemp(type);
            op2 = *(operand->GetTag<IL::ILOperand>("ILOperand"));
            opcode = IL::Not;
            GenerateIL(opcode, op1, op2);
            break;
        case Expression::Reference:
        {
            if(operand->GetLValue() == NULL)
            {
                assert(!"Invalid expression for reference expression - not lvalue.");
            }
            Expression *lvalue = operand->GetLValue();
            op1 = GenerateTemp(type);

            switch(lvalue->GetAction())
            {
                case Expression::Variable:
                    opcode = IL::Lda;
                    op2 = *(lvalue->GetTag<IL::ILOperand>("ILOperand"));
                    GenerateIL(opcode, op1, op2);
                    break;
                case Expression::Dereference:
                {
                    UnaryExpression *e = dynamic_cast<UnaryExpression *>(lvalue);
                    opcode = IL::Mov;
                    op2 = *(e->GetOperand()->GetTag<IL::ILOperand>("ILOperand"));
                    GenerateIL(opcode, op1, op2);
                    break;
                }
                case Expression::ArrayIndex:
                {
                    IndexExpression *e = dynamic_cast<IndexExpression *>(lvalue);
                    e->GetContainer()->Accept(this);
                    e->GetIndex()->Accept(this);
                    opcode = IL::Ldelema;
                    op2 = *(e->GetContainer()->GetTag<IL::ILOperand>("ILOperand"));
                    op3 = *(e->GetIndex()->GetTag<IL::ILOperand>("ILOperand"));
                    GenerateIL(opcode, op1, op2, op3);
                    break;
                }
                default:
                    abort();
            }
            break;
        }
        case Expression::Dereference:
            if(typeid(*operandType) == typeid(PointerType))
            {
                PointerType *t = dynamic_cast<PointerType *>(operandType);
                opcode = IL::Ldind;
                op1 = GenerateTemp(type);
                op2 = *(operand->GetTag<IL::ILOperand>("ILOperand"));
                GenerateIL(opcode, op1, op2);
            }
            else
            {
                assert(!"Invalid type of dereference expression.");
            }
            break;
        default:
            abort();
    }

    node->SetTag<IL::ILOperand>("ILOperand", new IL::ILOperand(op1));
}

void ILGenerator::Visit(ConvertExpression* node)
{
    Expression *source = node->GetSourceExpression();
    source->Accept(this);

    IL::ILOpcode opcode;
    IL::ILOperand op1, op2;
    op2 = *(source->GetTag<IL::ILOperand>("ILOperand"));

    Type *srcType = source->GetTag<Type>("Type");
    Type *tgtType = node->GetTargetType();

    bool srcIsPrimitive = true;
    bool tgtIsPrimitive = true;

    if(typeid(*srcType) == typeid(StructType))
    {
        srcIsPrimitive = false;
    }

    if(typeid(*tgtType) == typeid(StructType))
    {
        tgtIsPrimitive = false;
    }

    if(srcIsPrimitive && tgtIsPrimitive)
    {
        IL::ILOperandType srcILType = GetOperandType(srcType);
        IL::ILOperandType tgtILType = GetOperandType(tgtType);
        if(srcILType == tgtILType)
        {
            op1 = op2;
        }
        else
        {
            opcode = IL::Conv;

            op1 = GenerateTemp(tgtType);
            GenerateIL(opcode, op1, op2);
        }
    }
    else
    {
        opcode = IL::Cast;
        assert(!"Not implemented: class casting.");
        GenerateIL(opcode, op1, op2);
    }

    node->SetTag<IL::ILOperand>("ILOperand", new IL::ILOperand(op1));

}

void ILGenerator::Visit(ReturnExpression* node)
{
    if(node->GetValue() == NULL)
    {
        GenerateIL(IL::Ret);
    }
    else
    {
        node->GetValue()->Accept(this);
        IL::ILOperand operand = *(node->GetValue()->GetTag<IL::ILOperand>("ILOperand"));
        GenerateIL(IL::Ret, operand);
    }

}

void ILGenerator::Visit(FunctionExpression* node)
{
    _function = new ILFunction(_class, node->GetSymbol());
    _function->Scope = node->GetFunctionScope();
    _staticChain.push(_function->Scope);

    BlockExpression *body = node->GetBody();
    body->Accept(this);

    _staticChain.pop();
    _class->Functions.push_back(_function);
    _function = NULL;
}

void ILGenerator::Visit(ProgramExpression* node)
{

    _program = new ILProgram();

    // FIXME: Class type is not supported yet.
    Symbol *cs = new Symbol("Program", new VoidType());

    _class = new ILClass(_program, cs);

    _program->Claases.push_back(_class);
    _program->Scope = SymbolScope::GetRootScope();

    _staticChain.push(SymbolScope::GetRootScope());

    std::vector<Expression *> *functions = node->GetFunctions();
    for(std::vector<Expression *>::iterator it = functions->begin(); it != functions->end(); ++it)
    {
        Expression *expr = *it;
        expr->Accept(this);
    }

    _staticChain.pop();
}

void ILGenerator::Visit(CallExpression* node)
{
    bool indirectCall;
    std::vector<IL::ILOperand> oplist;

    node->GetTarget()->Accept(this);
    IL::ILOperand targetop = *(node->GetTarget()->GetTag<IL::ILOperand>("ILOperand"));

    assert(targetop.OperandKind == IL::Variable);

    Symbol *targetSymbol = targetop.SymRef->Lookup();
    assert(targetSymbol != NULL);

    Type *targetType = targetSymbol->DeclType;
    Type *returnType;
    if(typeid(*targetType) == typeid(FunctionType))
    {
        indirectCall = false;
        FunctionType *t = dynamic_cast<FunctionType *>(targetType);
        returnType = t->GetReturnType();
    }
    else if(typeid(*targetType) == typeid(PointerType))
    {
        indirectCall = true;
        PointerType *pt = dynamic_cast<PointerType *>(targetType);
        FunctionType *t = dynamic_cast<FunctionType *>(pt->GetUnderlyingType());
        assert(t != NULL);
        returnType = t->GetReturnType();
    }

    oplist.push_back(targetop);

    IL::ILOperand retop;
    if(typeid(*returnType) == typeid(VoidType))
    {
        retop = IL::ILOperand(IL::Empty, IL::Void, (intmax_t)0);
    }
    else
    {
        retop = GenerateTemp(returnType);

    }
    oplist.push_back(retop);
    std::vector<Expression *> *argList = node->GetArgumentList();
    for(std::vector<Expression *>::iterator it = argList->begin(); it != argList->end(); ++it)
    {
        Expression *arg = *it;
        arg->Accept(this);
        IL::ILOperand argop = *(arg->GetTag<IL::ILOperand>("ILOperand"));
        oplist.push_back(argop);
    }

    if(indirectCall)
    {
        GenerateIL(IL::CallInd, oplist);
    }
    else
    {
        GenerateIL(IL::Call, oplist);
    }
    node->SetTag<IL::ILOperand>("ILOperand", new IL::ILOperand(retop));
}

void ILGenerator::Visit(NopExpression* node)
{
    //GenerateIL(IL::Nop);
}

void ILGenerator::Visit(AssignExpression* node)
{
    //node->GetTarget()->Accept(this);
    node->GetSource()->Accept(this);

    Type *type = node->GetTag<Type>("Type");
    Expression *lvalue = node->GetTarget()->GetLValue();
    IL::ILOpcode opcode;
    IL::ILOperand op1, op2, op3;
    op2 = *(node->GetSource()->GetTag<IL::ILOperand>("ILOperand"));

    switch(lvalue->GetAction())
    {
        case Expression::Variable:
        {
            VariableExpression *e = dynamic_cast<VariableExpression *>(lvalue);
            opcode = IL::Mov;
            op1 = IL::ILOperand(IL::Variable, GetOperandType(type), e->GetSymbol());
            GenerateIL(opcode, op1, op2);
            break;
        }
        case Expression::Dereference:
        {
            UnaryExpression *e = dynamic_cast<UnaryExpression *>(lvalue);
            e->GetOperand()->Accept(this);
            if(e->GetOperand()->GetTag<IL::ILOperand>("ILOperand") == NULL)
            {
                e->GetOperand()->Accept(this);
            }
            opcode = IL::Stind;
            op1 = *(e->GetOperand()->GetTag<IL::ILOperand>("ILOperand"));
            GenerateIL(opcode, op1, op2);
            break;
        }
        case Expression::ArrayIndex:
        {
            IndexExpression *e = dynamic_cast<IndexExpression *>(lvalue);

            e->GetContainer()->Accept(this);
            e->GetIndex()->Accept(this);

            if(e->GetContainer()->GetTag<IL::ILOperand>("ILOperand") == NULL)
            {
                e->GetContainer()->Accept(this);
            }

            if(e->GetIndex()->GetTag<IL::ILOperand>("ILOperand") == NULL)
            {
                e->GetIndex()->Accept(this);
            }

            op1 = *(e->GetContainer()->GetTag<IL::ILOperand>("ILOperand"));
            op3 = *(e->GetIndex()->GetTag<IL::ILOperand>("ILOperand"));

            opcode = IL::Stelem;
            GenerateIL(opcode, op1, op3, op2);
            break;
        }
        case Expression::Member:
        {
            MemberExpression *e = dynamic_cast<MemberExpression *>(lvalue);
            e->Container->Accept(this);
            opcode = IL::Stfld;
            op1 = *(e->GetContainer()->GetTag<IL::ILOperand>("ILOperand"));
            op3 = IL::ILOperand(IL::FieldToken, IL::Str, e->FieldName);
            GenerateIL(opcode, op1, op3, op2);
            break;
        }
        default:
            abort();
    }
}

void ILGenerator::Visit(ArraySegmentExpression* node)
{
    node->GetArray()->Accept(this);
    node->GetStartIndex()->Accept(this);
    node->GetEndIndex()->Accept(this);

    IL::ILOperand arrop = *(node->GetArray()->GetTag<IL::ILOperand>("ILOperand"));

    node->SetTag<IL::ILOperand>("ILOperand", new IL::ILOperand(arrop));
}

void ILGenerator::Visit(NewRunnerExpression* node)
{
    // NOTE: Step 1. Fill in the metadata
    /*
     * 0x200001000:     Stack base
     * 0x200001008:     Stack size
     * 0x200001010:     number of heap ranges
     * 0x200001018:     heap base 1
     *                  heap size 1
     *                  heap base 2
     *                  heap size 2
     *                  ...
     *                  heap base N
     *                  heap size N
     * 0x2000XXXXX:     number of watch ranges
     *                  watch base 1
     *                  watch size 1
     *                  watch base 2
     *                  watch size 2
     *                  ...
     *                  watch base M
     *                  watch size M
     * 0x20000XXXXX:    FI
     *
     */

    long long  p = NEWR_METADATA_ADDR;
    long long pstack, pheap, pwatch, pfi, pin;

    pstack = p;
    char buffer[1024];

    IL::ILOperand sb = GenerateTemp(new PointerType(new VoidType));
    IL::ILOperand ss = GenerateTemp(new PointerType(new VoidType));
    IL::ILOperand scursor = GenerateTemp(new PointerType(new VoidType));

    // Stack: automatic allocation
    GenerateIL(IL::Asm, IL::ILOperand(IL::Constant, IL::Str, "mov $0x0:msq, 0x200001000:msq"));
    GenerateIL(IL::Asm, IL::ILOperand(IL::Constant, IL::Str, "mov $0x0:msq, 0x200001008:msq"));
    p += 0x10;

    pheap = p;

    // Heap
    std::vector<Expression *> *usingList = node->GetUsingList();
    sprintf(buffer, "mov $0x%llx:msq, 0x%llx:msq", (long long)(usingList->size()), p);
    GenerateIL(IL::Asm, IL::ILOperand(IL::Constant, IL::Str, buffer));
    p += 0x08;

    for(std::vector<Expression *>::iterator it = usingList->begin(); it != usingList->end(); ++it)
    {
        Expression *expr = *it;

        IL::ILOperand rangeStart, rangeSize;
        rangeStart = GenerateTemp(new PointerType(new VoidType()));
        rangeSize = GenerateTemp(new PointerType(new VoidType()));

        if(expr->GetAction() == Expression::ArraySegment)
        {

            ArraySegmentExpression *e = dynamic_cast<ArraySegmentExpression *>(expr);
            e->GetArray()->Accept(this);
            e->GetStartIndex()->Accept(this);
            e->GetEndIndex()->Accept(this);
            IL::ILOperand exprop = *(e->GetArray()->GetTag<IL::ILOperand>("ILOperand"));
            IL::ILOperand startop = *(e->GetStartIndex()->GetTag<IL::ILOperand>("ILOperand"));
            IL::ILOperand endop = *(e->GetEndIndex()->GetTag<IL::ILOperand>("ILOperand"));

            GenerateIL(IL::Ldelema, rangeStart, exprop, startop);
            GenerateIL(IL::Sub, rangeSize, endop, startop);

            int64_t elementSize;
            Type *type = e->GetArray()->GetTag<Type>("Type");
            if(typeid(*type) == typeid(PointerType))
            {
                PointerType *t = dynamic_cast<PointerType *>(type);
                elementSize = t->GetUnderlyingType()->GetSize();
            }
            else if(typeid(*type) == typeid(ArrayType))
            {
                ArrayType *t = dynamic_cast<ArrayType *>(type);
                elementSize = t->GetElementType()->GetSize();
            }

            GenerateIL(IL::Mul, rangeSize, IL::ILOperand(IL::Constant, IL::I, elementSize), rangeSize);
        }
        else if(expr->GetAction() == Expression::Variable)
        {

            VariableExpression *e = dynamic_cast<VariableExpression *>(expr);
            e->Accept(this);
            IL::ILOperand exprop = *(e->GetTag<IL::ILOperand>("ILOperand"));
            GenerateIL(IL::Lda, rangeStart, exprop);
            GenerateIL(IL::Mov, rangeSize, IL::ILOperand(IL::Constant, IL::I, e->GetSymbol()->DeclType->GetSize()));
        }
        else if(expr->GetAction() == Expression::ArrayIndex)
        {
            IndexExpression *e = dynamic_cast<IndexExpression *>(expr);
            e->GetContainer()->Accept(this);
            e->GetIndex()->Accept(this);
            IL::ILOperand exprop = *(e->GetContainer()->GetTag<IL::ILOperand>("ILOperand"));
            IL::ILOperand indexop = *(e->GetIndex()->GetTag<IL::ILOperand>("ILOperand"));
            int64_t elementSize;
            Type *type = e->GetContainer()->GetTag<Type>("Type");
            if(typeid(*type) == typeid(PointerType))
            {
                PointerType *t = dynamic_cast<PointerType *>(type);
                elementSize = t->GetUnderlyingType()->GetSize();
            }
            else if(typeid(*type) == typeid(ArrayType))
            {
                ArrayType *t = dynamic_cast<ArrayType *>(type);
                elementSize = t->GetElementType()->GetSize();
            }
            GenerateIL(IL::Ldelema, rangeStart, exprop, indexop);
            GenerateIL(IL::Mov, rangeSize, IL::ILOperand(IL::Constant, IL::I, elementSize));
        }
        else
        {
            abort();
        }

        GenerateIL(IL::Stind, IL::ILOperand(IL::Constant, IL::I, (intmax_t)p), rangeStart);
        p += 0x08;
        GenerateIL(IL::Stind, IL::ILOperand(IL::Constant, IL::I, (intmax_t)p), rangeSize);
        p += 0x08;

    }

    pwatch = p;

    // Watch
    std::vector<Expression *> *watchList = node->GetWatchingList();
    sprintf(buffer, "mov $0x%llx:msq, 0x%llx:msq", (long long)(watchList->size()), p);
    GenerateIL(IL::Asm, IL::ILOperand(IL::Constant, IL::Str, buffer));
    p += PointerType::PointerSize;

    for(std::vector<Expression *>::iterator it = watchList->begin(); it != watchList->end(); ++it)
    {
        Expression *expr = *it;

        IL::ILOperand rangeStart, rangeSize;
        rangeStart = GenerateTemp(new PointerType(new VoidType()));
        rangeSize = GenerateTemp(new PointerType(new VoidType()));


        if(expr->GetAction() == Expression::ArraySegment)
        {

            ArraySegmentExpression *e = dynamic_cast<ArraySegmentExpression *>(expr);
            e->GetArray()->Accept(this);
            e->GetStartIndex()->Accept(this);
            e->GetEndIndex()->Accept(this);
            IL::ILOperand exprop = *(e->GetArray()->GetTag<IL::ILOperand>("ILOperand"));

            IL::ILOperand startop = *(e->GetStartIndex()->GetTag<IL::ILOperand>("ILOperand"));
            IL::ILOperand endop = *(e->GetEndIndex()->GetTag<IL::ILOperand>("ILOperand"));
            GenerateIL(IL::Ldelema, rangeStart, exprop, startop);
            GenerateIL(IL::Sub, rangeSize, endop, startop);

            int64_t elementSize;
            Type *type = e->GetArray()->GetTag<Type>("Type");
            if(typeid(*type) == typeid(PointerType))
            {
                PointerType *t = dynamic_cast<PointerType *>(type);
                elementSize = t->GetUnderlyingType()->GetSize();
            }
            else if(typeid(*type) == typeid(ArrayType))
            {
                ArrayType *t = dynamic_cast<ArrayType *>(type);
                elementSize = t->GetElementType()->GetSize();
            }

            GenerateIL(IL::Mul, rangeSize, IL::ILOperand(IL::Constant, IL::I, elementSize), rangeSize);
        }
        else if(expr->GetAction() == Expression::Variable)
        {

            VariableExpression *e = dynamic_cast<VariableExpression *>(expr);
            e->Accept(this);
            IL::ILOperand exprop = *(e->GetTag<IL::ILOperand>("ILOperand"));
            GenerateIL(IL::Lda, rangeStart, exprop);
            GenerateIL(IL::Mov, rangeSize, IL::ILOperand(IL::Constant, IL::I, e->GetSymbol()->DeclType->GetSize()));
        }
        else if(expr->GetAction() == Expression::ArrayIndex)
        {
            IndexExpression *e = dynamic_cast<IndexExpression *>(expr);
            e->GetContainer()->Accept(this);
            e->GetIndex()->Accept(this);
            IL::ILOperand exprop = *(e->GetContainer()->GetTag<IL::ILOperand>("ILOperand"));
            IL::ILOperand indexop = *(e->GetIndex()->GetTag<IL::ILOperand>("ILOperand"));
            int64_t elementSize;
            Type *type = e->GetContainer()->GetTag<Type>("Type");
            if(typeid(*type) == typeid(PointerType))
            {
                PointerType *t = dynamic_cast<PointerType *>(type);
                elementSize = t->GetUnderlyingType()->GetSize();
            }
            else if(typeid(*type) == typeid(ArrayType))
            {
                ArrayType *t = dynamic_cast<ArrayType *>(type);
                elementSize = t->GetElementType()->GetSize();
            }
            GenerateIL(IL::Ldelema, rangeStart, exprop, indexop);
            GenerateIL(IL::Mov, rangeSize, IL::ILOperand(IL::Constant, IL::I, elementSize));
        }
        else
        {
            abort();
        }

        GenerateIL(IL::Stind, IL::ILOperand(IL::Constant, IL::I, (intmax_t)p), rangeStart);
        p += 0x08;
        GenerateIL(IL::Stind, IL::ILOperand(IL::Constant, IL::I, (intmax_t)p), rangeSize);
        p += 0x08;
    }


    // fi
    pfi = p;

    node->GetTarget()->Accept(this);

    sprintf(buffer, "mov $%s:msq, 0x%llx:msq", "__newr0", p);
    GenerateIL(IL::Asm, IL::ILOperand(IL::Constant, IL::Str, buffer));
    p += PointerType::PointerSize;

    // TODO: working here
    // in
    pin = p;

    std::vector<Expression *> *in = node->GetSpace();
    if (in->size() > 0) {
        // only 1
        if (in->size() > 1) {
            abort();
        }

        for(std::vector<Expression *>::iterator it = in->begin(); it != in->end(); ++it)
        {
            Expression *expr = *it;

            // printf("debug 1\n");

            if(expr->GetAction() == Expression::Variable)
            {
                // printf("debug 2\n");

                VariableExpression *e = dynamic_cast<VariableExpression *>(expr);
                // printf("debug 3\n");
                e->Accept(this);
                // printf("debug 4\n");
                IL::ILOperand exprop = *(e->GetTag<IL::ILOperand>("ILOperand"));
                // printf("debug 5\n");
                IL::ILOperand tmp;
                tmp = GenerateTemp(new PointerType(new VoidType()));
                // printf("debug 6\n");
                // GenerateIL(IL::Lda, tmp, exprop);
                GenerateIL(IL::Mov, tmp, exprop);
                // printf("debug 7\n");
                GenerateIL(IL::Stind, IL::ILOperand(IL::Constant, IL::I, (intmax_t)p), tmp);
                // printf("debug 8\n");
                p += PointerType::PointerSize;
            } else {
                printf("Space is not a variable.\n");
                abort();
            }
        }
    }

    // NOTE: Step 2. generate newr instruction

    int64_t pNewrArgs = p;
    sprintf(buffer, "mov $0x%llx:msq, 0x%llx:msq", pstack, p);
    GenerateIL(IL::Asm, IL::ILOperand(IL::Constant, IL::Str, buffer));
    p += PointerType::PointerSize;
    
    sprintf(buffer, "mov $0x%llx:msq, 0x%llx:msq", pheap, p);
    GenerateIL(IL::Asm, IL::ILOperand(IL::Constant, IL::Str, buffer));
    p += PointerType::PointerSize;
    
    sprintf(buffer, "mov $0x%llx:msq, 0x%llx:msq", pwatch, p);
    GenerateIL(IL::Asm, IL::ILOperand(IL::Constant, IL::Str, buffer));
    p += PointerType::PointerSize;
    
    sprintf(buffer, "mov $0x%llx:msq, 0x%llx:msq", pfi, p);
    GenerateIL(IL::Asm, IL::ILOperand(IL::Constant, IL::Str, buffer));
    p += PointerType::PointerSize;

    if (in->size() > 0) {
        sprintf(buffer, "mov $0x%llx:msq, 0x%llx:msq", pin, p);
        GenerateIL(IL::Asm, IL::ILOperand(IL::Constant, IL::Str, buffer));
        p += PointerType::PointerSize;
        sprintf(buffer, "spawnx 0x%llx, 0x%llx, 0x%llx, 0x%llx, 0x%llx", 
                (long long)(pNewrArgs + PointerType::PointerSize * 0),
                (long long)(pNewrArgs + PointerType::PointerSize * 1),
                (long long)(pNewrArgs + PointerType::PointerSize * 2),
                (long long)(pNewrArgs + PointerType::PointerSize * 3),
                (long long)(pNewrArgs + PointerType::PointerSize * 4)
               );

    } else {
        // no space specified
        sprintf(buffer, "newr 0x%llx, 0x%llx, 0x%llx, 0x%llx", 
                (long long)(pNewrArgs + PointerType::PointerSize * 0),
                (long long)(pNewrArgs + PointerType::PointerSize * 1),
                (long long)(pNewrArgs + PointerType::PointerSize * 2),
                (long long)(pNewrArgs + PointerType::PointerSize * 3)
               );
    } 

    GenerateIL(IL::Asm, IL::ILOperand(IL::Constant, IL::Str, buffer));

    // NOTE: Step 3. get the allocated stack location


    GenerateIL(IL::Ldind, sb, IL::ILOperand(IL::Constant, IL::I, 0x100000400));
    GenerateIL(IL::Ldind, ss, IL::ILOperand(IL::Constant, IL::I, 0x100000408));
    GenerateIL(IL::Add, scursor, sb, ss);

    // NOTE: Step 4. pass the arguments
    std::vector<Expression *> *argList = node->GetArgumentList();

    int64_t argSize = 0;
    for(std::vector<Expression *>::reverse_iterator it = argList->rbegin(); it != argList->rend(); ++it)
    {
        Expression *expr = *it;
        argSize += ROUND_UP(expr->GetTag<Type>("Type")->GetSize(), PointerType::PointerSize);
    }

    IL::ILOperand targetop = *(node->GetTarget()->GetTag<IL::ILOperand>("ILOperand"));
    assert(targetop.OperandKind == IL::Variable);
    
    IL::ILOperand targetaddr = GenerateTemp(new PointerType(new VoidType));
    GenerateIL(IL::Lda, targetaddr, targetop);
    GenerateIL(IL::Sub, scursor, scursor, IL::ILOperand(IL::Constant, IL::I8, (intmax_t)PointerType::PointerSize));
    GenerateIL(IL::Stind, scursor, targetaddr);

    GenerateIL(IL::Sub, scursor, scursor, IL::ILOperand(IL::Constant, IL::I8, (intmax_t)PointerType::PointerSize));
    GenerateIL(IL::Stind, scursor, IL::ILOperand(IL::Constant, IL::I8, ROUND_UP(argSize, PointerType::PointerSize)));
    
    for(std::vector<Expression *>::reverse_iterator it = argList->rbegin(); it != argList->rend(); ++it)
    {
        Expression *expr = *it;
        expr->Accept(this);

        IL::ILOperand argop = *(expr->GetTag<IL::ILOperand>("ILOperand"));

        GenerateIL(IL::Sub, scursor, scursor, IL::ILOperand(IL::Constant, IL::I8, ROUND_UP(expr->GetTag<Type>("Type")->GetSize(), PointerType::PointerSize)));
        GenerateIL(IL::Stind, scursor, argop);
    }

}

void ILGenerator::Visit(CommitExpression* node)
{
    bool delWatcher = node->GetDeleteWatcher();
    if(delWatcher)
    {
        GenerateIL(IL::Asm, IL::ILOperand(IL::Constant, IL::Str, "exit:cd"));
    }
    else
    {
        GenerateIL(IL::Asm, IL::ILOperand(IL::Constant, IL::Str, "exit:c"));
    }
}

void ILGenerator::Visit(AbortExpression* node)
{
    bool delWatcher = node->GetDeleteWatcher();
    if(delWatcher)
    {
        GenerateIL(IL::Asm, IL::ILOperand(IL::Constant, IL::Str, "exit:ad"));
    }
    else
    {
        GenerateIL(IL::Asm, IL::ILOperand(IL::Constant, IL::Str, "exit:a"));
    }
}

void ILGenerator::Visit(InlineAssemblyExpression* node)
{
    // TODO: Inline assembly parameters
    GenerateIL(IL::Asm, IL::ILOperand(IL::Constant, IL::Str, node->GetTemplateString()));
}

IL::ILOperand ILGenerator::GenerateTemp(Type* type)
{
    SymbolScope *curScope = _staticChain.top();
    Symbol *sym = NULL;
    while(sym == NULL)
    {
        char buffer[32];
        sprintf(buffer, "__temp_%d", _tempId++);
        std::string name = buffer;
        if(curScope->Lookup(name) == NULL)
        {
            sym = new Symbol(name, type);
            curScope->Add(sym);
        }
    }

    IL::ILOperand op ;
    op.OperandKind = IL::Variable;
    op.OperandType = GetOperandType(type);
    op.SymRef = new SymbolRef(sym);

    return op;
}

void ILGenerator::GenerateLabel(IL::ILOperand labelOp)
{
    GenerateIL(IL::Label, labelOp);
}


void ILGenerator::GenerateIL(IL::ILOpcode opcode)
{
    IL il(opcode);
    _function->Body.push_back(il);
}

void ILGenerator::GenerateIL(IL::ILOpcode opcode, const IL::ILOperand& operand1)
{
    IL il(opcode, operand1);
    _function->Body.push_back(il);
}

void ILGenerator::GenerateIL(IL::ILOpcode opcode, const IL::ILOperand& operand1, const IL::ILOperand& operand2)
{
    IL il(opcode, operand1, operand2);
    _function->Body.push_back(il);
}

void ILGenerator::GenerateIL(IL::ILOpcode opcode, const IL::ILOperand& operand1, const IL::ILOperand& operand2, const IL::ILOperand& operand3)
{
    IL il(opcode, operand1, operand2, operand3);
    _function->Body.push_back(il);
}

void ILGenerator::GenerateIL(IL::ILOpcode opcode, const IL::ILOperand& operand1, const IL::ILOperand& operand2, const IL::ILOperand& operand3, const IL::ILOperand& operand4)
{
    IL il(opcode, operand1, operand2, operand3, operand4);
    _function->Body.push_back(il);
}

void ILGenerator::GenerateIL(IL::ILOpcode opcode, const std::vector< IL::ILOperand >& operands)
{
    IL il(opcode, operands);
    _function->Body.push_back(il);

}

IL::ILOperandType ILGenerator::GetOperandType(Type* type)
{
    IL::ILOperandType optype;

    if(typeid(*type) == typeid(IntegerType))
    {
        IntegerType *t = dynamic_cast<IntegerType *>(type);
        assert(t != NULL);

        if(t->IsSigned())
        {
            switch(t->GetSize())
            {
                case 1:
                    optype = IL::I1;
                    break;
                case 2:
                    optype = IL::I2;
                    break;
                case 4:
                    optype = IL::I4;
                    break;
                case 8:
                    optype = IL::I8;
                    break;
                default:
                    assert(!"Unsupported signed integer type.");
            }
        }
        else
        {
            switch(t->GetSize())
            {
                case 1:
                    optype = IL::U1;
                    break;
                case 2:
                    optype = IL::U2;
                    break;
                case 4:
                    optype = IL::U4;
                    break;
                case 8:
                    optype = IL::U8;
                    break;
                default:
                    assert(!"Unsupported unsigned integer type.");
            }
        }
    }
    else if(typeid(*type) == typeid(BooleanType))
    {
        optype = IL::I;
    }
    else if(typeid(*type) == typeid(FloatingPointType))
    {
        FloatingPointType *t = dynamic_cast<FloatingPointType *>(type);
        assert(t != NULL);
        switch(t->GetSize())
        {
            case 4:
                optype = IL::R4;
                break;
            case 8:
                optype = IL::R8;
                break;
            default:
                assert(!"Unsupported floating-point type.");
        }
    }
    else if(typeid(*type) == typeid(PointerType) || typeid(*type) == typeid(ArrayType))
    {
        optype = IL::I;
    }
    else if(typeid(*type) == typeid(FunctionType) || typeid(*type) == typeid(LabelType))
    {
        optype = IL::I;
    }
    else if (typeid(*type) == typeid(StructType))
    {
        optype = IL::Struct;
    }
    else
    {
        assert(!"Unsupported data type for IL operand.");
    }

    return optype;
}




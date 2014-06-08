#include "TypeDeduction.h"
#include <core/Core.h>
#include <stdlib.h>
#include <assert.h>
#include <typeinfo>

void TypeDeduction::Visit(InlineAssemblyExpression* node)
{
    node->SetTag<Type>("Type", new VoidType());
}

void TypeDeduction::Visit(AbortExpression* node)
{
    node->SetTag<Type>("Type", new VoidType());
}

void TypeDeduction::Visit(CommitExpression* node)
{
    node->SetTag<Type>("Type", new VoidType());
}

void TypeDeduction::Visit(NewRunnerExpression* node)
{
    node->GetTarget()->Accept(this);
    std::vector<Expression *> *arguments = node->GetArgumentList();
    for (std::vector<Expression *>::iterator it = arguments->begin(); it != arguments->end(); ++it)
    {
        Expression *arg = *it;
        arg->Accept(this);
        //TODO: Check the parameter types, and do implicit conversion if needed
    }

    std::vector<Expression *> *usingRanges = node->GetUsingList();
    
    for (std::vector<Expression *>::iterator it = usingRanges->begin(); it != usingRanges->end(); ++it)
    {
        Expression *expr = *it;
        expr->Accept(this);
    }
    
    
    std::vector<Expression *> *watchingRanges = node->GetWatchingList();
    
    for (std::vector<Expression *>::iterator it = watchingRanges->begin(); it != watchingRanges->end(); ++it)
    {
        Expression *expr = *it;
        expr->Accept(this);
    }
 
    std::vector<Expression *> *space = node->GetSpace();
    
    for (std::vector<Expression *>::iterator it = space->begin(); it != space->end(); ++it)
    {
        Expression *expr = *it;
        expr->Accept(this);
    }

    FunctionType *funcType = dynamic_cast<FunctionType *>(node->GetTarget()->GetTag<Type>("Type"));
    if (funcType == NULL)
    {
        
        CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Requires a function type.");
    }

    if (!funcType->GetReturnType()->Equals(new VoidType()))
    {
        
        CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Runner function cannot have return value.");
    }
    node->SetTag<Type>("Type", funcType->GetReturnType());
}

void TypeDeduction::Visit(ArraySegmentExpression* node)
{
    node->GetArray()->Accept(this);
    node->GetStartIndex()->Accept(this);
    node->GetEndIndex()->Accept(this);
    node->SetTag<Type>("Type", node->GetArray()->GetTag<Type>("Type"));    
}

void TypeDeduction::Visit(AssignExpression* node)
{
    Expression *source = node->GetSource();
    Expression *target = node->GetTarget();

    source->Accept(this);
    target->Accept(this);

    Expression *lvalue = target->GetLValue();
    if (lvalue == NULL)
    {
        
        CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Assignment target is not an l-value.");
    }

    if (lvalue->GetTag<Type>("Type") == NULL)
    {
        lvalue->Accept(this);
    }


    Type *sourceType = source->GetTag<Type>("Type");
    Type *targetType = lvalue->GetTag<Type>("Type");

    if (!sourceType->CanImplicitlyConvertTo(targetType))
    {
        
        if (sourceType->CanExplicitlyConvertTo(targetType))
        {
            CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Cannot assign %s to %s, an explicit conversion is required.", sourceType->ToString().c_str(), targetType->ToString().c_str());
        }
        else
        {
            CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Cannot assign %s to %s.", sourceType->ToString().c_str(), targetType->ToString().c_str());
        }
    }
    else
    {
        if (!sourceType->Equals(targetType))
        {
            ConvertExpression *castExpr = new ConvertExpression(targetType, source);
            node->SetSource(castExpr);
            castExpr->SetTag<Type>("Type", targetType);
        }

        node->SetTag<Type>("Type", targetType);
    }
}

void TypeDeduction::Visit(NopExpression* node)
{
    node->SetTag<Type>("Type", new VoidType());
}

void TypeDeduction::Visit(CallExpression* node)
{
    node->GetTarget()->Accept(this);
    std::vector<Expression *> *arguments = node->GetArgumentList();
    for (std::vector<Expression *>::iterator it = arguments->begin(); it != arguments->end(); ++it)
    {
        Expression *arg = *it;
        arg->Accept(this);
        //TODO: Check the parameter types, and do implicit conversion if needed
    }

    FunctionType *funcType = dynamic_cast<FunctionType *>(node->GetTarget()->GetTag<Type>("Type"));
    if (funcType == NULL)
    {
        
        CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Requires a function type.");
    }

    node->SetTag<Type>("Type", funcType->GetReturnType());
}

void TypeDeduction::Visit(ProgramExpression* node)
{
    SymbolScope *oldScope = _currentScope;
    _currentScope = SymbolScope::GetRootScope();

    std::vector<Expression *> *functions = node->GetFunctions();
    for (std::vector<Expression *>::iterator it = functions->begin(); it != functions->end(); ++it)
    {
        Expression *func = *it;
        func->Accept(this);
    }
    _currentScope = oldScope;
}

void TypeDeduction::Visit(FunctionExpression* node)
{
    SymbolScope *oldScope = _currentScope;
    _currentScope = node->GetFunctionScope();
    node->GetBody()->Accept(this);
    _currentScope = oldScope;
}

void TypeDeduction::Visit(ReturnExpression* node)
{
    if (node->GetValue() != NULL)
    {
        node->GetValue()->Accept(this);
    }
    node->SetTag<Type>("Type", new VoidType());
}

void TypeDeduction::Visit(ConvertExpression* node)
{
    node->GetSourceExpression()->Accept(this);
    Type *sourceType = node->GetSourceExpression()->GetTag<Type>("Type");
    Type *targetType = node->GetTargetType();

    if (!sourceType->CanExplicitlyConvertTo(targetType))
    {
        
        CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Cannot convert %s to %s.", sourceType->ToString().c_str(), targetType->ToString().c_str());
    }

    node->SetTag<Type>("Type", targetType);
}


void TypeDeduction::Visit(UnaryExpression* node)
{
    Type *targetType;
    node->GetOperand()->Accept(this);
    Type *operandType = node->GetOperand()->GetTag<Type>("Type");
    assert(operandType != NULL);
    switch (node->GetAction())
    {
        case Expression::Plus:
        case Expression::Minus:
        case Expression::Not:
            //TODO: Check the type
            targetType = operandType;
            break;
        case Expression::Reference:
            if (node->GetOperand()->GetLValue() == NULL)
            {
                
                CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Only l-value can be get its address taken.");
            }
            targetType = new PointerType(operandType);
            break;
        case Expression::Dereference:
        {
            PointerType *pointerType = dynamic_cast<PointerType *>(operandType);
            if (pointerType == NULL)
            {
                
                CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Dereferencing requires a pointer type, but %s is provided.", operandType->ToString().c_str());
            }
            targetType = pointerType->GetUnderlyingType();
            break;
        }
        default:
            abort();
    }
    
    node->SetTag<Type>("Type", targetType);
}

void TypeDeduction::Visit(VariableExpression* node)
{
    node->SetTag<Type>("Type", node->GetSymbol()->DeclType);
}

void TypeDeduction::Visit(MemberExpression* node)
{
    //TODO: Finish it.
    node->GetContainer()->Accept(this);
    StructType *structType = dynamic_cast<StructType *>(node->GetContainer()->GetTag<Type>("Type"));
    if (structType == NULL)
    {
        
        CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Member expression requires struct type.");        
    }
    
    
    std::vector<Declaration *> * fileds = structType->GetFieldList();
    for (std::vector<Declaration *>::iterator it = fileds->begin(); it != fileds->end(); ++it)
    {
        Declaration *decl = *it;
        if (decl->GetName() == node->GetFieldName())
        {
            Type *fieldType = decl->GetType();
            node->SetTag<Type>("Type", fieldType);
            return;
        }
    }
    
    
    CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "%s is not a member of type %s", node->GetFieldName().c_str(), structType->ToString().c_str());        

    
    abort();
}

void TypeDeduction::Visit(LoopExpression* node)
{
    if (node->GetInitializingExpression() != NULL)
    {
        node->GetInitializingExpression()->Accept(this);
    }
    
    if (node->GetConditionExpression() != NULL)
    {
        node->GetConditionExpression()->Accept(this);
    }
    
    if (node->GetIncrementExpression() != NULL)
    {
        node->GetIncrementExpression()->Accept(this);
    }
    
    if (node->GetBodyExpression() != NULL)
    {
        node->GetBodyExpression()->Accept(this);
    }
    
    
    node->SetTag<Type>("Type", new VoidType());
}

void TypeDeduction::Visit(LabelExpression* node)
{

    if (_currentScope->Lookup(node->GetName()) == NULL)
    {
        Symbol *sym = new Symbol(node->GetName(), new LabelType());
        _currentScope->Add(sym);
    }
    node->SetTag<Type>("Type", new VoidType());
}

void TypeDeduction::Visit(IndexExpression* node)
{
    node->GetContainer()->Accept(this);
    node->GetIndex()->Accept(this);

    Type *containerType = node->GetContainer()->GetTag<Type>("Type");
    Type *indexType = node->GetIndex()->GetTag<Type>("Type");

    if (!indexType->CanImplicitlyConvertTo(new IntegerType(8,true)))
    {
        CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Index must be an integer, but %s is provided.", containerType->ToString().c_str());
    }
    
    if (typeid(*containerType) == typeid(ArrayType))
    {
        ArrayType *t= dynamic_cast<ArrayType *>(containerType);
        node->SetTag<Type>("Type", t->GetElementType());
    }
    else if (typeid(*containerType) == typeid(PointerType))
    {
        PointerType *t= dynamic_cast<PointerType *>(containerType);
        node->SetTag<Type>("Type", t->GetUnderlyingType());
    }
    else
    {
        CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Index expression requires an array or pointer type, but %s is provided.", containerType->ToString().c_str());
    }
}

void TypeDeduction::Visit(GotoExpression* node)
{
    node->SetTag<Type>("Type", new VoidType());
}

void TypeDeduction::Visit(ConstantExpression* node)
{
    //TODO: Finish it. auto deducing types of constant values
    Type *type = node->GetValue()->GetType();
    node->SetTag<Type>("Type", type );
}

void TypeDeduction::Visit(ConditionalExpression* node)
{
    std::map<Expression *, Expression *> *condBranches = node->GetConditionalBranches();
    for (std::map<Expression *, Expression *>::iterator it = condBranches->begin(); it != condBranches->end(); ++it)
    {
        Expression *condition = it->first;
        Expression *condExpr = it->second;
        condition->Accept(this);
        condExpr->Accept(this);
        Type *conditionType = condition->GetTag<Type>("Type");
        if (!conditionType->CanImplicitlyConvertTo(new BooleanType()))
        {
            CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Boolean expression expected.");
        }
    }
    node->GetDefaultBranch()->Accept(this);

    Type *defType = node->GetDefaultBranch()->GetTag<Type>("Type");
    for (std::map<Expression *, Expression *>::iterator it = condBranches->begin(); it != condBranches->end(); ++it)
    {
        Expression *condition = it->first;
        Expression *condExpr = it->second;
        Type *exprType = condExpr->GetTag<Type>("Type");
        if (!defType->Equals(exprType))
        {
            node->SetTag<Type>("Type", new VoidType());
            return;
        }
    }
    node->SetTag<Type>("Type", defType);
}

void TypeDeduction::Visit(BlockExpression* node)
{
    SymbolScope *oldScope = _currentScope;
    _currentScope = node->GetBlockScope();
    //TODO: Finish it.
    std::vector<Expression *> *exprList = node->GetExpressionList();
    for (std::vector<Expression *>::iterator it = exprList->begin(); it != exprList->end(); ++it)
    {
        Expression *expr = *it;
        expr->Accept(this);
    }

    if (exprList->size() > 0)
    {
        Type *lastExprType = exprList->back()->GetTag<Type>("Type");
        assert(lastExprType != NULL);
        node->SetTag<Type>("Type", lastExprType);
    }
    else
    {
        node->SetTag<Type>("Type", new VoidType());
    }
    _currentScope = oldScope;

}

void TypeDeduction::Visit(BinaryExpression* node)
{
    Type *leftType, *rightType;

    node->GetLeftOperand()->Accept(this);
    node->GetRightOperand()->Accept(this);

    leftType = node->GetLeftOperand()->GetTag<Type>("Type");
    rightType = node->GetRightOperand()->GetTag<Type>("Type");

    Type *targetType = NULL;

    switch (node->GetAction())
    {
        case Expression::Add:
        case Expression::Subtract:
        case Expression::Multiply:
        case Expression::Divide:
        case Expression::Modulo:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                if (typeid(*rightType) == typeid(IntegerType))
                {
                    IntegerType *leftIntType = dynamic_cast<IntegerType *>(leftType);
                    IntegerType *rightIntType = dynamic_cast<IntegerType *>(rightType);
                    targetType = new IntegerType(MAX(leftIntType->GetSize(), rightIntType->GetSize()), leftIntType->IsSigned() || rightIntType->IsSigned());
                    leftType = targetType;
                    rightType = targetType;
                }
                else if (typeid(*rightType) == typeid(FloatingPointType))
                {
                    targetType = rightType;
                    leftType = targetType;
                }
                else
                {
                    targetType = NULL;
                }
            }
            else if (typeid(*leftType) == typeid(FloatingPointType))
            {
                if (typeid(*rightType) == typeid(IntegerType))
                {
                    targetType = leftType;
                    rightType = targetType;
                }
                else if (typeid(*rightType) == typeid(FloatingPointType))
                {
                    FloatingPointType *lt = dynamic_cast<FloatingPointType *>(leftType);
                    FloatingPointType *rt = dynamic_cast<FloatingPointType *>(rightType);
                    targetType = new FloatingPointType(MAX(lt->GetSize(), rt->GetSize()));
                    leftType = targetType;
                    rightType = targetType;
                }
                else
                {
                    targetType = NULL;
                }
            }
            else if (typeid(*leftType) == typeid(PointerType))
            {
                if (typeid(*rightType) == typeid(IntegerType))
                {
                    targetType = leftType;
                }
                else
                {
                    targetType = NULL;
                }
            }
            else
            {
                targetType = NULL;
            }
            break;
        case Expression::LeftShift:
        case Expression::RightShift:
            if (typeid(*leftType) == typeid(IntegerType) && typeid(*rightType) == typeid(IntegerType))
            {
                targetType = leftType;
            }
            else
            {
                targetType = NULL;
            }
            break;
        case Expression::Equal:
        case Expression::NotEqual:
        case Expression::GreaterThan:
        case Expression::GreaterThanOrEqual:
        case Expression::LessThan:
        case Expression::LessThanOrEqual:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                if (typeid(*rightType) == typeid(IntegerType))
                {
                    IntegerType *leftIntType = dynamic_cast<IntegerType *>(leftType);
                    IntegerType *rightIntType = dynamic_cast<IntegerType *>(rightType);
                    leftType = new IntegerType(MAX(leftIntType->GetSize(), rightIntType->GetSize()), leftIntType->IsSigned() || rightIntType->IsSigned());;
                    targetType = new BooleanType();
                }
                else if (typeid(*rightType) == typeid(FloatingPointType))
                {
                    leftType = rightType;
                    targetType = new BooleanType();
                }
                else
                {
                    targetType = NULL;
                }
            }
            else if (typeid(*leftType) == typeid(FloatingPointType))
            {
                if (typeid(*rightType) == typeid(IntegerType))
                {
                    rightType = leftType;
                    targetType = new BooleanType();
                }
                else if (typeid(*rightType) == typeid(FloatingPointType))
                {
                    FloatingPointType *lt = dynamic_cast<FloatingPointType *>(leftType);
                    FloatingPointType *rt = dynamic_cast<FloatingPointType *>(rightType);
                    leftType = rightType = new FloatingPointType(MAX(lt->GetSize(), rt->GetSize()));
                    targetType = new BooleanType();
                }
                else
                {
                    targetType = NULL;
                }
            }
            else if (typeid(*leftType) == typeid(PointerType))
            {
                if (typeid(*rightType) == typeid(PointerType))
                {
                    targetType = new BooleanType();
                }
                else
                {
                    targetType = NULL;
                }
            }
            else
            {
                std::cout << "ERROR: TypeDeduction.cpp @558\n";
                exit(-1);
                targetType = NULL;
            }
            break;
        case Expression::And:
        case Expression::Or:
        case Expression::ExclusiveOr:
            if (typeid(*leftType) == typeid(IntegerType) && typeid(*rightType) == typeid(IntegerType))
            {
                IntegerType *leftIntType = dynamic_cast<IntegerType *>(leftType);
                IntegerType *rightIntType = dynamic_cast<IntegerType *>(rightType);
                targetType = new IntegerType(MAX(leftIntType->GetSize(), rightIntType->GetSize()), leftIntType->IsSigned() || rightIntType->IsSigned());
                leftType = targetType;
                rightType = targetType;
            }
            else if (typeid(*leftType) == typeid(BooleanType) && typeid(*rightType) == typeid(BooleanType))
            {
                targetType = new BooleanType();
            }
            else
            {
                targetType = NULL;
            }
            break;
        case Expression::AndAlso:
        case Expression::OrElse:
            if (typeid(*leftType) == typeid(BooleanType) && typeid(*rightType) == typeid(BooleanType))
            {
                targetType = new BooleanType();
            }
            else
            {
                targetType = NULL;
            }
            break;
        default:
            abort();
    }

    if (targetType == NULL)
    {
        
        CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Type mismatch.");
        return;
    }

    if (!leftType->Equals(node->GetLeftOperand()->GetTag<Type>("Type")))
    {
        node->SetLeftOperand(new ConvertExpression(leftType, node->GetLeftOperand()));
        node->GetLeftOperand()->SetTag<Type>("Type", leftType);
    }

    if (!rightType->Equals(node->GetRightOperand()->GetTag<Type>("Type")))
    {
        node->SetRightOperand(new ConvertExpression(rightType, node->GetRightOperand()));
        node->GetRightOperand()->SetTag<Type>("Type", rightType);
    }

    node->SetTag<Type>("Type", targetType);
}

TypeDeduction::TypeDeduction()
{
    _currentScope = NULL;
}

TypeDeduction::~TypeDeduction()
{
}

#include "ConstantPropagation.h"
#include <core/Core.h>
#include <stdlib.h>
#include <assert.h>
void ConstantPropagation::Visit(AbortExpression* node)
{
}

void ConstantPropagation::Visit(CommitExpression* node)
{
}

void ConstantPropagation::Visit(NewRunnerExpression* node)
{
}

void ConstantPropagation::Visit(ArraySegmentExpression* node)
{
}

void ConstantPropagation::Visit(AssignExpression* node)
{
    node->GetSource()->Accept(this);
    node->GetTarget()->Accept(this);
    
    node->SetSource(Propagate(node->GetSource()));
    node->SetTarget(Propagate(node->GetTarget()));
}

void ConstantPropagation::Visit(NopExpression* node)
{
}

void ConstantPropagation::Visit(CallExpression* node)
{
    //TODO: Constant functions
}

void ConstantPropagation::Visit(ProgramExpression* node)
{
    std::vector<Expression *> *functions = node->GetFunctions();
    for (std::vector<Expression *>::iterator it = functions->begin(); it != functions->end(); ++it)
    {
        Expression *func = *it;
        func->Accept(this);
    }
}

void ConstantPropagation::Visit(FunctionExpression* node)
{
    node->GetBody()->Accept(this);
}

void ConstantPropagation::Visit(ReturnExpression* node)
{
}
void ConstantPropagation::Visit(ConvertExpression* node)
{
    node->GetSourceExpression()->Accept(this);
    ConstantValue *v = GetConstant(node->GetSourceExpression());
    ConstantValue *new_v;
    Type *targetType = node->GetTargetType();
    if (v != NULL)
    {
        if (typeid(*targetType) == typeid(BooleanType)
                || typeid(*targetType) == typeid(IntegerType)
                || typeid(*targetType) == typeid(FloatingPointType)
                || typeid(*targetType) == typeid(PointerType)
           )
        {
            new_v = new ConstantValue(*v); // Default copy constructor is ok.
            new_v->SetType(node->GetTargetType());
            SetConstant(node, new_v);
        }
    }
    
}


void ConstantPropagation::Visit(UnaryExpression* node)
{
    node->Operand->Accept(this);
    node->Operand = Propagate(node->Operand);
    
    ConstantValue *operand = GetConstant(node);
    ConstantValue *evaluatedValue = NULL;

    if (operand != NULL)
    {
        Type *type = operand->GetType();
        switch (node->GetAction())
        {
            case Expression::Plus:
                if (typeid(*type) == typeid(IntegerType))
                {
                    IntegerType *itype = dynamic_cast<IntegerType *>(type);
                    if (itype->IsSigned())
                    {
                        evaluatedValue = new ConstantValue(operand->GetIntValue());
                    }
                    else
                    {
                        evaluatedValue = new ConstantValue(operand->GetUIntValue());
                    }
                }
                else if (typeid(*type) == typeid(FloatingPointType))
                {
                    evaluatedValue = new ConstantValue(operand->GetDoubleValue());
                }
                break;
            case Expression::Minus:
                if (typeid(*type) == typeid(IntegerType))
                {
                    IntegerType *itype = dynamic_cast<IntegerType *>(type);
                    if (itype->IsSigned())
                    {
                        evaluatedValue = new ConstantValue(-(operand->GetIntValue()));
                    }
                    else
                    {
                        evaluatedValue = new ConstantValue(-(operand->GetUIntValue()));
                    }

                }
                else if (typeid(*type) == typeid(FloatingPointType))
                {
                    evaluatedValue = new ConstantValue(-(operand->GetDoubleValue()));
                }
                break;
            case Expression::Not:
                if (typeid(*type) == typeid(BooleanType))
                {
                    evaluatedValue = new ConstantValue(!(operand->GetBoolValue()));
                }
                else if (typeid(*type) == typeid(IntegerType))
                {
                    IntegerType *itype = dynamic_cast<IntegerType *>(type);
                    if (itype->IsSigned())
                    {
                        evaluatedValue = new ConstantValue(~(operand->GetIntValue()));
                    }
                    else
                    {
                        evaluatedValue = new ConstantValue(~(operand->GetUIntValue()));
                    }
                }
                break;
            default:
                break;
        }
    }

    SetConstant(node, evaluatedValue);
}

void ConstantPropagation::Visit(VariableExpression* node)
{
}

void ConstantPropagation::Visit(MemberExpression* node)
{
    node->GetContainer()->Accept(this);
    node->Container = Propagate(node->Container);
    //TODO: Propagate
}

void ConstantPropagation::Visit(LoopExpression* node)
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
    
    
}

void ConstantPropagation::Visit(LabelExpression* node)
{
}

void ConstantPropagation::Visit(IndexExpression* node)
{
    node->GetContainer()->Accept(this);
    node->GetIndex()->Accept(this);
    
    node->Container = Propagate(node->Container);
    node->Index = Propagate(node->Index);
    
}

void ConstantPropagation::Visit(GotoExpression* node)
{
}

void ConstantPropagation::Visit(ConstantExpression* node)
{
    SetConstant(node, node->GetValue());
}

void ConstantPropagation::Visit(ConditionalExpression* node)
{
    for (std::map<Expression*, Expression *>::iterator it = node->GetConditionalBranches()->begin(); it != node->GetConditionalBranches()->end(); ++it)
    {
        it->first->Accept(this);
        it->second->Accept(this);
        //TODO
        
//         it->first = Propagate(it->first);
//         it->second = Propagate(it->second);
    }
    node->GetDefaultBranch()->Accept(this);
    node->SetDefaultBranch(Propagate(node->GetDefaultBranch()));
    
    //TODO: Sometimes the condition is a constant, if so evaluate to the conditional branch
}

void ConstantPropagation::Visit(BlockExpression* node)
{
    std::vector<Expression *> *exprList = node->GetExpressionList();
    for (std::vector<Expression *>::iterator it = exprList->begin(); it != exprList->end(); ++it)
    {
        Expression *expr = *it;
        expr->Accept(this);
    }
}

void ConstantPropagation::Visit(BinaryExpression* node)
{
    node->GetLeftOperand()->Accept(this);
    node->GetRightOperand()->Accept(this);

    ConstantValue *left = GetConstant(node->GetLeftOperand()); 
    ConstantValue *right = GetConstant(node->GetRightOperand()); 

    node->SetLeftOperand(Propagate(node->GetLeftOperand()));
    node->SetRightOperand(Propagate(node->GetRightOperand()));
    
    if ((left == NULL || right == NULL) && (node->GetAction() != Expression::AndAlso && node->GetAction() != Expression::OrElse))
    {
        return;
    }
    
    Type *leftType = NULL;
    Type *rightType = NULL;
    
    if (left != NULL)
    {
        leftType = left->GetType();
    }
    
    if (right != NULL)
    {
        rightType = right->GetType();
    }

    ConstantValue *evaluatedValue = NULL;

    switch (node->GetAction())
    {
        case Expression::Add:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                IntegerType *itype = dynamic_cast<IntegerType *>(leftType);
                assert(typeid(*rightType) == typeid(IntegerType));
                if (itype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetIntValue() + right->GetIntValue());
                }
                else
                {
                    evaluatedValue = new ConstantValue(left->GetUIntValue() + right->GetUIntValue());
                }
            }
            else if (typeid(*leftType) == typeid(FloatingPointType))
            {
                evaluatedValue = new ConstantValue(left->GetDoubleValue() + right->GetDoubleValue());
            }
            break;
        case Expression::Subtract:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                IntegerType *itype = dynamic_cast<IntegerType *>(leftType);
                assert(typeid(*rightType) == typeid(IntegerType));
                if (itype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetIntValue() - right->GetIntValue());
                }
                else
                {
                    evaluatedValue = new ConstantValue(left->GetUIntValue() - right->GetUIntValue());
                }
            }
            else if (typeid(*leftType) == typeid(FloatingPointType))
            {
                evaluatedValue = new ConstantValue(left->GetDoubleValue() - right->GetDoubleValue());
            }
            break;
        case Expression::Multiply:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                IntegerType *itype = dynamic_cast<IntegerType *>(leftType);
                assert(typeid(*rightType) == typeid(IntegerType));
                if (itype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetIntValue() * right->GetIntValue());
                }
                else
                {
                    evaluatedValue = new ConstantValue(left->GetUIntValue() * right->GetUIntValue());
                }
            }
            else if (typeid(*leftType) == typeid(FloatingPointType))
            {
                evaluatedValue = new ConstantValue(left->GetDoubleValue() * right->GetDoubleValue());
            }
            break;
        case Expression::Divide:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                IntegerType *itype = dynamic_cast<IntegerType *>(leftType);
                assert(typeid(*rightType) == typeid(IntegerType));
                if (itype->IsSigned())
                {
                    if (right->GetIntValue() == 0)
                    {
                        CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Divide by zero in constant epxression");
                    }
                    evaluatedValue = new ConstantValue(left->GetIntValue() / right->GetIntValue());
                }
                else
                {
                    if (right->GetIntValue() == 0)
                    {
                        CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Divide by zero in constant epxression");
                    }
                    evaluatedValue = new ConstantValue(left->GetUIntValue() / right->GetUIntValue());
                }
            }
            else if (typeid(*leftType) == typeid(FloatingPointType))
            {
                if (right->GetIntValue() == 0)
                {
                    CompilationContext::GetInstance()->ReportWarning(node->SourceLocation, "Divide by zero in floating point constant epxression");
                }
                evaluatedValue = new ConstantValue(left->GetDoubleValue() / right->GetDoubleValue());
            }
            break;
        case Expression::Modulo:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                IntegerType *itype = dynamic_cast<IntegerType *>(leftType);
                assert(typeid(*rightType) == typeid(IntegerType));
                if (itype->IsSigned())
                {
                    if (right->GetIntValue() == 0)
                    {
                        CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Divide by zero in constant epxression");
                    }
                    evaluatedValue = new ConstantValue(left->GetIntValue() % right->GetIntValue());
                }
                else
                {
                    if (right->GetIntValue() == 0)
                    {
                        CompilationContext::GetInstance()->ReportError(node->SourceLocation, false, "Divide by zero in constant epxression");
                    }
                    evaluatedValue = new ConstantValue(left->GetUIntValue() %  right->GetUIntValue());
                }
            }
  
            break;      
        case Expression::LeftShift:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                IntegerType *itype = dynamic_cast<IntegerType *>(leftType);
                IntegerType *rtype = dynamic_cast<IntegerType *>(rightType);
                assert(rightType != NULL);
                if (itype->IsSigned() && rtype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetIntValue() << right->GetIntValue());
                }
                else if (itype->IsSigned() && !rtype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetIntValue() << right->GetUIntValue());
                }
                else if (!itype->IsSigned() && rtype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetUIntValue() << right->GetIntValue());
                }
                else
                {
                    evaluatedValue = new ConstantValue(left->GetUIntValue() << right->GetUIntValue());
                }
            }
            break;
        case Expression::RightShift:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                IntegerType *itype = dynamic_cast<IntegerType *>(leftType);
                IntegerType *rtype = dynamic_cast<IntegerType *>(rightType);
                assert(rightType != NULL);
                if (itype->IsSigned() && rtype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetIntValue() >> right->GetIntValue());
                }
                else if (itype->IsSigned() && !rtype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetIntValue() >> right->GetUIntValue());
                }
                else if (!itype->IsSigned() && rtype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetUIntValue() >> right->GetIntValue());
                }
                else
                {
                    evaluatedValue = new ConstantValue(left->GetUIntValue() >> right->GetUIntValue());
                }
            }
            break;
        case Expression::And:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                IntegerType *itype = dynamic_cast<IntegerType *>(leftType);
                assert(typeid(*rightType) == typeid(IntegerType));
                if (itype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetIntValue() & right->GetIntValue());
                }
                else
                {
                    evaluatedValue = new ConstantValue(left->GetUIntValue() & right->GetUIntValue());
                }
            }
            else if (typeid(*leftType) == typeid(BooleanType))
            {
                evaluatedValue = new ConstantValue(left->GetBoolValue() && right->GetBoolValue());
            }
            break;
        case Expression::Or:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                IntegerType *itype = dynamic_cast<IntegerType *>(leftType);
                assert(typeid(*rightType) == typeid(IntegerType));
                if (itype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetIntValue() | right->GetIntValue());
                }
                else
                {
                    evaluatedValue = new ConstantValue(left->GetUIntValue() | right->GetUIntValue());
                }
            }
            else if (typeid(*leftType) == typeid(BooleanType))
            {
                evaluatedValue = new ConstantValue(left->GetBoolValue() || right->GetBoolValue());
            }
            break;
        case Expression::ExclusiveOr:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                IntegerType *itype = dynamic_cast<IntegerType *>(leftType);
                assert(typeid(*rightType) == typeid(IntegerType));
                if (itype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetIntValue() ^ right->GetIntValue());
                }
                else
                {
                    evaluatedValue = new ConstantValue(left->GetUIntValue() ^ right->GetUIntValue());
                }
            }
            else if (typeid(*leftType) == typeid(BooleanType))
            {
                evaluatedValue = new ConstantValue((left->GetBoolValue() && !right->GetBoolValue()) || (!left->GetBoolValue() && right->GetBoolValue()));
            }
            break;
        case Expression::Equal:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                IntegerType *itype = dynamic_cast<IntegerType *>(leftType);
                assert(typeid(*rightType) == typeid(IntegerType));
                if (itype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetIntValue() == right->GetIntValue());
                }
                else
                {
                    evaluatedValue = new ConstantValue(left->GetUIntValue() == right->GetUIntValue());
                }
            }
            else if (typeid(*leftType) == typeid(FloatingPointType))
            {
                evaluatedValue = new ConstantValue(left->GetDoubleValue() == right->GetBoolValue());
            }
            else if (typeid(*leftType) == typeid(BooleanType))
            {
                evaluatedValue = new ConstantValue(left->GetDoubleValue() == right->GetBoolValue());
            }            
            break;
        case Expression::NotEqual:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                IntegerType *itype = dynamic_cast<IntegerType *>(leftType);
                assert(typeid(*rightType) == typeid(IntegerType));
                if (itype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetIntValue() != right->GetIntValue());
                }
                else
                {
                    evaluatedValue = new ConstantValue(left->GetUIntValue() != right->GetUIntValue());
                }
            }
            else if (typeid(*leftType) == typeid(FloatingPointType))
            {
                evaluatedValue = new ConstantValue(left->GetDoubleValue() != right->GetBoolValue());
            }
            else if (typeid(*leftType) == typeid(BooleanType))
            {
                evaluatedValue = new ConstantValue(left->GetDoubleValue() != right->GetBoolValue());
            }            
            break;
        case Expression::LessThan:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                IntegerType *itype = dynamic_cast<IntegerType *>(leftType);
                assert(typeid(*rightType) == typeid(IntegerType));
                if (itype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetIntValue() < right->GetIntValue());
                }
                else
                {
                    evaluatedValue = new ConstantValue(left->GetUIntValue() < right->GetUIntValue());
                }
            }
            else if (typeid(*leftType) == typeid(FloatingPointType))
            {
                evaluatedValue = new ConstantValue(left->GetDoubleValue() < right->GetBoolValue());
            }
            else if (typeid(*leftType) == typeid(BooleanType))
            {
                evaluatedValue = new ConstantValue(left->GetDoubleValue() < right->GetBoolValue());
            }            
            break;
        case Expression::LessThanOrEqual:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                IntegerType *itype = dynamic_cast<IntegerType *>(leftType);
                assert(typeid(*rightType) == typeid(IntegerType));
                if (itype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetIntValue() <= right->GetIntValue());
                }
                else
                {
                    evaluatedValue = new ConstantValue(left->GetUIntValue() <= right->GetUIntValue());
                }
            }
            else if (typeid(*leftType) == typeid(FloatingPointType))
            {
                evaluatedValue = new ConstantValue(left->GetDoubleValue() <= right->GetBoolValue());
            }
            else if (typeid(*leftType) == typeid(BooleanType))
            {
                evaluatedValue = new ConstantValue(left->GetDoubleValue() <= right->GetBoolValue());
            }            
            break;
        case Expression::GreaterThanOrEqual:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                IntegerType *itype = dynamic_cast<IntegerType *>(leftType);
                assert(typeid(*rightType) == typeid(IntegerType));
                if (itype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetIntValue() >= right->GetIntValue());
                }
                else
                {
                    evaluatedValue = new ConstantValue(left->GetUIntValue() >= right->GetUIntValue());
                }
            }
            else if (typeid(*leftType) == typeid(FloatingPointType))
            {
                evaluatedValue = new ConstantValue(left->GetDoubleValue() >= right->GetBoolValue());
            }
            else if (typeid(*leftType) == typeid(BooleanType))
            {
                evaluatedValue = new ConstantValue(left->GetDoubleValue() >= right->GetBoolValue());
            }            
            break;
        case Expression::GreaterThan:
            if (typeid(*leftType) == typeid(IntegerType))
            {
                IntegerType *itype = dynamic_cast<IntegerType *>(leftType);
                assert(typeid(*rightType) == typeid(IntegerType));
                if (itype->IsSigned())
                {
                    evaluatedValue = new ConstantValue(left->GetIntValue() > right->GetIntValue());
                }
                else
                {
                    evaluatedValue = new ConstantValue(left->GetUIntValue() > right->GetUIntValue());
                }
            }
            else if (typeid(*leftType) == typeid(FloatingPointType))
            {
                evaluatedValue = new ConstantValue(left->GetDoubleValue() > right->GetBoolValue());
            }
            else if (typeid(*leftType) == typeid(BooleanType))
            {
                evaluatedValue = new ConstantValue(left->GetDoubleValue() > right->GetBoolValue());
            }            
            break;
            break;
        case Expression::AndAlso:
            if (left == NULL)
            {
                evaluatedValue = NULL;
            }
            else if (!left->GetBoolValue())
            {
                evaluatedValue = new ConstantValue(false);
            }
            else
            {
                evaluatedValue = right;
            }
            break;
        case Expression::OrElse:
            if (left == NULL)
            {
                evaluatedValue = NULL;
            }
            else if (left->GetBoolValue())
            {
                evaluatedValue = new ConstantValue(true);
            }
            else
            {
                evaluatedValue = right;
            }
            break;
        default:
            abort();
    }

    SetConstant(node, evaluatedValue);
}


ConstantPropagation::ConstantPropagation()
{

}

ConstantPropagation::~ConstantPropagation()
{

}

ConstantValue* ConstantPropagation::Evaluate(Expression* expr)
{
    expr->Accept(this);
    return expr->GetTag<ConstantValue>("Constant");
}

void ConstantPropagation::Visit(InlineAssemblyExpression* node)
{
    
}

void ConstantPropagation::Reset()
{

}


ConstantValue* ConstantPropagation::GetConstant(Expression* e)
{
    return e->GetTag<ConstantValue>("Constant");
}

void ConstantPropagation::SetConstant(Expression* e, ConstantValue *v)
{
    e->SetTag<ConstantValue>("Constant", v);
}

Expression* ConstantPropagation::Propagate(Expression* e)
{
    ConstantValue *v = GetConstant(e);
    if (v == NULL)
    {
        return e;
    }
    else
    {
        Expression *newExpr = new ConstantExpression(v);
        SetConstant(newExpr, v);
        return newExpr;
    }
}



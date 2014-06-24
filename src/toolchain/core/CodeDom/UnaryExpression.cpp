
#include "UnaryExpression.h"
#include <stdlib.h>
#include "ExpressionVisitor.h"
#include <core/Type/PointerType.h>
#include <core/Type/BooleanType.h>

UnaryExpression::UnaryExpression(): Expression(Expression::Nop), Operand(NULL)
{
}

UnaryExpression::UnaryExpression(Expression::ExpressionAction nodeType, Expression* operand)
        : Expression(nodeType)
{
    this->Operand = operand;
}

UnaryExpression::~UnaryExpression()
{

}

void UnaryExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

Expression* UnaryExpression::GetLValue()
{
    switch (this->GetAction())
    {
        case Dereference:
            return this;
        default:
            return NULL;
    }
}

Expression* UnaryExpression::GetOperand()
{
    return Operand;
}

Type* UnaryExpression::GetType()
{
    switch (this->GetAction())
    {
        case Expression::Plus:
        case Expression::Minus:
            return Operand->GetType();
        case Expression::Not:
            return new BooleanType();
        case Expression::Reference:
            return new PointerType(Operand->GetType());
        case Expression::Dereference:
        {
            PointerType *pointerType = dynamic_cast<PointerType *>(Operand->GetType());
            return pointerType->GetUnderlyingType();
        }
        default:
            abort();
    }
}

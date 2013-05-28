
#include "NopExpression.h"
#include "ExpressionVisitor.h"
#include <core/Type/VoidType.h>

Type* NopExpression::GetType()
{
    return new VoidType();
}

Expression* NopExpression::GetLValue()
{
    return NULL;
}

void NopExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

NopExpression::NopExpression()
:Expression(Expression::Nop)
{
}

NopExpression::~NopExpression()
{

}


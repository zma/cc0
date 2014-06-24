
#include "ConstantExpression.h"
#include "ExpressionVisitor.h"
#include <stdlib.h>
#include <core/Type/PointerType.h>
#include <core/Misc/ConstantValue.h>
//TODO: Finish it

ConstantExpression::ConstantExpression():Expression(Expression::Constant), _value(NULL)
{
}

ConstantExpression::ConstantExpression(ConstantValue *value)
        : Expression(Expression::Constant)
{
    _value = value;
}

ConstantValue *ConstantExpression::GetValue()
{
    return _value;
}

ConstantExpression::~ConstantExpression()
{

}


Type* ConstantExpression::GetType()
{
    return _value->GetType();
}

Expression* ConstantExpression::GetLValue()
{
    return NULL;
}

void ConstantExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}


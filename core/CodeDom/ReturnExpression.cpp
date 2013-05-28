
#include "ReturnExpression.h"
#include "ExpressionVisitor.h"
#include <stdlib.h>
#include <core/Type/VoidType.h>

Type* ReturnExpression::GetType()
{
    if (_value != NULL)
    {
        return _value->GetType();
    }
    else
    {
        return new VoidType();
    }
}

Expression* ReturnExpression::GetLValue()
{
    return NULL;
}

void ReturnExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}


ReturnExpression::~ReturnExpression()
{

}
ReturnExpression::ReturnExpression(Expression* value)
        : Expression(Expression::Return)
{
    this->_value = value;
}

ReturnExpression::ReturnExpression()
        : Expression(Expression::Return)
{
    _value = NULL;
}

Expression* ReturnExpression::GetValue()
{
    return _value;
}




#include "ConvertExpression.h"
#include "ExpressionVisitor.h"

ConvertExpression::ConvertExpression():Expression(Expression::Convert), _targetType(NULL), _source(NULL)
{
}

ConvertExpression::ConvertExpression(Type* targetType, Expression* sourceExpression)
: Expression(Expression::Convert)
{
    _targetType = targetType;
    _source = sourceExpression;
}
Expression *ConvertExpression::GetSourceExpression()
{
    return _source;
}

Type* ConvertExpression::GetTargetType()
{
    return _targetType;
}


Type* ConvertExpression::GetType()
{
    return _targetType;
}

Expression* ConvertExpression::GetLValue()
{
    return NULL;
}

void ConvertExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

ConvertExpression::~ConvertExpression()
{

}


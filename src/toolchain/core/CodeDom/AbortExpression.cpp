#include "AbortExpression.h"
#include "ExpressionVisitor.h"
#include <core/Type/VoidType.h>

AbortExpression::AbortExpression():Expression(Expression::Abort),_deleteWatcher(false)
{
}

Type* AbortExpression::GetType()
{
    return new VoidType();
}

Expression* AbortExpression::GetLValue()
{
    return NULL;
}

void AbortExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

AbortExpression::AbortExpression(bool deleteWatcher)
:Expression(Expression::Abort)
{
    _deleteWatcher = deleteWatcher;
}

AbortExpression::~AbortExpression()
{

}

bool AbortExpression::GetDeleteWatcher()
{
    return _deleteWatcher;
}

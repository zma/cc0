#include "CommitExpression.h"
#include <core/Type/VoidType.h>
#include "ExpressionVisitor.h"

CommitExpression::CommitExpression():Expression(Expression::Commit), _deleteWatcher(false)
{
}

CommitExpression::CommitExpression(bool deleteWatcher)
    :Expression(Expression::Commit)
{
    _deleteWatcher = deleteWatcher;
}

CommitExpression::~CommitExpression()
{

}

void CommitExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}
Expression* CommitExpression::GetLValue()
{
    return NULL;
}

Type* CommitExpression::GetType()
{       
    return new VoidType();
}
bool CommitExpression::GetDeleteWatcher()
{
    return _deleteWatcher;
}

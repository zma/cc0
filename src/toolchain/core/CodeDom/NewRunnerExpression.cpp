#include "NewRunnerExpression.h"
#include "ExpressionVisitor.h"

NewRunnerExpression::NewRunnerExpression(): Expression(Expression::NewRunner), _target(NULL), _arguments(NULL), _usingList(NULL), _watchList(NULL), _space(NULL)
{
}

NewRunnerExpression::NewRunnerExpression(Expression* target, std::vector< Expression* >* arguments, std::vector< Expression* >* watchingList, std::vector< Expression* >* usingList, std::vector<Expression *> *space)
        : Expression(Expression::NewRunner)
{
    _target = target;
    _watchList = watchingList;
    _usingList = usingList;
    _arguments = arguments;
    _space = space;
}

NewRunnerExpression::~NewRunnerExpression()
{

}

Expression* NewRunnerExpression::GetLValue()
{
    return NULL;
}

void NewRunnerExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

std::vector< Expression* >* NewRunnerExpression::GetArgumentList()
{
    return _arguments;
}

Expression* NewRunnerExpression::GetTarget()
{
    return _target;
}

std::vector< Expression* >* NewRunnerExpression::GetWatchingList()
{
    return _watchList;
}

std::vector< Expression* >* NewRunnerExpression::GetUsingList()
{
    return _usingList;
}

std::vector<Expression *>* NewRunnerExpression::GetSpace()
{
    return _space;
}


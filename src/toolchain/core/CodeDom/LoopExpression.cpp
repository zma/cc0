#include "LoopExpression.h"
#include "ExpressionVisitor.h"
#include <core/Type/VoidType.h>

LoopExpression::LoopExpression():Expression(Expression::Loop), _initializingExpression(NULL), _conditionExpression(NULL), _incrementExpression(NULL), _bodyExpression(NULL), _checkCondAfter(false)
{
}

LoopExpression::LoopExpression(Expression* init, Expression* cond, Expression* incr, Expression* body, bool checkAfter)
    :Expression(Expression::Loop)
{
    _initializingExpression = init;
    _conditionExpression = cond;
    _incrementExpression = incr;
    _bodyExpression = body;
    _checkCondAfter = checkAfter;
}

LoopExpression::~LoopExpression()
{

}

Expression* LoopExpression::GetLValue()
{
    return NULL;
}

void LoopExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

Expression* LoopExpression::GetInitializingExpression()
{
    return _initializingExpression;
}

Expression* LoopExpression::GetConditionExpression()
{
    return _conditionExpression;
}

Expression* LoopExpression::GetIncrementExpression()
{
    return _incrementExpression;
}

Expression* LoopExpression::GetBodyExpression()
{
    return _bodyExpression;
}

bool LoopExpression::CheckConditionAfter()
{
    return _checkCondAfter;
}


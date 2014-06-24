
#include "ConditionalExpression.h"
#include "ExpressionVisitor.h"



ConditionalExpression::ConditionalExpression()
    :Expression(Expression::Conditional)
{
    _conditionalBranches = new std::map<Expression *, Expression *>(); //FIXME: memory leak!
    _defaultBranch = NULL;
    _goThrough = false;
}

ConditionalExpression::~ConditionalExpression()
{
    
}

Expression* ConditionalExpression::GetLValue()
{
    return NULL;
}

void ConditionalExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

std::map< Expression*, Expression* >* ConditionalExpression::GetConditionalBranches()
{
    return _conditionalBranches;
}

void ConditionalExpression::SetConditionalBranches(std::map< Expression*, Expression* >* branches)
{
    _conditionalBranches = branches;
}

Expression* ConditionalExpression::GetDefaultBranch()
{
    return _defaultBranch;
}

void ConditionalExpression::SetDefaultBranch(Expression* expr)
{
    _defaultBranch = expr;
}

bool ConditionalExpression::GoThrough()
{
    return _goThrough;
}
void ConditionalExpression::SetGoThrough(bool v)
{
    _goThrough = v;
}

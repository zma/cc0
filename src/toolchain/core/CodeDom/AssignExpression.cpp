#include "AssignExpression.h"
#include "ExpressionVisitor.h"

AssignExpression::AssignExpression():Expression(Expression::Assign),_additionOperator(Expression::Nop),_target(NULL), _source(NULL)
{
}

Type* AssignExpression::GetType()
{
    return _target->GetType();
}

Expression* AssignExpression::GetLValue()
{
    return NULL;
}

void AssignExpression::SetSource(Expression* source)
{
    _source = source;
}

void AssignExpression::SetTarget(Expression* target)
{
    _target = target;
}


void AssignExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

AssignExpression::AssignExpression(Expression* target, Expression* source, Expression::ExpressionAction additionalOp)
: Expression(Expression::Assign)
{
    _target = target;
    _source = source;
    _additionOperator = additionalOp;
}

AssignExpression::AssignExpression(Expression* target, Expression* source)
: Expression(Expression::Assign)
{
    _target = target;
    _source = source;
    _additionOperator = Expression::Nop;
}


AssignExpression::~AssignExpression()
{
    
}

Expression::ExpressionAction AssignExpression::GetAdditionalOperator()
{
    return _additionOperator;
}

Expression* AssignExpression::GetSource()
{
    return _source;
}

Expression* AssignExpression::GetTarget()
{
    return _target;
}


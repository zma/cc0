
#include "CallExpression.h"
#include "ExpressionVisitor.h"
#include <core/Type/FunctionType.h>

CallExpression::CallExpression():Expression(Expression::Call), _target(NULL)
{
}

CallExpression::CallExpression(Expression *target)
:Expression(Expression::Call)
{
    _target = target;
    _arguments.clear();
}

CallExpression::CallExpression(Expression *target, std::vector<Expression *> *arguments)
:Expression(Expression::Call)
{
    _target = target;
    _arguments = *arguments;
}
CallExpression::~CallExpression()
{

}

void CallExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

Expression* CallExpression::GetLValue()
{
    return NULL;
}

std::vector< Expression* >* CallExpression::GetArgumentList()
{
    return &_arguments;
}

Type* CallExpression::GetType()
{
    FunctionType *type = dynamic_cast<FunctionType *>(_target->GetType());
    return type->GetReturnType();
}

Expression* CallExpression::GetTarget()
{
    return _target;
}


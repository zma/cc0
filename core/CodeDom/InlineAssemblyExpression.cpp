#include "InlineAssemblyExpression.h"
#include <core/CodeDom/ExpressionVisitor.h>

Expression* InlineAssemblyExpression::GetLValue()
{
    return NULL;
}

void InlineAssemblyExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

InlineAssemblyExpression::InlineAssemblyExpression(std::string fmt, std::vector< Expression* >* args)
        : Expression(Expression::InlineAssembly)
{
    _fmt = fmt;
    _args = args;
}


InlineAssemblyExpression::InlineAssemblyExpression(std::string fmt)
        : Expression(Expression::InlineAssembly)
{
    _fmt = fmt;
    _args = new std::vector<Expression *>();
}


InlineAssemblyExpression::~InlineAssemblyExpression()
{

}

std::string InlineAssemblyExpression::GetTemplateString()
{
    return _fmt;
}

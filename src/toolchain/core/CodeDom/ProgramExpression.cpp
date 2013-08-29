
#include "ProgramExpression.h"
#include "ExpressionVisitor.h"

Type* ProgramExpression::GetType()
{
    return NULL;
}

Expression* ProgramExpression::GetLValue()
{
    return NULL;
}

void ProgramExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}


ProgramExpression::ProgramExpression()
        : Expression(Expression::Program)
{
}

ProgramExpression::~ProgramExpression()
{

}

std::vector<Expression* > *ProgramExpression::GetFunctions()
{
    return &(this->_functions);
}

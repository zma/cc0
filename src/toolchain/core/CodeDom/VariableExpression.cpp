
#include "VariableExpression.h"
#include "ExpressionVisitor.h"
#include <core/Symbol/Symbol.h>
#include "ConstantExpression.h"
#include <core/Misc/ConstantValue.h>

VariableExpression::VariableExpression(): Expression(Expression::Variable), _symbol(NULL)
{
}

VariableExpression::VariableExpression(Symbol* symbol)
: Expression(Expression::Variable)
{
    _symbol = symbol;
}
Symbol* VariableExpression::GetSymbol()
{
    //_symbol->GetDeclScope()->LookupLocal(_symbol->GetName());
    return _symbol;
}

Type* VariableExpression::GetType()
{
    return _symbol->DeclType;
}

Expression* VariableExpression::GetLValue()
{
    return this;
}

void VariableExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

VariableExpression::~VariableExpression()
{

}


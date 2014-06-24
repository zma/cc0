
#include "LabelExpression.h"
#include "ExpressionVisitor.h"
#include <core/Type/LabelType.h>
#include <core/Type/VoidType.h>
#include <core/Type/PointerType.h>
#include <core/Symbol/Symbol.h>
#include "ConstantExpression.h"
#include <core/Misc/ConstantValue.h>
#include <stdio.h>

unsigned int LabelExpression::_labelID = 0;

LabelExpression::LabelExpression():Expression(Expression::Label), _symbol(NULL)
{
}

Type* LabelExpression::GetType()
{
    //FIXME: It cannot be the evlautated type of a block
    return new LabelType();
}

Expression* LabelExpression::GetLValue()
{
    //TODO: Get the code address of _target (not the l-value address of _target)
    return new ConstantExpression(new ConstantValue(_symbol->Address));
}

void LabelExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

LabelExpression::LabelExpression(Symbol *symbol)
        : Expression(Expression::Label)
{
    if (symbol == NULL)
    {
        char buf[100];
        sprintf(buf, "__label_%u", _labelID);
        _symbol = new Symbol(buf, new LabelType(), Symbol::ObjectName);        
    }
    else
    {
        _symbol = symbol;
    }
}

std::string LabelExpression::GetName()
{
    return _symbol->Name;
}


LabelExpression::~LabelExpression()
{

}

Symbol* LabelExpression::GetSymbol()
{
    return _symbol;
}




#include "GotoExpression.h"
#include <stdlib.h>
#include "ExpressionVisitor.h"
#include <core/Type/VoidType.h>

GotoExpression::GotoExpression():Expression(Expression::Goto), _targetType(NamedLabel)
{
}

GotoExpression::GotoExpression(GotoExpression::GotoTargetType targetType)
        : Expression(Expression::Goto)
{
    this->_targetType = targetType;
}

GotoExpression::GotoExpression(std::string labelName)
        : Expression(Expression::Goto)
{
    this->_targetType = GotoExpression::NamedLabel;
    this->_labelName = labelName;
}

GotoExpression::~GotoExpression()
{
    
}

Type* GotoExpression::GetType()
{
    return new VoidType();
}


void GotoExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

Expression* GotoExpression::GetLValue()
{
    return NULL;
}

std::string GotoExpression::GetLabelName()
{
    return this->_labelName;
}

GotoExpression::GotoTargetType GotoExpression::GetTargetType()
{
    return this->_targetType;
}

Type * GetType()
{
    return new VoidType();
}



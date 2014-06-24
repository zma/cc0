
#include <stdlib.h>
#include "BinaryExpression.h"
#include "ExpressionVisitor.h"
#include <core/Type/ArrayType.h>
#include <core/Type/PointerType.h>

BinaryExpression::BinaryExpression():Expression(Expression::Nop), _left(NULL), _right(NULL)
{
}

BinaryExpression::BinaryExpression(ExpressionAction nodeType,  Expression *left, Expression *right)
        : Expression(nodeType)
{
    //TODO: Check the nodeType
    this->_left = left;
    this->_right = right;
}

BinaryExpression::~BinaryExpression()
{

}

void BinaryExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

Expression* BinaryExpression::GetLValue()
{
    return NULL;
}

Expression* BinaryExpression::GetLeftOperand()
{
    return _left;
}

Expression* BinaryExpression::GetRightOperand()
{
    return _right;
}

void BinaryExpression::SetLeftOperand(Expression* expr)
{
    _left = expr;
}

void BinaryExpression::SetRightOperand(Expression* expr)
{
    _right = expr;
}


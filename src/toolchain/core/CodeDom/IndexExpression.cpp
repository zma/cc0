#include "IndexExpression.h"
#include "ExpressionVisitor.h"
#include <core/Type/Type.h>
#include <core/Type/ArrayType.h>
#include <core/Type/PointerType.h>
#include <stdlib.h>
#include "ConstantExpression.h"
#include "BinaryExpression.h"
#include <core/Misc/ConstantValue.h>
#include <typeinfo>

IndexExpression::IndexExpression():Expression(Expression::ArrayIndex), Container(NULL), Index(NULL)
{
}

Type* IndexExpression::GetType()
{
    Type *containerType =  Container->GetType();
    if (typeid(*containerType) == typeid(ArrayType))
    {
        ArrayType *arrayType = dynamic_cast<ArrayType *>(containerType);
        return arrayType->GetElementType();
    }
    else if (typeid(*containerType) == typeid(PointerType))
    {
        PointerType *pointerType = dynamic_cast<PointerType *>(containerType);
        return pointerType->GetUnderlyingType();
    }
    else
    {
        abort();
    }
}

Expression* IndexExpression::GetLValue()
{
//     //Address of array element
//     Expression * base = _container;
//     Type *elementType = this->GetType();
//     Expression *offset = new BinaryExpression(Expression::Multiply, new ConstantExpression(new ConstantValue(elementType->GetSize())), _index);
//     
//     Expression *addr = new BinaryExpression(Expression::Add, base, offset);
    return this;
}

void IndexExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

IndexExpression::IndexExpression(Expression *container, Expression *index)
    :Expression(Expression::ArrayIndex)
{
    Container = container;
    Index = index;
}

IndexExpression::~IndexExpression()
{

}

Expression* IndexExpression::GetContainer()
{
    return Container;
}

Expression* IndexExpression::GetIndex()
{
    return Index;
}


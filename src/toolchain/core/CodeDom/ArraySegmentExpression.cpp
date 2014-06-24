#include "ArraySegmentExpression.h"
#include "ExpressionVisitor.h"

ArraySegmentExpression::ArraySegmentExpression():Expression(Expression::ArraySegment), _container(NULL), _start(NULL), _end(NULL)
{
}

ArraySegmentExpression::ArraySegmentExpression(Expression* array, Expression* start, Expression* end)
    : Expression(Expression::ArraySegment)
{
    _container = array;
    _start = start;
    _end = end;
}


ArraySegmentExpression::~ArraySegmentExpression()
{

}

Expression* ArraySegmentExpression::GetLValue()
{
    //FIXME
    
    return this->GetArray()->GetLValue();
}

void ArraySegmentExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

Expression* ArraySegmentExpression::GetArray()
{
    return _container;
}

Expression* ArraySegmentExpression::GetStartIndex()
{
    return _start;
}

Expression* ArraySegmentExpression::GetEndIndex()
{
    return _end;
}

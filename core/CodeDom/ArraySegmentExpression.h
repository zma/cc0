#ifndef ARRAYSEGMENTEXPRESSION_H
#define ARRAYSEGMENTEXPRESSION_H

#include <core/CodeDom/Expression.h>


class ArraySegmentExpression : public Expression
{
private:
    Expression *_container;
    Expression *_start;
    Expression *_end;
public:
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    ArraySegmentExpression(Expression *array, Expression *start, Expression *end);
    virtual ~ArraySegmentExpression();
public:
    Expression *GetArray();
    Expression *GetStartIndex();
    Expression *GetEndIndex();
};

#endif // ARRAYSEGMENTEXPRESSION_H

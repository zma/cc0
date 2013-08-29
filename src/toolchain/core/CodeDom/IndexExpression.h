#ifndef INDEXEXPRESSION_H
#define INDEXEXPRESSION_H

#include "Expression.h"


class IndexExpression : public Expression
{
public:
    Expression *Container;
    Expression *Index;
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    IndexExpression(Expression *container, Expression *index);
    virtual ~IndexExpression();
public:
    Expression *GetContainer();
    Expression *GetIndex();
};

#endif // INDEXEXPRESSION_H

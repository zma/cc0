#ifndef NOPEXPRESSION_H
#define NOPEXPRESSION_H

#include "Expression.h"


class NopExpression : public Expression
{
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    NopExpression();
    virtual ~NopExpression();
};

#endif // NOPEXPRESSION_H

#ifndef RETURNEXPRESSION_H
#define RETURNEXPRESSION_H
#include "Expression.h"


class ReturnExpression : public Expression
{
private:
    Expression *_value;
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    ReturnExpression();
    ReturnExpression(Expression *value);
    virtual ~ReturnExpression();
public:
    Expression *GetValue();
};

#endif // RETURNEXPRESSION_H

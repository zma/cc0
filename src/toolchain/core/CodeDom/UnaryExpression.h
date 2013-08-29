#ifndef UNARYEXPRESSION_H
#define UNARYEXPRESSION_H
#include "Expression.h"

class UnaryExpression: public Expression
{
public:
    Expression *Operand;
public:
    UnaryExpression(ExpressionAction nodeType, Expression *operand);
    virtual ~UnaryExpression();
public:
    virtual void Accept(ExpressionVisitor *visitor);
    virtual Expression * GetLValue();    
    virtual Type * GetType();
public:
    Expression * GetOperand();
};

#endif // UNARYEXPRESSION_H

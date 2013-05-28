#ifndef BINARYEXPRESSION_H
#define BINARYEXPRESSION_H

#include "Expression.h"

class BinaryExpression : public Expression
{
private:
    Expression *_left, *_right;
public:
    BinaryExpression(ExpressionAction nodeType, Expression *left, Expression *right);
    virtual ~BinaryExpression();
public:
    virtual void Accept(ExpressionVisitor *visitor);
    virtual Expression * GetLValue();
public:
    Expression *GetLeftOperand();
    Expression *GetRightOperand();
    void SetLeftOperand(Expression *expr);
    void SetRightOperand(Expression *expr);
};

#endif // BINARYEXPRESSION_H

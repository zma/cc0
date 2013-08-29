#ifndef ASSIGNEXPRESSION_H
#define ASSIGNEXPRESSION_H

#include "Expression.h"


class AssignExpression : public Expression
{
private:
    Expression::ExpressionAction _additionOperator;
    Expression *_target;
    Expression *_source;
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    AssignExpression(Expression *target, Expression *source);
    AssignExpression(Expression *target, Expression *source, Expression::ExpressionAction additionalOp);
    virtual ~AssignExpression();
public:
    Expression *GetTarget();
    Expression *GetSource();
    void SetSource(Expression *source);
    void SetTarget(Expression *target);
    Expression::ExpressionAction GetAdditionalOperator();
};

#endif // ASSIGNEXPRESSION_H

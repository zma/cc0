#ifndef CONVERTEXPRESSION_H
#define CONVERTEXPRESSION_H

#include "Expression.h"

class Type;

class ConvertExpression : public Expression
{
private:
    Type *_targetType;
    Expression * _source;
public:
    ConvertExpression(Type *targetType, Expression *sourceExpression);
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    virtual ~ConvertExpression();
public:
    Type *GetTargetType();
    Expression *GetSourceExpression();
};

#endif // CONVERTEXPRESSION_H

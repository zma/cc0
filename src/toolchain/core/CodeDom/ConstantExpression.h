#ifndef CONSTANTEXPRESSION_H
#define CONSTANTEXPRESSION_H

#include "Expression.h"
#include <string>
#include <core/Type/Type.h>

class ConstantValue;
class ConstantExpression : public Expression
{
private:
    ConstantValue *_value;
public:
    
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
public:
    ConstantExpression(ConstantValue *value);
    virtual ~ConstantExpression();
public:
    ConstantValue *GetValue();
};

#endif // CONSTANTEXPRESSION_H

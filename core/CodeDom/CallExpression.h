#pragma once 

#include "Expression.h"

#include <vector>
#include <string>

class CallExpression : public Expression
{
private:
    Expression *_target;
    std::vector<Expression *> _arguments;
public:
    CallExpression(Expression *target);
    CallExpression(Expression *target, std::vector<Expression *> *arguments);
    virtual ~CallExpression();
public:
    virtual void Accept(ExpressionVisitor *visitor);
    virtual Expression * GetLValue();
    virtual Type * GetType();
public:
    Expression * GetTarget();
    std::vector<Expression *> * GetArgumentList();
};
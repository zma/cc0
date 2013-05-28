#ifndef VARIABLEEXPRESSION_H
#define VARIABLEEXPRESSION_H

#include "Expression.h"

class Symbol;

class VariableExpression : public Expression
{
private:
    Symbol *_symbol;    
public:
    VariableExpression(Symbol *symbol);
    Symbol * GetSymbol();
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    virtual ~VariableExpression();
};

#endif // VARIABLEEXPRESSION_H

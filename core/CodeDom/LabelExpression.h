#ifndef LABELEXPRESSION_H
#define LABELEXPRESSION_H

#include "Expression.h"
#include <string>

class LabelExpression : public Expression
{
private:
    static unsigned int _labelID;
    Symbol *_symbol;
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
public:
    LabelExpression(Symbol *symbol);
    virtual ~LabelExpression();
public:
    std::string GetName();
    Symbol *GetSymbol();
};

#endif // LABELEXPRESSION_H

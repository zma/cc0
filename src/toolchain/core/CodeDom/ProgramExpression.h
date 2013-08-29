#ifndef PROGRAMEXPRESSION_H
#define PROGRAMEXPRESSION_H

#include "Expression.h"
#include <vector>
#include <core/Symbol/SymbolScope.h>

class SymbolScope;
class FunctionExpression;

class ProgramExpression : public Expression
{
private:
    std::vector<Expression *> _functions;
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
public:
    ProgramExpression();
    virtual ~ProgramExpression();
public:
    std::vector<Expression *> *GetFunctions();
};

#endif // PROGRAMEXPRESSION_H

#ifndef FUNCTIONEXPRESSION_H
#define FUNCTIONEXPRESSION_H
#include "Expression.h"
#include <vector>
#include <string>

class FunctionType;
class BlockExpression;
class Declaration;
class Type;

class FunctionExpression : public Expression
{
private:
    BlockExpression *_body;
    Symbol *_symbol;
    SymbolScope *_functionScope;
public:
    FunctionExpression(Symbol *symbol, SymbolScope *functionScope);
    virtual ~FunctionExpression();
public:
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    virtual Type * GetType();
public:
    Symbol *GetSymbol();
    std::string GetName();
    BlockExpression *GetBody();
    void SetBody(BlockExpression *body);
    Type * GetReturnType();
    std::vector<Declaration *> *GetParameters();
    SymbolScope *GetFunctionScope();
    
};

#endif // FUNCTIONEXPRESSION_H

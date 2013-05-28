#ifndef BLOCKEXPRESSION_H
#define BLOCKEXPRESSION_H
#include "Expression.h"

#include <vector>

class BlockExpression : public Expression
{
private:
    std::vector<Expression *> *_expressions;
public:
    BlockExpression();
    virtual ~BlockExpression();
    SymbolScope *_scope;
public:
    virtual void Accept(ExpressionVisitor *visitor);
    virtual Expression * GetLValue();
    virtual Type * GetType();
public:
    std::vector<Expression *> * GetExpressionList();
    void SetExpressionList(std::vector<Expression *> *exprList);
    SymbolScope *GetBlockScope();
    void *SetBlockScope(SymbolScope *scope);
};

#endif // BLOCKEXPRESSION_H

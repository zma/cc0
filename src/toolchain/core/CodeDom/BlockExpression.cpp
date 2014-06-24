
#include "BlockExpression.h"
#include <stdlib.h>
#include "ExpressionVisitor.h"
#include <core/Type/VoidType.h>

BlockExpression::BlockExpression()
        : Expression(Expression::Block), _scope(NULL)
{
    _expressions = new std::vector<Expression *>();
    //FIXME: memory leak!
}

BlockExpression::~BlockExpression()
{

}

void BlockExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

Expression* BlockExpression::GetLValue()
{
    return NULL;
}

std::vector<Expression *> * BlockExpression::GetExpressionList()
{
    return _expressions;
}

Type* BlockExpression::GetType()
{
    if (_expressions->empty())
    {
        return new VoidType();
    }
    else
    {
        return (*_expressions)[_expressions->size() - 1]->GetType();
    }
}

void BlockExpression::SetExpressionList(std::vector< Expression* >* exprList)
{
    _expressions = exprList;
}

SymbolScope* BlockExpression::GetBlockScope()
{
    return _scope;
}

void* BlockExpression::SetBlockScope(SymbolScope* scope)
{
    _scope = scope;
}



#include "FunctionExpression.h"
#include "ExpressionVisitor.h"
#include <stdlib.h>
#include <string>
#include <core/Type/FunctionType.h>
#include <core/Symbol/Symbol.h>

FunctionExpression::FunctionExpression():Expression(Expression::Function), _body(NULL), _symbol(NULL), _functionScope(NULL)
{
}

FunctionExpression::FunctionExpression(Symbol *symbol, SymbolScope *functionScope)
        : Expression(Expression::Function), _body(NULL)
{

    _symbol = symbol;
    _functionScope = functionScope;
    _symbol->DeclSource = this;
}

FunctionExpression::~FunctionExpression()
{

}

Expression* FunctionExpression::GetLValue()
{
    return NULL;
}

void FunctionExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

Type* FunctionExpression::GetType()
{
    return _symbol->DeclType;
}

BlockExpression* FunctionExpression::GetBody()
{
    return _body;
}

std::string FunctionExpression::GetName()
{
    return _symbol->Name;
}

Type* FunctionExpression::GetReturnType()
{
    FunctionType *functionType = dynamic_cast<FunctionType *>(this->_symbol->DeclType);
    return functionType->GetReturnType();
}

std::vector< Declaration* >* FunctionExpression::GetParameters()
{
    FunctionType *functionType = dynamic_cast<FunctionType *>(this->_symbol->DeclType);
    return functionType->GetParameters();
}

void FunctionExpression::SetBody(BlockExpression* body)
{
    _body = body;
}

Symbol* FunctionExpression::GetSymbol()
{
    return _symbol;
}

SymbolScope* FunctionExpression::GetFunctionScope()
{
    return _functionScope;
}


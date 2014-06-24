
#include "Symbol.h"
#include <stdlib.h>

Symbol::Symbol() : DeclType(NULL), Scope(NULL), DeclSource(NULL), Address(0), Kind(ObjectName)
{
}

Symbol::Symbol(std::string name, Type* declType, SymbolKind kind) : Scope(NULL), DeclSource(NULL)
{
    this->Name = name;
    this->DeclType = declType;
    this->Kind = kind;
    this->Address = 0;
}

/*

Expression* Symbol::GetDeclExpression()
{
    return _declExpr;
}

std::string Symbol::GetName()
{
    return _name;
    
}


Type* Symbol::GetDeclType()
{
    return this->_declType;
}

SymbolScope* Symbol::GetDeclScope()
{
    return _declScope;
}

SymbolAddress Symbol::GetAddress()
{
    return _addr;
}
void Symbol::SetAddress(SymbolAddress addr)
{
    _addr = addr;
}

Symbol::SymbolKind Symbol::GetSymbolKind()
{
    return _kind;
}
*/


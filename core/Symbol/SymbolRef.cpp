#include "SymbolRef.h"
#include "Symbol.h"
#include "SymbolScope.h"

SymbolRef::SymbolRef()
{
    Scope = NULL;
    Name = "";
}

SymbolRef::SymbolRef(SymbolScope* scope, std::string name)
{
    Scope = scope;
    Name = name;
}

SymbolRef::SymbolRef(Symbol* symbol)
{
    Scope = symbol->Scope;
    Name = symbol->Name;
}

Symbol* SymbolRef::Lookup()
{
    return Scope->Lookup(Name);
}

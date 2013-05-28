#ifndef SYMBOLREF_H
#define SYMBOLREF_H

#include <string>

class Symbol;
class SymbolScope;

struct SymbolRef
{
    SymbolRef();
    SymbolRef(SymbolScope *scope, std::string name);
    SymbolRef(Symbol* symbol);
    SymbolScope *Scope;
    std::string Name;
    
    Symbol *Lookup();
};

#endif // SYMBOLREF_H

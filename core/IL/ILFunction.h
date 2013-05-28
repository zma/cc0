#pragma once 
#include "IL.h"

class ILClass;
class SymbolScope;
class ILFunction
{
public:
    std::vector<IL> Body;
    SymbolScope *Scope;
    ILClass *Container;
    Symbol *FunctionSymbol;
public:
    ILFunction(ILClass *container, Symbol *functionSymbol);
    virtual ~ILFunction();
};


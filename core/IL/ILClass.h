#pragma once 
#include <vector>

class ILProgram;
class SymbolScope;
class ILFunction;
class Symbol;

class ILClass
{
public:
    std::vector<ILFunction *> Functions;
    SymbolScope *Scope;
    ILProgram *Container;
    Symbol *ClassSymbol;
public:
    ILClass(ILProgram *container, Symbol *classSymbol);
    virtual ~ILClass();
};


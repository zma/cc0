#pragma once 

#include <vector>

class ILClass;
class SymbolScope;

class ILProgram
{
public:
    std::vector<ILClass *> Claases;
    SymbolScope *Scope;
    ILProgram();
    virtual ~ILProgram();
};


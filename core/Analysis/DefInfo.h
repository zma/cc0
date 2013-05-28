#pragma once

#include <vector>

class Symbol;
class IL;
class UseInfo;

struct DefInfo
{
public:
    Symbol *ReferencedSymbol;
    IL * EffectiveTac;
    std::vector<UseInfo *> UseClosure;
};
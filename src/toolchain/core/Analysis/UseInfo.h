#pragma once

#include <vector>

class Symbol;
class IL;
class DefInfo;

struct UseInfo
{
public:
    Symbol *ReferencedSymbol;
    std::vector<IL *> TacList;
    std::vector<DefInfo *> DefClosure;
};
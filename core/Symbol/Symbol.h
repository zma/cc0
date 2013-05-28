#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <stdint.h>

#include "SymbolAddress.h"

class Expression;
class SymbolScope;
class SymbolAddressAllocator;
class SemanticTreeNode;
class Type;
class FunctionExpression;

struct Symbol
{   
    enum SymbolKind
    {
        ObjectName,
        TypeName
    };
    Symbol(std::string name, Type *declType, SymbolKind kind = Symbol::ObjectName);
    std::string Name;
    Type *DeclType;
    SymbolScope *Scope;
    Expression *DeclSource;
    int64_t Address;
    SymbolKind Kind;
};

#endif // SYMBOL_H

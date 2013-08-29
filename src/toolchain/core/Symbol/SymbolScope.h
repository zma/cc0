#ifndef SYMBOLSCOPE_H
#define SYMBOLSCOPE_H
#include <string>
#include <map>
#include <core/Misc/Location.h>
#include <vector>
#include <stdint.h>


class Expression;
class Symbol;
class SemanticTreeNode;

class SymbolScope
{
public:
    enum ScopeKind
    {
        Global,
        Struct,
        Function,
        Block
    };
private:
    SymbolScope * _parentScope;
    std::vector<SymbolScope *> _childScopes;
    std::map<std::string, Symbol *> _symbolTable;
    ScopeKind _kind;
    static SymbolScope *_rootScope;
    Expression *_expression;
    uint64_t _memorySize;
public:
    std::map<std::string, Symbol *> *GetSymbolTable();
    void SetMemorySize(int64_t size);
public:
    static SymbolScope *GetRootScope();
public:
    SymbolScope(SymbolScope *parentScope, ScopeKind kind, Expression *associatedExpression);
    virtual ~SymbolScope();
public:
    int64_t GetMemorySize();
    SymbolScope *GetParentScope();
    std::vector<SymbolScope *> *GetChildScopes();
    ScopeKind GetScopeKind();
    Symbol *Lookup(std::string name);
    Symbol *LookupLocal(std::string name);
    Symbol *Lookup(std::string, Location refLocation);
    Symbol *LookupLocal(std::string, Location refLocation);
    bool Add(Symbol *symbol);
    bool Remove(std::string);
    Expression *GetAssociatedExpression();
    void SetAssociatedExpression(Expression *expr);
public:
    void Dump();
protected:
    void MakeIndent(int level);
    void Dump(int level);
//    Expression * GetTreeNode();
};

#endif // SYMBOLSCOPE_H

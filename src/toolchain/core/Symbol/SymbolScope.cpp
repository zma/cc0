#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#include <map>

#include <core/Core.h>
#include <core/Type/Type.h>
#include <core/CodeDom/Expression.h>
#include "core/IL/ILProgram.h"
#include "SymbolScope.h"
#include "Symbol.h"

SymbolScope* SymbolScope::_rootScope = NULL;

void SymbolScope::__SetRootScopt(ILProgram* il_program)
{
	_rootScope = il_program->Scope;
}


SymbolScope::SymbolScope():	_parentScope(NULL), _kind(Global), _expression(NULL), _memorySize(0)
{
}

SymbolScope* SymbolScope::GetRootScope()
{
    return _rootScope;
}


SymbolScope::SymbolScope(SymbolScope *parentScope, ScopeKind kind, Expression *associatedExpression):_memorySize(0)
{
    assert(parentScope != this);
    this->_kind = kind;
    this->_parentScope = parentScope;
    this->_expression = associatedExpression;

    if (SymbolScope::_rootScope == NULL)
    {
        if (parentScope == NULL)
        {
            assert(kind == SymbolScope::Global);
            SymbolScope::_rootScope = this;
        }
        else
        {
            abort();
        }
    }
    else
    {
        if (parentScope == NULL)
        {
            abort();
        }
        else
        {
            this->_parentScope->_childScopes.push_back(this);
        }
    }


}

SymbolScope::~SymbolScope()
{
    if (_parentScope != NULL)
    {
        for (std::vector<SymbolScope *>::iterator it = _parentScope->_childScopes.begin();
             it != _parentScope->_childScopes.end();
             ++it)
        {
            SymbolScope *scope = *it;
            if (scope == this)
            {
                _parentScope->_childScopes.erase(it);
                break;
            }
        }
    }
}



SymbolScope::ScopeKind SymbolScope::GetScopeKind()
{
    return _kind;
}

bool SymbolScope::Add(Symbol* symbol)
{
    std::pair<std::string, Symbol*> pair;
    pair.first = symbol->Name;
    pair.second = symbol;
    _symbolTable[pair.first] = pair.second;
    _symbolTable.erase(symbol->Name);
    bool result = (_symbolTable.insert(pair)).second;
    if (result)
    {
        symbol->Scope = this;
    }
    return result;
}

bool SymbolScope::Remove(std::string name)
{
    _symbolTable.erase(name);
}
Symbol* SymbolScope::Lookup(std::string name, Location refLocation)
{
    Symbol *sym = Lookup(name);
    return sym;
}
Symbol* SymbolScope::LookupLocal(std::string name, Location refLocation)
{
    Symbol *sym = LookupLocal(name);
    return sym;
}

SymbolScope* SymbolScope::GetParentScope()
{
    return _parentScope;
}

std::vector< SymbolScope* >* SymbolScope::GetChildScopes()
{
    return &_childScopes;
}

Symbol* SymbolScope::Lookup(std::string name)
{
    Symbol *sym = LookupLocal(name);
    if (sym == NULL)
    {
        if (_parentScope != NULL)
        {
            return _parentScope->Lookup(name);
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        return sym;
    }
}
Symbol* SymbolScope::LookupLocal(std::string name)
{
    std::map<std::string, Symbol *>::iterator it = _symbolTable.find(name);
    if (it == _symbolTable.end())
    {
        return NULL;
    }

    return it->second;
}

void SymbolScope::MakeIndent(int level)
{
    for (int i = 0; i < level; i++)
    {
        if(CompilationContext::GetInstance()->Debug)
            printf("    ");
    }
}

void SymbolScope::Dump(int level)
{
    MakeIndent(level);
    switch (_kind)
    {
    case SymbolScope::Global:
        if(CompilationContext::GetInstance()->Debug)
            printf("Global scope: SIZE: 0x%llX {\n", (long long)_memorySize);
        break;
    case SymbolScope::Function:
        if(CompilationContext::GetInstance()->Debug)
            printf("Function scope: SIZE: 0x%llX {\n", (long long)_memorySize);
        break;
    case SymbolScope::Block:
        if(CompilationContext::GetInstance()->Debug)
            printf("Local scope: SIZE: 0x%llX {\n", (long long)_memorySize);
        break;
    }

    for (std::map<std::string, Symbol *>::iterator it = _symbolTable.begin(); it != _symbolTable.end(); ++it)
    {
        std::pair<std::string, Symbol *> pair = *it;
        Symbol *sym = pair.second;
        MakeIndent(level + 1);
        if (sym->Address != 0)
        {
            if (sym->Address >= 0)
            {
                if(CompilationContext::GetInstance()->Debug)
                    printf("NAME: %s \tTYPE: %s \tOFFSET: +0x%llX\n", sym->Name.c_str(), sym->DeclType->ToString().c_str(), (long long)sym->Address);
            }
            else
            {
                if(CompilationContext::GetInstance()->Debug)
                    printf("NAME: %s \tTYPE: %s \tOFFSET: -0x%llX\n", sym->Name.c_str(), sym->DeclType->ToString().c_str(), -(long long)(sym->Address));

            }
        }
        else
        {
            if(CompilationContext::GetInstance()->Debug)
                printf("NAME: %s \tTYPE: %s\n", sym->Name.c_str(), sym->DeclType->ToString().c_str());

        }
    }

    for (std::vector<SymbolScope *>::iterator it = _childScopes.begin();
         it != _childScopes.end();
         ++it)
    {
        (*it)->Dump(level + 1);
    }

    MakeIndent(level);
    if(CompilationContext::GetInstance()->Debug)
        printf("}\n");
}

void SymbolScope::Dump()
{
    if(CompilationContext::GetInstance()->Debug)
        printf("====================================================\n");
    Dump(0);
    if(CompilationContext::GetInstance()->Debug)
        printf("====================================================\n");
}

Expression* SymbolScope::GetAssociatedExpression()
{
    return _expression;
}

std::map< std::string, Symbol* >* SymbolScope::GetSymbolTable()
{
    return &_symbolTable;
}


int64_t SymbolScope::GetMemorySize()
{
    return _memorySize;
}

void SymbolScope::SetMemorySize(int64_t size)
{
    _memorySize = size;
}

void SymbolScope::SetAssociatedExpression(Expression* expr)
{
    _expression = expr;
}

void SymbolScope::__SetParentScope(SymbolScope* __parent){
	_parentScope = __parent;
}


#include "Expression.h"
#include <core/Symbol/SymbolScope.h>
#include <core/Misc/CompilationContext.h>
#include <stdlib.h>
#include <antlr3commontoken.h>
#include <antlr3parser.h>

Expression::Expression() : _parent(NULL), _nodeType(Nop), _scope(NULL)
{
}

Expression::Expression(ExpressionAction nodeType):_parent(NULL)
{
    this->_nodeType = nodeType;
    _scope = NULL;
    pANTLR3_PARSER pParser = CompilationContext::GetInstance()->CurrentParser;
    if (pParser != NULL)
    {
        pANTLR3_COMMON_TOKEN token = pParser->tstream->_LT(pParser->tstream, 0);
        Location loc;
        loc.Line = token->getLine(token);
        loc.Column = token->getCharPositionInLine(token);
        loc.FileName = CompilationContext::GetInstance()->CurrentFileName;
        this->SourceLocation = loc;
    }
    else
    {
        this->SourceLocation = Location();
    }
 
}

Expression::~Expression()
{
//     pCParser ctx;
}

Expression::ExpressionAction Expression::GetAction()
{
    return _nodeType;
}


/*
Expression* Expression::GetParent()
{
    return _parent;
}

SymbolScope* Expression::GetScope()
{
    if (_scope == NULL)
    {
        if (_parent == NULL)
        {
            return NULL;
        }
        else
        {
            return _parent->GetScope();
        }
    }
    else
    {
        return _scope;
    }
}

void Expression::CreateScope(SymbolScope::ScopeKind kind)
{
    SymbolScope *parentScope;
    if (_parent == NULL)
    {
        parentScope = NULL;
    }
    else
    {
        parentScope = _parent->GetScope();
    }
    _scope = new SymbolScope(parentScope, kind);
}
*/



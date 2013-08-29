#pragma once
#include <core/Symbol/SymbolScope.h>

class SymbolScope;
class ExpressionVisitor;
class Type;
class LValue;
class Expression
{
public:
    enum ExpressionAction
    {
        Nop,
        Program,
        Add,
        And,
        AndAlso,
        ArrayIndex,
        Call,
        Conditional,
        Constant,
        Convert,
        Divide,
        Equal,
        ExclusiveOr,
        Function,
        GreaterThan,
        GreaterThanOrEqual,
        LeftShift,
        LessThan,
        LessThanOrEqual,
        Modulo,
        Multiply,
        Plus,
        Minus,
        Not,
        NotEqual,
        Or,
        OrElse,
        RightShift,
        Subtract,
        Assign,
        Block,
        Return,
        Goto,
        Label,
        Loop,
        Switch,
        Member,
        Variable,
        Reference,
        Dereference,
        InlineAssembly,
        
        // DISA specific
        ArraySegment,
        NewRunner,
        Commit,
        Abort,
    };
private:
    Expression *_parent;
    ExpressionAction _nodeType;
    SymbolScope *_scope;
    std::map<std::string, void *> _tags;
public:
    Location SourceLocation;
protected:
    Expression(ExpressionAction nodeType);
public:
    virtual ~Expression();
public:
    virtual void Accept(ExpressionVisitor *visitor) = 0;
    virtual Expression * GetLValue() = 0;
public:
    ExpressionAction GetAction();

    template <typename T> T* GetTag(std::string name)
    {
        std::map<std::string, void *>::iterator it = _tags.find(name);
        if (it == _tags.end())
        {
            return NULL;
        }
        else
        {
            void *data = it->second;
            return reinterpret_cast<T *>(data);
        }
    }    
    
    template <typename T> void SetTag(std::string name, T* value)
    {
        void *data = reinterpret_cast<void *>(value);
        _tags[name] = data;
    }
    
    Type * GetType()
    {
        return GetTag<Type>("Type");
    }

};


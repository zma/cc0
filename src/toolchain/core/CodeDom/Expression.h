#pragma once
#include <core/Symbol/SymbolScope.h>
#include <core/IL/IL.h>
#include <core/Type/Type.h>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>

class SymbolScope;
class ExpressionVisitor;
class Type;
class LValue;
class Expression
{
private:
	Expression();
	friend class ::boost::serialization::access;
	template<class A>
	void save(A& ar, const unsigned int) const
	{
		ar & BOOST_SERIALIZATION_NVP(_parent);
		ar & BOOST_SERIALIZATION_NVP(_nodeType);
		ar & BOOST_SERIALIZATION_NVP(_scope);
		ar & BOOST_SERIALIZATION_NVP(SourceLocation);
		size_t _tags_size = _tags.size();
		ar & BOOST_SERIALIZATION_NVP(_tags_size);
		for(std::map<std::string, void *>::const_iterator i = _tags.begin(), iE = _tags.end(); i != iE; ++i)
		{
			const std::string& current_tag = i->first;
			ar & BOOST_SERIALIZATION_NVP(current_tag);
			if(current_tag == "Constant")
			{
				ConstantValue* pObj = reinterpret_cast<ConstantValue*>(i->second);
				ar & BOOST_SERIALIZATION_NVP(pObj);
			}
			else if(current_tag == "ILOperand" || current_tag == "ContinueLabel" || current_tag == "BreakLabel" || current_tag == "StartLabel")
			{
				IL::ILOperand* pObj = reinterpret_cast<IL::ILOperand*>(i->second);
				ar & BOOST_SERIALIZATION_NVP(pObj);
			}
			else if(current_tag == "Type")
			{
				Type* pObj = reinterpret_cast<Type*>(i->second);
				ar & BOOST_SERIALIZATION_NVP(pObj);
			}
			else
			{
				::std::cerr<<"on save: expression _tag "<<current_tag<< " not handled!\n";
				exit(-1);
			}
		}
		/*if(_tags.size())
		{
			::std::cerr<<"expression _tags has content\n";
			exit(-1);
		}*/
	}
	template<class A>
	void load(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_NVP(_parent);
		ar & BOOST_SERIALIZATION_NVP(_nodeType);
		ar & BOOST_SERIALIZATION_NVP(_scope);
		ar & BOOST_SERIALIZATION_NVP(SourceLocation);
		size_t _tags_size;
		ar & BOOST_SERIALIZATION_NVP(_tags_size);
		for(size_t i = 0; i < _tags_size; ++i)
		{
			std::string current_tag;
			ar & BOOST_SERIALIZATION_NVP(current_tag);
			if(current_tag == "Constant")
			{
				ConstantValue* pObj;
				ar & BOOST_SERIALIZATION_NVP(pObj);
				_tags[current_tag] = reinterpret_cast<void*>(pObj);
			}
			else if(current_tag == "ILOperand" || current_tag == "ContinueLabel" || current_tag == "BreakLabel" || current_tag == "StartLabel")
			{
				IL::ILOperand* pObj;
				ar & BOOST_SERIALIZATION_NVP(pObj);
				_tags[current_tag] = reinterpret_cast<void*>(pObj);
			}
			else if(current_tag == "Type")
			{
				Type* pObj;
				ar & BOOST_SERIALIZATION_NVP(pObj);
				_tags[current_tag] = reinterpret_cast<void*>(pObj);
			}
			else
			{
				::std::cerr<<"on load: expression _tag "<<current_tag<< " not handled!\n";
				exit(-1);
			}
		}
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER();
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


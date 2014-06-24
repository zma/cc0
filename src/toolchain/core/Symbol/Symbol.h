#ifndef SYMBOL_H
#define SYMBOL_H

#include <string>
#include <stdint.h>

#include "SymbolAddress.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>

class Expression;
class SymbolScope;
class SymbolAddressAllocator;
class SemanticTreeNode;
class Type;
class FunctionExpression;

class Symbol
{   
private:
	Symbol();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_NVP(Name);
		ar & BOOST_SERIALIZATION_NVP(DeclType);
		ar & BOOST_SERIALIZATION_NVP(Scope);
		ar & BOOST_SERIALIZATION_NVP(DeclSource);
		ar & BOOST_SERIALIZATION_NVP(Address);
		ar & BOOST_SERIALIZATION_NVP(Kind);
	}
public:
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

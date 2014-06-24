#pragma once 
#include "IL.h"
#include <boost/serialization/vector.hpp>

class ILClass;
class SymbolScope;
class ILFunction
{
private:
	ILFunction();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_NVP(Body);
		ar & BOOST_SERIALIZATION_NVP(Scope);
		ar & BOOST_SERIALIZATION_NVP(Container);
		ar & BOOST_SERIALIZATION_NVP(FunctionSymbol);
	}
public:
    std::vector<IL> Body;
    SymbolScope *Scope;
    ILClass *Container;
    Symbol *FunctionSymbol;
public:
    ILFunction(ILClass *container, Symbol *functionSymbol);
    virtual ~ILFunction();
};


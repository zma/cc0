#pragma once 
#include <vector>
#include <boost/serialization/vector.hpp>

class ILProgram;
class SymbolScope;
class ILFunction;
class Symbol;

class ILClass
{
private:
	ILClass();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_NVP(Functions);
		ar & BOOST_SERIALIZATION_NVP(Scope);
		ar & BOOST_SERIALIZATION_NVP(Container);
		ar & BOOST_SERIALIZATION_NVP(ClassSymbol);
	}
public:
    std::vector<ILFunction *> Functions;
    SymbolScope *Scope;
    ILProgram *Container;
    Symbol *ClassSymbol;
public:
    ILClass(ILProgram *container, Symbol *classSymbol);
    virtual ~ILClass();
};


#ifndef SYMBOLREF_H
#define SYMBOLREF_H

#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>

class Symbol;
class SymbolScope;

class SymbolRef
{
private:
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_NVP(Scope);
		ar & BOOST_SERIALIZATION_NVP(Name);
	}
public:
    SymbolRef();
    SymbolRef(SymbolScope *scope, std::string name);
    SymbolRef(Symbol* symbol);
    SymbolScope *Scope;
    std::string Name;
    
    Symbol *Lookup();
};

#endif // SYMBOLREF_H

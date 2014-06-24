#pragma once 

#include <vector>
#include <boost/serialization/vector.hpp>

class ILClass;
class SymbolScope;

class ILProgram
{
private:
	friend class ::boost::serialization::access;
	template<class Archive>
	void serialize(Archive& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_NVP(Claases);
		ar & BOOST_SERIALIZATION_NVP(Scope);
	}
public:
    std::vector<ILClass *> Claases;
    SymbolScope *Scope;
    ILProgram();
    virtual ~ILProgram();
};


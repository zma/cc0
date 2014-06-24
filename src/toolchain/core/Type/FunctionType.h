#pragma once
#include "Type.h"
#include <vector>
#include <boost/serialization/vector.hpp>

class SymbolScope;
class Declaration;

class FunctionType: public Type
{
private:
	FunctionType();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Type);
		ar & BOOST_SERIALIZATION_NVP(_parameters);
		ar & BOOST_SERIALIZATION_NVP(_returnType);
	}
private:
    std::vector<Declaration *> *_parameters;
    Type *_returnType;
public:
    FunctionType(Type *returnType, std::vector<Declaration *> *parameters);
    virtual ~FunctionType();
public:
    virtual int64_t GetSize();
    virtual bool CanImplicitlyConvertTo(Type *other);
    virtual bool CanExplicitlyConvertTo(Type *other);
    virtual bool Equals(Type *other);
public:
    std::vector<Declaration *> *GetParameters();
    Type * GetReturnType();
    virtual std::string ToString() ;
};

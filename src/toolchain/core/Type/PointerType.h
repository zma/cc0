#ifndef POINTERTYPE_H
#define POINTERTYPE_H
#include "Type.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class PointerType: public Type
{
private:
	PointerType();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Type);
		ar & BOOST_SERIALIZATION_NVP(_underlyingType);
	}
public:
    // TODO: Make it more portable
    static const int PointerSize = 8;
private:
    Type * _underlyingType;
public:
    PointerType(Type * underlyingType);
    virtual ~PointerType();
public:
    virtual int64_t GetSize();
    virtual bool CanImplicitlyConvertTo(Type *other);
    virtual bool CanExplicitlyConvertTo(Type *other);
    virtual bool Equals(Type *other);
public:
    Type * GetUnderlyingType();
    virtual std::string ToString() ;
};

#endif // POINTERTYPE_H

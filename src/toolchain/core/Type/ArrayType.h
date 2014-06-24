#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include "Type.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class Expression;

class ArrayType : public Type
{
private:
	ArrayType();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Type);
		ar & BOOST_SERIALIZATION_NVP(_elementType);
		ar & BOOST_SERIALIZATION_NVP(_size);
	}
private:
    Type * _elementType;
    int64_t _size;
public:
    virtual std::string ToString();
    virtual bool CanExplicitlyConvertTo(Type* other);
    virtual bool CanImplicitlyConvertTo(Type* other);
    virtual bool Equals(Type* other);
    virtual int64_t GetSize();
public:
    ArrayType(Type* elementType, int64_t size = -1);
    virtual ~ArrayType();
public:
    Type * GetElementType();
};

#endif // ARRAYTYPE_H

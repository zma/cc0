#ifndef VOIDTYPE_H
#define VOIDTYPE_H
#include "Type.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class VoidType: public Type
{
private:
	friend class ::boost::serialization::access;
	template <class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Type);
	}
public:
    VoidType();
    virtual ~VoidType();
public:
    virtual int64_t GetSize();
    virtual bool CanImplicitlyConvertTo(Type *other);
    virtual bool CanExplicitlyConvertTo(Type *other);
    virtual bool Equals(Type *other);
    virtual std::string ToString() ;
};

#endif // VOIDTYPE_H

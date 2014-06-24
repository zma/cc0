#ifndef INTEGERTYPE_H
#define INTEGERTYPE_H

#include <core/Type/Type.h>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class IntegerType : public Type
{
private:
	IntegerType();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Type);
		ar & BOOST_SERIALIZATION_NVP(_legnth);
		ar & BOOST_SERIALIZATION_NVP(_signedType);
	}
private:
    int64_t _legnth;
    bool _signedType;
public:
    IntegerType(int64_t length, bool signedType);
    virtual ~IntegerType();
public:
    virtual std::string ToString();
    virtual bool CanExplicitlyConvertTo(Type* other);
    virtual bool CanImplicitlyConvertTo(Type* other);
    virtual bool Equals(Type* other);
    virtual int64_t GetSize();
public:
    bool IsSigned();
};

#endif // INTEGERTYPE_H

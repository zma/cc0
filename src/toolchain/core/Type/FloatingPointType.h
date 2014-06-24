#ifndef FLOATINGPOINTTYPE_H
#define FLOATINGPOINTTYPE_H

#include <core/Type/Type.h>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class FloatingPointType : public Type
{
private:
	FloatingPointType();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Type);
		ar & BOOST_SERIALIZATION_NVP(_legnth);
	}
private:
    int64_t _legnth;
public:
    virtual std::string ToString();
    virtual bool CanExplicitlyConvertTo(Type* other);
    virtual bool CanImplicitlyConvertTo(Type* other);
    virtual bool Equals(Type* other);
    virtual int64_t GetSize();
    FloatingPointType(int64_t length);
    virtual ~FloatingPointType();
};

#endif // FLOATINGPOINTTYPE_H

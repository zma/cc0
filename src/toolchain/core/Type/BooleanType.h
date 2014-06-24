#ifndef BOOLEANTYPE_H
#define BOOLEANTYPE_H

#include "Type.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>


class BooleanType : public Type
{
private:
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Type);
	}
public:
    virtual std::string ToString();
    virtual bool CanExplicitlyConvertTo(Type* other);
    virtual bool CanImplicitlyConvertTo(Type* other);
    virtual bool Equals(Type* other);
    virtual int64_t GetSize();
    BooleanType();
    virtual ~BooleanType();
};

#endif // BOOLEANTYPE_H

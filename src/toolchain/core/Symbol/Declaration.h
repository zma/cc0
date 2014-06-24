#pragma once

#include <string>
#include <stdint.h>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>


class Type;

class Declaration
{
private:
	Declaration();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_NVP(_type);
		ar & BOOST_SERIALIZATION_NVP(_name);
		ar & BOOST_SERIALIZATION_NVP(_offset);
	}
private:
    Type *_type;
    std::string _name;
    int64_t _offset;
public:
    Declaration(std::string name, Type *type);
    virtual ~Declaration();
public:
    Type *GetType();
    std::string GetName();
public:
    int64_t GetOffset();
    void SetOffset(uint64_t offset);
};

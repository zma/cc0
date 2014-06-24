#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <stdint.h>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class Type
{
private:
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_NVP(_specifiers);
	}
public:
    enum TypeSpecifier
    {
        None = 0,
        Extern = 1 << 0,
        Static = 1 << 1,
        Auto=  1<< 2,
        Register = 1 << 3,
        Standalone = 1 << 4,
        Const = 1 << 16,
        Volatile = 1 << 17,
        ForwardDecl = 1 << 31,
        STORAGE_CLASS_MASK = Standalone | Extern | Static
    };
protected:
    Type();
public:
    virtual ~Type();
private:
    TypeSpecifier _specifiers;
public:
    TypeSpecifier GetSpecifiers();
    void SetSpecifiers(TypeSpecifier specifiers);
public:
    virtual int64_t GetSize() = 0;
    virtual bool Equals(Type *other) = 0;
    virtual bool CanImplicitlyConvertTo(Type *other) = 0;
    virtual bool CanExplicitlyConvertTo(Type *other) = 0;
    virtual std::string ToString() = 0;
};

#endif // TYPE_H

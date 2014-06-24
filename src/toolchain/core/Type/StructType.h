#ifndef STRUCTTYPE_H
#define STRUCTTYPE_H
#include "Type.h"
#include <vector>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

class Symbol;
class Declaration;

class StructType: public Type
{
private:
	StructType();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Type);
		ar & BOOST_SERIALIZATION_NVP(_name);
		ar & BOOST_SERIALIZATION_NVP(_layoutMode);
		ar & BOOST_SERIALIZATION_NVP(_fields);
	}
public:
    enum LayoutMode
    {
        Sequential,
        Union
    };
private:
    std::string _name;
    LayoutMode _layoutMode;
    std::vector<Declaration *> _fields;
public:
    StructType(LayoutMode layoutMode);
    StructType(LayoutMode layoutMode, std::string name);
    virtual ~StructType();
public:
    virtual LayoutMode GetLayoutMode();
    virtual int64_t GetSize();
    virtual bool CanImplicitlyConvertTo(Type *other);
    virtual bool CanExplicitlyConvertTo(Type *other);
    std::vector<Declaration *> *GetFieldList();
    virtual std::string ToString() ;
    virtual bool Equals(Type* other);
public:
    std::string GetName();
};

#endif // STRUCTTYPE_H

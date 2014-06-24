#ifndef CONSTANTVALUE_H
#define CONSTANTVALUE_H

#include <core/Type/Type.h>
#include <stdint.h>
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>

class ConstantType;

class ConstantValue
{
private:
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_NVP(_declType);
		ar & BOOST_SERIALIZATION_NVP(_resolvedType);
		ar & BOOST_SERIALIZATION_NVP(_boolValue);
		ar & BOOST_SERIALIZATION_NVP(_intValue);
		ar & BOOST_SERIALIZATION_NVP(_uintValue);
		ar & BOOST_SERIALIZATION_NVP(_charValue);
		ar & BOOST_SERIALIZATION_NVP(_stringValue);
		ar & BOOST_SERIALIZATION_NVP(_doubleValue);
	}
private:
    friend class ConstantType;
    Type* _declType;
    Type *_resolvedType;
    bool _boolValue;
    int64_t _intValue;
    uint64_t _uintValue;
    char _charValue;
    std::string _stringValue;
    double _doubleValue;
public:
    ConstantValue();
    ConstantValue(bool value);
    ConstantValue(int64_t value);
    ConstantValue(uint64_t value);
    ConstantValue(char value);
    ConstantValue(double value);
    ConstantValue(std::string value);
public:
    Type *GetType();
    void SetType(Type *type);
    bool GetBoolValue();
    int64_t GetIntValue();
    uint64_t GetUIntValue();
    char GetCharValue();
    double GetDoubleValue();
    std::string GetStringValue();
public:
    std::string ToString();
};

#endif // CONSTANTVALUE_H

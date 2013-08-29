#ifndef CONSTANTVALUE_H
#define CONSTANTVALUE_H

#include <core/Type/Type.h>
#include <stdint.h>
#include <string>

class ConstantType;

class ConstantValue
{
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

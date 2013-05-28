#include "ConstantValue.h"
#include "core/Core.h"
#include <stdlib.h>
#include <stdio.h>

ConstantValue::ConstantValue()
{
    _declType = new VoidType();
    _resolvedType = NULL;
}

ConstantValue::ConstantValue(bool value)
{
    _declType = new BooleanType();
    if (value)
    {
        _boolValue = true;
        _intValue = 1;
        _uintValue = 1;
        _doubleValue = 1;
        _charValue = (char)1;
    }
    else
    {
    _boolValue = false;
    _intValue = 0;
    _uintValue = 0;
    _doubleValue = 0;
    _charValue = 0;
        
    }
    _resolvedType = NULL;
}

ConstantValue::ConstantValue(int64_t value)
{
    _declType = new IntegerType(8, true);
    _boolValue = (value != 0);
    _intValue = value;
    _uintValue = (uint64_t)value;
    _doubleValue = value;
    _charValue = (char)value;
    _resolvedType = NULL;
}

ConstantValue::ConstantValue(uint64_t value)
{
    _declType = new IntegerType(8, false);
    _boolValue = (value != 0);
    _intValue = (int64_t)value;
    _uintValue = value;
    _doubleValue = value;
    _charValue = (char)value;
    _resolvedType = NULL;
}

ConstantValue::ConstantValue(char value)
{
    _declType = new IntegerType(1, true);
    _boolValue = (value != 0);
    _intValue = (int64_t)value;
    _uintValue = (uint64_t)value;
    _doubleValue = (double)value;
    _charValue = value;
    _resolvedType = NULL;
}

ConstantValue::ConstantValue(std::string value)
{
    _declType = new PointerType(new IntegerType(8, true));
    _stringValue = value;
    _resolvedType = NULL;
}

bool ConstantValue::GetBoolValue()
{
    return _boolValue;
    
}

char ConstantValue::GetCharValue()
{
    return _charValue;

}

int64_t ConstantValue::GetIntValue()
{
    return _intValue;

}


uint64_t ConstantValue::GetUIntValue()
{
    return _uintValue;

}

std::string ConstantValue::GetStringValue()
{
    return _stringValue;
}

Type* ConstantValue::GetType()
{
    if (_resolvedType == NULL)
    {
        return _declType;
    }
    return _resolvedType;
}

std::string ConstantValue::ToString()
{
    char buf[1000];
    Type *type = GetType();
    
    if (typeid(*type) == typeid(IntegerType))
    {
        sprintf(buf, "0x%llx", (long long)_intValue);
        return std::string(buf);
    }
    else if (typeid(*type) == typeid(BooleanType))
    {
            return std::string(_boolValue?"true":"false");
    }
    else if (typeid(*type) == typeid(FloatingPointType))
    {
            sprintf(buf, "%lf", (double)_doubleValue);
            return std::string(buf);
    }
    else if (typeid(*type) == typeid(PointerType) || typeid(*type) == typeid(ArrayType))
    {
        sprintf(buf, "0x%llx", (long long)_intValue);
        return std::string(buf);
    }
    else
    {
        return "<CONSTANT>";
    }
}

ConstantValue::ConstantValue(double value)
{
    _declType = new FloatingPointType(8);
    _boolValue = (value != 0);
    _intValue = (int64_t)value;
    _uintValue = (uint64_t)value;
    _doubleValue = value;
    _charValue = (char)value;
    _resolvedType = NULL;
}

double ConstantValue::GetDoubleValue()
{
    return _doubleValue;
}

void ConstantValue::SetType(Type* type)
{
    _resolvedType = type;
}

#include "IntegerType.h"
#include <stdio.h>
#include <typeinfo>
#include "PointerType.h"
#include "FloatingPointType.h"

IntegerType::IntegerType():_legnth(0), _signedType(false)
{
}

IntegerType::IntegerType(int64_t length, bool signedType)
{
    _legnth = length;
    _signedType = signedType;
}


IntegerType::~IntegerType()
{

}

std::string IntegerType::ToString()
{
    char buf[100];
    sprintf(buf, "%sint%d", _signedType?"":"u", (int)_legnth * 8);
    return std::string(buf);
}

bool IntegerType::CanExplicitlyConvertTo(Type* other)
{
    if (CanImplicitlyConvertTo(other))
    {
        return true;
    }
    else if (typeid(*other) == typeid(IntegerType))
    {
        return true;
    }
    else if (typeid(*other) == typeid(PointerType))
    {
        return true;
    }
    else
    {        
        return false;
    }
}

bool IntegerType::CanImplicitlyConvertTo(Type* other)
{
    if (Equals(other))
    {
        return true;
    }
    else if (typeid(*other) == typeid(IntegerType))
    {
        IntegerType *intType = dynamic_cast<IntegerType *>(other);
        return intType->GetSize() >= this->GetSize();
    }
    else if (typeid(*other) == typeid(FloatingPointType))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool IntegerType::Equals(Type* other)
{
    if (this == other)
    {
        return true;
    }
    else if (typeid(*other) == typeid(IntegerType))
    {
        IntegerType *intType = dynamic_cast<IntegerType *>(other);
        return intType->GetSize() == this->GetSize() && intType->IsSigned() == this->IsSigned();
    }
    else
    {
        return false;
    }   
}

int64_t IntegerType::GetSize()
{
    return _legnth;
}

bool IntegerType::IsSigned()
{
    return _signedType;
}



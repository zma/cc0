#include "FloatingPointType.h"
#include <stdlib.h>
#include <typeinfo>
#include "IntegerType.h"

FloatingPointType::FloatingPointType() : _legnth(0)
{}

std::string FloatingPointType::ToString()
{
    switch (_legnth)
    {
        case 4:
            return "float";
        case 8:
            return "double";
        default:
            abort();
    }
}

bool FloatingPointType::CanExplicitlyConvertTo(Type* other)
{
    if (this->CanImplicitlyConvertTo(other))
    {
        return true;
    }
    else if (typeid(*other) == typeid(FloatingPointType()))
    {
        return true;
    }
    else if (typeid(*other) == typeid(IntegerType))
    {
        return true;
    }
    else
    {        
        return false;
    }
}

bool FloatingPointType::CanImplicitlyConvertTo(Type* other)
{
    if (this->Equals(other))
    {
        return true;
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

bool FloatingPointType::Equals(Type* other)
{
    if (this == other)
    {
        return true;
    }
    else if (typeid(*other) == typeid(FloatingPointType))
    {
        FloatingPointType *intType = dynamic_cast<FloatingPointType *>(other);
        return intType->GetSize() == this->GetSize();
    }
    else
    {
        return false;
    }   
}

int64_t FloatingPointType::GetSize()
{
    return _legnth;
}

FloatingPointType::FloatingPointType(int64_t length)
{
    _legnth = length;
}

FloatingPointType::~FloatingPointType()
{

}


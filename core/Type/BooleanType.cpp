#include "BooleanType.h"
#include <typeinfo>
std::string BooleanType::ToString()
{
    return "bool";
}

bool BooleanType::CanExplicitlyConvertTo(Type* other)
{
    if (this->CanImplicitlyConvertTo(other))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool BooleanType::CanImplicitlyConvertTo(Type* other)
{
    if (this->Equals(other))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool BooleanType::Equals(Type* other)
{
    if (this == other)
    {
        return true;
    }
    else if (typeid(*other) == typeid(BooleanType))
    {
        return true;
    }
    else
    {
        return false;
    }
}

int64_t BooleanType::GetSize()
{
    return 8;
}

BooleanType::BooleanType()
{

}

BooleanType::~BooleanType()
{

}


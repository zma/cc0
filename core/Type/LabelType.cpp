
#include "LabelType.h"
#include <typeinfo>

bool LabelType::CanExplicitlyConvertTo(Type* other)
{
    if (this->CanImplicitlyConvertTo(other))
    {
        return true;
    }
    
    return false;
}

bool LabelType::CanImplicitlyConvertTo(Type* other)
{
    if (this->Equals(other))
    {
        return true;
    }
    
    return false;
}

bool LabelType::Equals(Type* other)
{
    return (typeid(*other) == typeid(LabelType));
}

int64_t LabelType::GetSize()
{
    return 0;
}

std::string LabelType::ToString()
{
    return "LABEL";
}

LabelType::LabelType()
{

}

LabelType::~LabelType()
{

}


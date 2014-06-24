
#include "VoidType.h"
#include <typeinfo>

VoidType::VoidType()
{

}

VoidType::~VoidType()
{

}

std::string VoidType::ToString()
{
    return "void";
}


bool VoidType::CanExplicitlyConvertTo(Type*)
{
    return false;
}

bool VoidType::CanImplicitlyConvertTo(Type*)
{
    return false;
}

bool VoidType::Equals(Type* other)
{
    return (typeid(*other) == typeid(VoidType));
}

int64_t VoidType::GetSize()
{
    return 0;
}


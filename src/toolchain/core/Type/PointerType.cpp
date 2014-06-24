#include "PointerType.h"
#include "IntegerType.h"
#include "VoidType.h"
#include <typeinfo>

PointerType::PointerType():_underlyingType(NULL)
{
}

PointerType::PointerType(Type *underlyingType)
{
    this->_underlyingType = underlyingType;
}

PointerType::~PointerType()
{

}

int64_t PointerType::GetSize()
{
    return PointerSize;
}
bool PointerType::CanExplicitlyConvertTo(Type* other)
{
    if (this->CanImplicitlyConvertTo(other))
    {
        return true;
    }
    
    if (typeid(*other) == typeid(IntegerType))
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
bool PointerType::CanImplicitlyConvertTo(Type* other)
{
    if (this->Equals(other))
    {
        return true;
    }
    
    if (typeid(*other) == typeid(PointerType))
    {
        PointerType *ptype = dynamic_cast<PointerType *>(other);
        if (this->GetUnderlyingType()->CanImplicitlyConvertTo(ptype->GetUnderlyingType()))
        {
            // T* -> S*, if (T->S)
            return true;
        }
        else if (typeid(*(ptype->GetUnderlyingType())) == typeid(VoidType))
        {
            // T* -> void *
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
Type* PointerType::GetUnderlyingType()
{
    return this->_underlyingType;
}

bool PointerType::Equals(Type* other)
{

    
    PointerType *anotherPointer = dynamic_cast<PointerType *>(other);
    if (anotherPointer == NULL)
    {
        return false;
    }
    return (_underlyingType->Equals(anotherPointer->_underlyingType));
}

std::string PointerType::ToString()
{
    return _underlyingType->ToString() + "*";
}



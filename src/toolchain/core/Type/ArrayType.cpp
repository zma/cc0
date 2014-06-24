
#include "ArrayType.h"
#include <core/CodeDom/Expression.h>
#include "PointerType.h"
#include <stdio.h>

ArrayType::ArrayType(): _elementType(NULL), _size(0)
{
}

ArrayType::ArrayType(Type* elementType, int64_t size)

{
    _elementType = elementType;
    _size = size;
}

ArrayType::~ArrayType()
{

}


std::string ArrayType::ToString()
{
    if (_size != -1)
    {    
        char buffer[100];
        sprintf(buffer, "%lld", (long long)_size);
        return _elementType->ToString() + "[" + buffer +  "]";
    }
    else
    {
        return _elementType->ToString() + "[UNKNOWN SIZE]";
    }
}

bool ArrayType::CanExplicitlyConvertTo(Type* other)
{
    if (this->CanImplicitlyConvertTo(other))
    {
        return true;
    }
    
    return false;
}

bool ArrayType::CanImplicitlyConvertTo(Type* other)
{
    if (this->Equals(other))
    {
        return true;
    }
    else
    {
        PointerType *pointerType = dynamic_cast<PointerType *>(other);
        if (pointerType == NULL)
        {
            return false;
        }
        
        if (pointerType->GetUnderlyingType()->CanImplicitlyConvertTo(_elementType))
        {
            return true;
        }
        
        return false;
    }
    
}

bool ArrayType::Equals(Type* other)
{
    ArrayType *arr2 = dynamic_cast<ArrayType *>(other);
    if (arr2 == NULL)
    {
        return false;
    }
    return _elementType->Equals(arr2->GetElementType());
    
}

int64_t ArrayType::GetSize()
{
    //FIXME: 
    return _size * _elementType->GetSize();
}

Type* ArrayType::GetElementType()
{
    return _elementType;
}




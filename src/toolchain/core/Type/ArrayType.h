#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

#include "Type.h"

class Expression;

class ArrayType : public Type
{
private:
    Type * _elementType;
    int64_t _size;
public:
    virtual std::string ToString();
    virtual bool CanExplicitlyConvertTo(Type* other);
    virtual bool CanImplicitlyConvertTo(Type* other);
    virtual bool Equals(Type* other);
    virtual int64_t GetSize();
public:
    ArrayType(Type* elementType, int64_t size = -1);
    virtual ~ArrayType();
public:
    Type * GetElementType();
};

#endif // ARRAYTYPE_H

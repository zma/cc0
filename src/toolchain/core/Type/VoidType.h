#ifndef VOIDTYPE_H
#define VOIDTYPE_H
#include "Type.h"

class VoidType: public Type
{
public:
    VoidType();
    virtual ~VoidType();
public:
    virtual int64_t GetSize();
    virtual bool CanImplicitlyConvertTo(Type *other);
    virtual bool CanExplicitlyConvertTo(Type *other);
    virtual bool Equals(Type *other);
    virtual std::string ToString() ;
};

#endif // VOIDTYPE_H

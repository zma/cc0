#ifndef POINTERTYPE_H
#define POINTERTYPE_H
#include "Type.h"

class PointerType: public Type
{
public:
    // TODO: Make it more portable
    static const int PointerSize = 8;
private:
    Type * _underlyingType;
public:
    PointerType(Type * underlyingType);
    virtual ~PointerType();
public:
    virtual int64_t GetSize();
    virtual bool CanImplicitlyConvertTo(Type *other);
    virtual bool CanExplicitlyConvertTo(Type *other);
    virtual bool Equals(Type *other);
public:
    Type * GetUnderlyingType();
    virtual std::string ToString() ;
};

#endif // POINTERTYPE_H

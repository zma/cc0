#ifndef BOOLEANTYPE_H
#define BOOLEANTYPE_H

#include "Type.h"


class BooleanType : public Type
{

public:
    virtual std::string ToString();
    virtual bool CanExplicitlyConvertTo(Type* other);
    virtual bool CanImplicitlyConvertTo(Type* other);
    virtual bool Equals(Type* other);
    virtual int64_t GetSize();
    BooleanType();
    virtual ~BooleanType();
};

#endif // BOOLEANTYPE_H

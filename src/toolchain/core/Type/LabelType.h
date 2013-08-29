#pragma once
#include "Type.h"


class LabelType : public Type
{

public:
    virtual bool CanExplicitlyConvertTo(Type* other);
    virtual bool CanImplicitlyConvertTo(Type* other);
    virtual bool Equals(Type* other);
    virtual int64_t GetSize();
    LabelType();
    virtual ~LabelType();
    virtual std::string ToString() ;
};


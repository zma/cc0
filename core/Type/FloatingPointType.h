#ifndef FLOATINGPOINTTYPE_H
#define FLOATINGPOINTTYPE_H

#include <core/Type/Type.h>

class FloatingPointType : public Type
{
private:
    int64_t _legnth;
public:
    virtual std::string ToString();
    virtual bool CanExplicitlyConvertTo(Type* other);
    virtual bool CanImplicitlyConvertTo(Type* other);
    virtual bool Equals(Type* other);
    virtual int64_t GetSize();
    FloatingPointType(int64_t length);
    virtual ~FloatingPointType();
};

#endif // FLOATINGPOINTTYPE_H

#ifndef INTEGERTYPE_H
#define INTEGERTYPE_H

#include <core/Type/Type.h>

class IntegerType : public Type
{
private:
    int64_t _legnth;
    bool _signedType;
public:
    IntegerType(int64_t length, bool signedType);
    virtual ~IntegerType();
public:
    virtual std::string ToString();
    virtual bool CanExplicitlyConvertTo(Type* other);
    virtual bool CanImplicitlyConvertTo(Type* other);
    virtual bool Equals(Type* other);
    virtual int64_t GetSize();
public:
    bool IsSigned();
};

#endif // INTEGERTYPE_H

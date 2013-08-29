#pragma once
#include "Type.h"
#include <vector>

class SymbolScope;
class Declaration;

class FunctionType: public Type
{
private:
    std::vector<Declaration *> *_parameters;
    Type *_returnType;
public:
    FunctionType(Type *returnType, std::vector<Declaration *> *parameters);
    virtual ~FunctionType();
public:
    virtual int64_t GetSize();
    virtual bool CanImplicitlyConvertTo(Type *other);
    virtual bool CanExplicitlyConvertTo(Type *other);
    virtual bool Equals(Type *other);
public:
    std::vector<Declaration *> *GetParameters();
    Type * GetReturnType();
    virtual std::string ToString() ;
};

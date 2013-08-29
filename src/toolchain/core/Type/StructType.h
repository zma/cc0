#ifndef STRUCTTYPE_H
#define STRUCTTYPE_H
#include "Type.h"
#include <vector>

class Symbol;
class Declaration;

class StructType: public Type
{
public:
    enum LayoutMode
    {
        Sequential,
        Union
    };
private:
    std::string _name;
    LayoutMode _layoutMode;
    std::vector<Declaration *> _fields;
public:
    StructType(LayoutMode layoutMode);
    StructType(LayoutMode layoutMode, std::string name);
    virtual ~StructType();
public:
    virtual LayoutMode GetLayoutMode();
    virtual int64_t GetSize();
    virtual bool CanImplicitlyConvertTo(Type *other);
    virtual bool CanExplicitlyConvertTo(Type *other);
    std::vector<Declaration *> *GetFieldList();
    virtual std::string ToString() ;
    virtual bool Equals(Type* other);
public:
    std::string GetName();
};

#endif // STRUCTTYPE_H

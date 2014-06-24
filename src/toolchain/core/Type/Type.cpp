
#include "Type.h"

Type::Type() : _specifiers(None)
{
}

Type::~Type()
{

}

Type::TypeSpecifier Type::GetSpecifiers()
{
    return _specifiers;
}

void Type::SetSpecifiers(Type::TypeSpecifier specifiers)
{
    _specifiers = specifiers;
}


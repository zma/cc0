
#include "Type.h"

Type::Type()
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


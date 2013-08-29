
#include "Declaration.h"
#include <stdio.h>
#include <core/Type/Type.h>

Declaration::Declaration(std::string name, Type * type)
{
    _name = name;
    _type = type;
}

Declaration::~Declaration()
{
 
}

std::string Declaration::GetName()
{
    return this->_name;
}

Type* Declaration::GetType()
{
    return _type;
}

int64_t Declaration::GetOffset()
{
    return _offset;
}

void Declaration::SetOffset(uint64_t offset)
{
    _offset = offset;
}

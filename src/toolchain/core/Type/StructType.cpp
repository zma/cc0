
#include "StructType.h"
#include <core/Symbol/Declaration.h>
#include <core/Symbol/Symbol.h>

StructType::StructType():_layoutMode(Sequential)
{
}

StructType::StructType(LayoutMode layout)
{
    _layoutMode = layout;
    _name = "";
}

StructType::StructType(StructType::LayoutMode layoutMode, std::string name)
{
    _layoutMode = layoutMode;
    _name = name;
}


StructType::~StructType()
{
}

int64_t StructType::GetSize()
{
    //FIXME: Maybe we can optimize it by pre-calculating it
    
    int totalSize = 0;
    for (std::vector<Declaration *>::iterator it = _fields.begin(); it != _fields.end(); ++it)
    {
        totalSize += (*it)->GetType()->GetSize();
    }
    
    return totalSize;
}

bool StructType::CanImplicitlyConvertTo(Type* other)
{
    if (other == this)
    {
        // FIXME
        return true;
    }
    
    
    return false;
}

bool StructType::CanExplicitlyConvertTo(Type* other)
{
    if (other == this)
    {
        // FIXME
        return true;
    }
    
    return false;
}

std::vector<Declaration* >* StructType::GetFieldList()
{
    return &(this->_fields);
}

StructType::LayoutMode StructType::GetLayoutMode()
{
    return _layoutMode;
}

std::string StructType::ToString()
{
    return "struct " + _name;
}


bool StructType::Equals(Type* other)
{
    return (other == this);
}

std::string StructType::GetName()
{
    return _name;
}



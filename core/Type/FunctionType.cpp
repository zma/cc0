
#include "FunctionType.h"
#include <core/Symbol/Declaration.h>



FunctionType::FunctionType(Type* returnType, std::vector< Declaration* >* parameters)
{
    _returnType = returnType;
    _parameters = parameters;
}

FunctionType::~FunctionType()
{

}

std::string FunctionType::ToString()
{
    std::string str = "FUNCTION (";
    for (int i = 0; i < _parameters->size(); i++)
    {
        Declaration *decl = (*_parameters)[i];
        std::string name = decl->GetName();
        Type *type = decl->GetType();
        if (name != "")
        {
            str.append(name);
            str.append(":");
        }
        
        str.append(type->ToString());
        
        if (i != _parameters->size() - 1)
        {
            str.append(", ");
        }
    }
    str.append(") =>");
    str.append(_returnType->ToString());
    return str;
}

int64_t FunctionType::GetSize()
{
    return 0;
}



bool FunctionType::CanExplicitlyConvertTo(Type* other)
{
    return false;
}

bool FunctionType::CanImplicitlyConvertTo(Type* other)
{
    return false;
}

std::vector< Declaration* >* FunctionType::GetParameters()
{
    return this->_parameters;
}

Type* FunctionType::GetReturnType()
{
    return this->_returnType;
}

bool FunctionType::Equals(Type* other)
{
    FunctionType *func2 = dynamic_cast<FunctionType *>(other);

    if (func2 == NULL)
    {
        return false;
    }
    
    if (this->_returnType->Equals(func2->_returnType))
    {
        return false;
    }

    if (this->_parameters->size() != func2->_parameters->size())
    {
        return false;
    }

    for (std::vector<Declaration *>::iterator it = this->_parameters->begin(), it2 = func2->_parameters->begin();
            it != this->_parameters->end();
            it++, it2++
        )
    {
        Type *type1 = (*it)->GetType();
        Type *type2 = (*it2)->GetType();
        if (!type1->Equals(type2))
        {
            return false;
        }
    }
    
    return true;

}

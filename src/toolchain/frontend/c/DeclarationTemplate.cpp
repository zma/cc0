#include <core/Core.h>
#include "DeclarationTemplate.h"
#include <stdlib.h>
#include <core/Pass/ConstantPropagation.h>
#include <assert.h>

DeclarationTemplate::DeclarationTemplate(Type* baseType)
{
    _baseType = baseType;
    _parent = NULL;
    _name = "";
}

DeclarationTemplate::DeclarationTemplate(DeclarationTemplate* parentDecl)
{
    _baseType = NULL;
    _parent = parentDecl;
    _name = "";
}

DeclarationTemplate::~DeclarationTemplate()
{

}

Type* DeclarationTemplate::GetDerivedType(Type* baseType, DeclarationTemplate::DeclarationSpecifier spec)
{
    ConstantPropagation *constEval;
    ConstantValue *v;
    int64_t arraySize;
    Type *newType;
    switch (spec.Kind)
    {
        case DeclarationTemplate::Array:
            constEval = new ConstantPropagation();
            v = constEval->Evaluate(spec.ArraySize);
            delete constEval;
            constEval = NULL;
            
            if (v == NULL)
            {
                arraySize = -1;
            }
            else if (v->GetType()->CanImplicitlyConvertTo(new IntegerType(8, true)))
            {
                arraySize = v->GetIntValue();
            }
        
            newType = new ArrayType(baseType, arraySize);
            break;
        case DeclarationTemplate::Pointer:
            newType = new PointerType(baseType);
            break;
        case DeclarationTemplate::Function:
            newType =  new FunctionType(baseType, spec.Parameters);
            break;
        default:
            abort();
    }      
    
    newType->SetSpecifiers((Type::TypeSpecifier)(newType->GetSpecifiers() | (baseType->GetSpecifiers() | Type::STORAGE_CLASS_MASK)));
    return newType;
}

Type* DeclarationTemplate::GetType()
{
    Type *type;
    
    if (_parent != NULL)
    {
        type = _parent->GetType();
    }
    else
    {
        type    = _baseType;
    }

    for (size_t i = 0; i < _pointers.size(); i++)
    {
        DeclarationSpecifier spec = _pointers[i];
        type = GetDerivedType(type, spec);
    }
    
    for (size_t i = _suffixes.size(); i > 0;i--)
    {
        DeclarationSpecifier spec = _suffixes[i - 1];
        type = GetDerivedType(type, spec);
    }

    return type;
}

std::vector< DeclarationTemplate::DeclarationSpecifier >* DeclarationTemplate::GetSuffixList()
{
    return &_suffixes;
}

std::string DeclarationTemplate::GetName()
{
    return _name;
}

void DeclarationTemplate::SetName(std::string name)
{
    _name = "";
    _name.append(name);
}


std::vector< DeclarationTemplate::DeclarationSpecifier >* DeclarationTemplate::GetPointerList()
{       
    return &_pointers;
}

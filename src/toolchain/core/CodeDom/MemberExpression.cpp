#include "MemberExpression.h"
#include "ExpressionVisitor.h"
#include <core/Type/StructType.h>
#include <vector>
#include <core/Symbol/Declaration.h>
#include <stdlib.h>
#include <assert.h>

MemberExpression::MemberExpression(): Expression(Expression::Member), Container(NULL)
{
}

Type* MemberExpression::GetType()
{
    Type *containerType = Container->GetType();
    
    StructType *structType = dynamic_cast<StructType *>(containerType);
    assert (structType != NULL);
    
    Declaration *targetField = NULL;
    std::vector<Declaration *> *fields = structType->GetFieldList();
    for (std::vector<Declaration *>::iterator it = fields->begin(); it != fields->end(); ++it)
    {
        Declaration *decl = *it;
        if (decl->GetName() == FieldName)
        {
            targetField = decl;
            break;
        }
    }
    assert(targetField != NULL);
    
    return targetField->GetType();
}

Expression* MemberExpression::GetLValue()
{
    return this;
}

void MemberExpression::Accept(ExpressionVisitor* visitor)
{
    visitor->Visit(this);
}

MemberExpression::MemberExpression(Expression *container, std::string fieldName)
    : Expression(Expression::Member)
{
    Container = container;
    FieldName = fieldName;
}


MemberExpression::~MemberExpression()
{

}

Expression* MemberExpression::GetContainer()
{
    return Container;
}

std::string MemberExpression::GetFieldName()
{
    return FieldName;
}


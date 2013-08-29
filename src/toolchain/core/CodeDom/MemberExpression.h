#ifndef MEMBEREXPRESSION_H
#define MEMBEREXPRESSION_H

#include <core/CodeDom/Expression.h>
#include <string>

class MemberExpression : public Expression
{
public:
    Expression *Container;
    std::string FieldName;
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    MemberExpression(Expression *container, std::string fieldName);
    virtual ~MemberExpression();
public:
    Expression *GetContainer();
    std::string GetFieldName();
};

#endif // MEMBEREXPRESSION_H

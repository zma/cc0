#ifndef GOTOEXPRESSION_H
#define GOTOEXPRESSION_H

#include "Expression.h"
#include <string>

class GotoExpression : public Expression
{
public:
    enum GotoTargetType
    {
        NamedLabel,
        Continue,
        Break
    };
private:
    GotoTargetType _targetType;
    std::string _labelName;
    
public:
    GotoExpression(GotoTargetType targetType);
    GotoExpression(std::string labelName);
    virtual ~GotoExpression();
public:
    virtual void Accept(ExpressionVisitor *visitor);
    virtual Expression * GetLValue();
    virtual Type * GetType();
public:
    GotoTargetType GetTargetType();
    std::string GetLabelName();
};

#endif // GOTOEXPRESSION_H

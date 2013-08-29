#ifndef INLINEASSEMBLYEXPRESSION_H
#define INLINEASSEMBLYEXPRESSION_H

#include <core/CodeDom/Expression.h>


class InlineAssemblyExpression : public Expression
{
private:
    std::string _fmt;
    std::vector<Expression *> *_args;
public:
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    InlineAssemblyExpression(std::string fmt);
    InlineAssemblyExpression(std::string fmt, std::vector<Expression *> *args);
    virtual ~InlineAssemblyExpression();
public:
    std::string GetTemplateString();
};

#endif // INLINEASSEMBLYEXPRESSION_H

#ifndef CONDITIONALEXPRESSION_H
#define CONDITIONALEXPRESSION_H

#include "Expression.h"


class ConditionalExpression : public Expression
{
private:
    std::map<Expression *, Expression *> *_conditionalBranches;
    Expression *_defaultBranch;
    bool _goThrough;
public:
    ConditionalExpression();
    virtual ~ConditionalExpression();
public:
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
public:
    std::map<Expression *, Expression *> *GetConditionalBranches();
    void SetConditionalBranches(std::map<Expression *, Expression *> *branches);
    Expression *GetDefaultBranch();
    void SetDefaultBranch(Expression *expr);
    bool GoThrough();
    void SetGoThrough(bool v);
};

#endif // CONDITIONALEXPRESSION_H

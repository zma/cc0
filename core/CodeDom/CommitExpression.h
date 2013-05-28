#ifndef COMMITEXPRESSION_H
#define COMMITEXPRESSION_H
#include "Expression.h"

class CommitExpression: public Expression
{
private:
    bool _deleteWatcher;
public:
    CommitExpression(bool deleteWatcher);
    virtual ~CommitExpression();
    virtual void Accept(ExpressionVisitor *visitor);
    virtual Expression * GetLValue();
    virtual Type * GetType();
public:
    bool GetDeleteWatcher();
};

#endif // COMMITEXPRESSION_H

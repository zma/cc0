#ifndef ABORTEXPRESSION_H
#define ABORTEXPRESSION_H

#include <core/CodeDom/Expression.h>

class AbortExpression : public Expression
{
private:
    bool _deleteWatcher;
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    AbortExpression(bool deleteWatcher);
    virtual ~AbortExpression();
public:
    bool GetDeleteWatcher();
};

#endif // ABORTEXPRESSION_H

#ifndef LOOPEXPRESSION_H
#define LOOPEXPRESSION_H

#include "Expression.h"

/*
 * 
 * {
 *      init;
 * enter:
 *      body;
 * continue:
 *      incr;
 *      if (cond) goto enter;
 * break:
 * 
 * }
 * 
 * 
 * {
 *      init;
 * enter:
 *      if (!cond) goto break;
 *      body;
 * continue:
 *      incr;
 *      goto enter;
 * }
 * 
 */


class LoopExpression : public Expression
{
private:
    Expression *_initializingExpression;
    Expression *_conditionExpression;
    Expression *_incrementExpression;
    Expression *_bodyExpression;
    bool _checkCondAfter;
public:
    LoopExpression(Expression *init, Expression *cond, Expression *incr, Expression *body, bool checkAfter);
    virtual ~LoopExpression();
public:
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
public:
    Expression *GetInitializingExpression();
    Expression *GetConditionExpression();
    Expression *GetIncrementExpression();
    Expression *GetBodyExpression();
    bool CheckConditionAfter();
};

#endif // LOOPEXPRESSION_H

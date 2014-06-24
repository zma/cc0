#ifndef LOOPEXPRESSION_H
#define LOOPEXPRESSION_H

#include "Expression.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

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
	LoopExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_initializingExpression);
		ar & BOOST_SERIALIZATION_NVP(_conditionExpression);
		ar & BOOST_SERIALIZATION_NVP(_incrementExpression);
		ar & BOOST_SERIALIZATION_NVP(_bodyExpression);
		ar & BOOST_SERIALIZATION_NVP(_checkCondAfter);
	}
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

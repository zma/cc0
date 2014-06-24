#ifndef ASSIGNEXPRESSION_H
#define ASSIGNEXPRESSION_H

#include "Expression.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class AssignExpression : public Expression
{
private:
	AssignExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_additionOperator);
		ar & BOOST_SERIALIZATION_NVP(_target);
		ar & BOOST_SERIALIZATION_NVP(_source);
	}
private:
    Expression::ExpressionAction _additionOperator;
    Expression *_target;
    Expression *_source;
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    AssignExpression(Expression *target, Expression *source);
    AssignExpression(Expression *target, Expression *source, Expression::ExpressionAction additionalOp);
    virtual ~AssignExpression();
public:
    Expression *GetTarget();
    Expression *GetSource();
    void SetSource(Expression *source);
    void SetTarget(Expression *target);
    Expression::ExpressionAction GetAdditionalOperator();
};

#endif // ASSIGNEXPRESSION_H

#pragma once 

#include "Expression.h"

#include <vector>
#include <string>
#include <boost/serialization/vector.hpp>

class CallExpression : public Expression
{
private:
	CallExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_target);
		ar & BOOST_SERIALIZATION_NVP(_arguments);
	}
private:
    Expression *_target;
    std::vector<Expression *> _arguments;
public:
    CallExpression(Expression *target);
    CallExpression(Expression *target, std::vector<Expression *> *arguments);
    virtual ~CallExpression();
public:
    virtual void Accept(ExpressionVisitor *visitor);
    virtual Expression * GetLValue();
    virtual Type * GetType();
public:
    Expression * GetTarget();
    std::vector<Expression *> * GetArgumentList();
};

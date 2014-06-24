#ifndef RETURNEXPRESSION_H
#define RETURNEXPRESSION_H
#include "Expression.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>


class ReturnExpression : public Expression
{
private:
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_value);
	}
private:
    Expression *_value;
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    ReturnExpression();
    ReturnExpression(Expression *value);
    virtual ~ReturnExpression();
public:
    Expression *GetValue();
};

#endif // RETURNEXPRESSION_H

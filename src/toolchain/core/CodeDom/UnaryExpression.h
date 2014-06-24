#ifndef UNARYEXPRESSION_H
#define UNARYEXPRESSION_H
#include "Expression.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class UnaryExpression: public Expression
{
private:
	UnaryExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(Operand);
	}
public:
    Expression *Operand;
public:
    UnaryExpression(ExpressionAction nodeType, Expression *operand);
    virtual ~UnaryExpression();
public:
    virtual void Accept(ExpressionVisitor *visitor);
    virtual Expression * GetLValue();    
    virtual Type * GetType();
public:
    Expression * GetOperand();
};

#endif // UNARYEXPRESSION_H

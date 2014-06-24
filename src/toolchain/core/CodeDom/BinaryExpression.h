#ifndef BINARYEXPRESSION_H
#define BINARYEXPRESSION_H

#include "Expression.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class BinaryExpression : public Expression
{
private:
	BinaryExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_left);
		ar & BOOST_SERIALIZATION_NVP(_right);
	}
private:
    Expression *_left, *_right;
public:
    BinaryExpression(ExpressionAction nodeType, Expression *left, Expression *right);
    virtual ~BinaryExpression();
public:
    virtual void Accept(ExpressionVisitor *visitor);
    virtual Expression * GetLValue();
public:
    Expression *GetLeftOperand();
    Expression *GetRightOperand();
    void SetLeftOperand(Expression *expr);
    void SetRightOperand(Expression *expr);
};

#endif // BINARYEXPRESSION_H

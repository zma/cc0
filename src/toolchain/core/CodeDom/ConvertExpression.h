#ifndef CONVERTEXPRESSION_H
#define CONVERTEXPRESSION_H

#include "Expression.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class Type;

class ConvertExpression : public Expression
{
private:
	ConvertExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_targetType);
		ar & BOOST_SERIALIZATION_NVP(_source);
	}
private:
    Type *_targetType;
    Expression * _source;
public:
    ConvertExpression(Type *targetType, Expression *sourceExpression);
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    virtual ~ConvertExpression();
public:
    Type *GetTargetType();
    Expression *GetSourceExpression();
};

#endif // CONVERTEXPRESSION_H

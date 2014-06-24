#ifndef CONSTANTEXPRESSION_H
#define CONSTANTEXPRESSION_H

#include "Expression.h"
#include <string>
#include <core/Type/Type.h>
#include "core/Misc/ConstantValue.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class ConstantValue;
class ConstantExpression : public Expression
{
private:
	ConstantExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_value);
	}
private:
    ConstantValue *_value;
public:
    
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
public:
    ConstantExpression(ConstantValue *value);
    virtual ~ConstantExpression();
public:
    ConstantValue *GetValue();
};

#endif // CONSTANTEXPRESSION_H

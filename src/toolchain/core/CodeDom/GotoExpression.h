#ifndef GOTOEXPRESSION_H
#define GOTOEXPRESSION_H

#include "Expression.h"
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>

class GotoExpression : public Expression
{
private:
	GotoExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_targetType);
		ar & BOOST_SERIALIZATION_NVP(_labelName);
	}
public:
    enum GotoTargetType
    {
        NamedLabel,
        Continue,
        Break
    };
private:
    GotoTargetType _targetType;
    std::string _labelName;
    
public:
    GotoExpression(GotoTargetType targetType);
    GotoExpression(std::string labelName);
    virtual ~GotoExpression();
public:
    virtual void Accept(ExpressionVisitor *visitor);
    virtual Expression * GetLValue();
    virtual Type * GetType();
public:
    GotoTargetType GetTargetType();
    std::string GetLabelName();
};

#endif // GOTOEXPRESSION_H

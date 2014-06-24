#ifndef MEMBEREXPRESSION_H
#define MEMBEREXPRESSION_H

#include <core/CodeDom/Expression.h>
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/string.hpp>

class MemberExpression : public Expression
{
private:
	MemberExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(Container);
		ar & BOOST_SERIALIZATION_NVP(FieldName);
	}
public:
    Expression *Container;
    std::string FieldName;
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    MemberExpression(Expression *container, std::string fieldName);
    virtual ~MemberExpression();
public:
    Expression *GetContainer();
    std::string GetFieldName();
};

#endif // MEMBEREXPRESSION_H

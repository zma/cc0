#ifndef ARRAYSEGMENTEXPRESSION_H
#define ARRAYSEGMENTEXPRESSION_H

#include <core/CodeDom/Expression.h>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class ArraySegmentExpression : public Expression
{
private:
	ArraySegmentExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_container);
		ar & BOOST_SERIALIZATION_NVP(_start);
		ar & BOOST_SERIALIZATION_NVP(_end);
	}
private:
    Expression *_container;
    Expression *_start;
    Expression *_end;
public:
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    ArraySegmentExpression(Expression *array, Expression *start, Expression *end);
    virtual ~ArraySegmentExpression();
public:
    Expression *GetArray();
    Expression *GetStartIndex();
    Expression *GetEndIndex();
};

#endif // ARRAYSEGMENTEXPRESSION_H

#ifndef INDEXEXPRESSION_H
#define INDEXEXPRESSION_H

#include "Expression.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class IndexExpression : public Expression
{
private:
	IndexExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(Container);
		ar & BOOST_SERIALIZATION_NVP(Index);
	}
public:
    Expression *Container;
    Expression *Index;
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    IndexExpression(Expression *container, Expression *index);
    virtual ~IndexExpression();
public:
    Expression *GetContainer();
    Expression *GetIndex();
};

#endif // INDEXEXPRESSION_H

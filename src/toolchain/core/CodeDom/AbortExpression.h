#ifndef ABORTEXPRESSION_H
#define ABORTEXPRESSION_H

#include <core/CodeDom/Expression.h>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class AbortExpression : public Expression
{
private:
	AbortExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_deleteWatcher);
	}
private:
    bool _deleteWatcher;
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    AbortExpression(bool deleteWatcher);
    virtual ~AbortExpression();
public:
    bool GetDeleteWatcher();
};

#endif // ABORTEXPRESSION_H

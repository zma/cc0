#ifndef COMMITEXPRESSION_H
#define COMMITEXPRESSION_H
#include "Expression.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class CommitExpression: public Expression
{
private:
	CommitExpression();
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
    CommitExpression(bool deleteWatcher);
    virtual ~CommitExpression();
    virtual void Accept(ExpressionVisitor *visitor);
    virtual Expression * GetLValue();
    virtual Type * GetType();
public:
    bool GetDeleteWatcher();
};

#endif // COMMITEXPRESSION_H

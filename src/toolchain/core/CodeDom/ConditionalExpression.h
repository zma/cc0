#ifndef CONDITIONALEXPRESSION_H
#define CONDITIONALEXPRESSION_H

#include "Expression.h"
#include <boost/serialization/map.hpp>


class ConditionalExpression : public Expression
{
private:
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_conditionalBranches);
		ar & BOOST_SERIALIZATION_NVP(_defaultBranch);
		ar & BOOST_SERIALIZATION_NVP(_goThrough);
	}
private:
    std::map<Expression *, Expression *> *_conditionalBranches;
    Expression *_defaultBranch;
    bool _goThrough;
public:
    ConditionalExpression();
    virtual ~ConditionalExpression();
public:
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
public:
    std::map<Expression *, Expression *> *GetConditionalBranches();
    void SetConditionalBranches(std::map<Expression *, Expression *> *branches);
    Expression *GetDefaultBranch();
    void SetDefaultBranch(Expression *expr);
    bool GoThrough();
    void SetGoThrough(bool v);
};

#endif // CONDITIONALEXPRESSION_H

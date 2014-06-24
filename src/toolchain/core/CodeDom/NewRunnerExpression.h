#ifndef NEWRUNNEREXPRESSION_H
#define NEWRUNNEREXPRESSION_H

#include <core/CodeDom/Expression.h>
#include <boost/serialization/vector.hpp>

// struct AddressRange
// {
//     int64_t Base;
//     int64_t Size;
// };
// 
// struct RangeMetadata
// {
//     int64_t Count;
//     AddressRange Ranges[0];
// };

class NewRunnerExpression : public Expression
{
private:
	NewRunnerExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_target);
		ar & BOOST_SERIALIZATION_NVP(_arguments);
		ar & BOOST_SERIALIZATION_NVP(_usingList);
		ar & BOOST_SERIALIZATION_NVP(_watchList);
		ar & BOOST_SERIALIZATION_NVP(_space);
	}
private:
    Expression *_target;
    std::vector<Expression *> *_arguments;
    std::vector<Expression *> *_usingList;
    std::vector<Expression *> *_watchList;
    std::vector<Expression *> *_space;
public:
    NewRunnerExpression(Expression *target, std::vector<Expression *> *arguments, std::vector<Expression *> *watchingList, std::vector<Expression *> *usingList, std::vector<Expression *> *space);
    virtual ~NewRunnerExpression();
public:
    Expression * GetTarget();
    std::vector<Expression *> * GetWatchingList();
    std::vector<Expression *> * GetUsingList();
    std::vector<Expression *> * GetArgumentList();
    std::vector<Expression *> * GetSpace();
public:
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
};

#endif // NEWRUNNEREXPRESSION_H

#ifndef PROGRAMEXPRESSION_H
#define PROGRAMEXPRESSION_H

#include "Expression.h"
#include <vector>
#include <core/Symbol/SymbolScope.h>
#include <boost/serialization/vector.hpp>

class SymbolScope;
class FunctionExpression;

class ProgramExpression : public Expression
{
private:
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_functions);
	}
private:
    std::vector<Expression *> _functions;
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
public:
    ProgramExpression();
    virtual ~ProgramExpression();
public:
    std::vector<Expression *> *GetFunctions();
};

#endif // PROGRAMEXPRESSION_H

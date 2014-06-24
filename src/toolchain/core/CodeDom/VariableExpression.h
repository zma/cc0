#ifndef VARIABLEEXPRESSION_H
#define VARIABLEEXPRESSION_H

#include "Expression.h"
#include "core/Symbol/Symbol.h"
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class Symbol;

class VariableExpression : public Expression
{
private:
	VariableExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_symbol);
	}
private:
    Symbol *_symbol;    
public:
    VariableExpression(Symbol *symbol);
    Symbol * GetSymbol();
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    virtual ~VariableExpression();
};

#endif // VARIABLEEXPRESSION_H

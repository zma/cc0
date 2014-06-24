#ifndef LABELEXPRESSION_H
#define LABELEXPRESSION_H

#include "Expression.h"
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class LabelExpression : public Expression
{
private:
	LabelExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		//ar & BOOST_SERIALIZATION_NVP(_labelID);
		//FIXME: serialize static field of _labelID of LabelExpression
		ar & BOOST_SERIALIZATION_NVP(_symbol);
	}
private:
    static unsigned int _labelID;
    Symbol *_symbol;
public:
    virtual Type* GetType();
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
public:
    LabelExpression(Symbol *symbol);
    virtual ~LabelExpression();
public:
    std::string GetName();
    Symbol *GetSymbol();
};

#endif // LABELEXPRESSION_H

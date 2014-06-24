#ifndef INLINEASSEMBLYEXPRESSION_H
#define INLINEASSEMBLYEXPRESSION_H

#include <core/CodeDom/Expression.h>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

class InlineAssemblyExpression : public Expression
{
private:
	InlineAssemblyExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_fmt);
		ar & BOOST_SERIALIZATION_NVP(_args);
	}
private:
    std::string _fmt;
    std::vector<Expression *> *_args;
public:
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    InlineAssemblyExpression(std::string fmt);
    InlineAssemblyExpression(std::string fmt, std::vector<Expression *> *args);
    virtual ~InlineAssemblyExpression();
public:
    std::string GetTemplateString();
};

#endif // INLINEASSEMBLYEXPRESSION_H

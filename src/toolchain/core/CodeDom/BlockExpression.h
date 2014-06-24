#ifndef BLOCKEXPRESSION_H
#define BLOCKEXPRESSION_H
#include "Expression.h"

#include <vector>
#include <boost/serialization/vector.hpp>

class BlockExpression : public Expression
{
private:
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_expressions);
		ar & BOOST_SERIALIZATION_NVP(_scope);
	}
private:
    std::vector<Expression *> *_expressions;
public:
    BlockExpression();
    virtual ~BlockExpression();
    SymbolScope *_scope;
public:
    virtual void Accept(ExpressionVisitor *visitor);
    virtual Expression * GetLValue();
    virtual Type * GetType();
public:
    std::vector<Expression *> * GetExpressionList();
    void SetExpressionList(std::vector<Expression *> *exprList);
    SymbolScope *GetBlockScope();
    void *SetBlockScope(SymbolScope *scope);
};

#endif // BLOCKEXPRESSION_H

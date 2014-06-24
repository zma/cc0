#ifndef FUNCTIONEXPRESSION_H
#define FUNCTIONEXPRESSION_H
#include "Expression.h"
#include <vector>
#include <string>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>

class FunctionType;
class BlockExpression;
class Declaration;
class Type;

class FunctionExpression : public Expression
{
private:
	FunctionExpression();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Expression);
		ar & BOOST_SERIALIZATION_NVP(_body);
		ar & BOOST_SERIALIZATION_NVP(_symbol);
		ar & BOOST_SERIALIZATION_NVP(_functionScope);
	}
private:
    BlockExpression *_body;
    Symbol *_symbol;
    SymbolScope *_functionScope;
public:
    FunctionExpression(Symbol *symbol, SymbolScope *functionScope);
    virtual ~FunctionExpression();
public:
    virtual Expression* GetLValue();
    virtual void Accept(ExpressionVisitor* visitor);
    virtual Type * GetType();
public:
    Symbol *GetSymbol();
    std::string GetName();
    BlockExpression *GetBody();
    void SetBody(BlockExpression *body);
    Type * GetReturnType();
    std::vector<Declaration *> *GetParameters();
    SymbolScope *GetFunctionScope();
    
};

#endif // FUNCTIONEXPRESSION_H

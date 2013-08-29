#pragma once 

#include <core/CodeDom/ExpressionVisitor.h>
#include <core/Misc/ConstantValue.h>


class ConstantPropagation : public ExpressionVisitor
{
private:
    ConstantValue _evaluatedValue;
public:
    ConstantValue *Evaluate(Expression *expr);
public:
    virtual void Visit(AbortExpression* node);
    virtual void Visit(CommitExpression* node);
    virtual void Visit(NewRunnerExpression* node);
    virtual void Visit(ArraySegmentExpression* node);
    virtual void Visit(AssignExpression* node);
    virtual void Visit(NopExpression* node);
    virtual void Visit(CallExpression* node);
    virtual void Visit(ProgramExpression* node);
    virtual void Visit(FunctionExpression* node);
    virtual void Visit(ReturnExpression* node);
    virtual void Visit(ConvertExpression* node);
    virtual void Visit(UnaryExpression* node);
    virtual void Visit(VariableExpression* node);
    virtual void Visit(MemberExpression* node);
    virtual void Visit(LoopExpression* node);
    virtual void Visit(LabelExpression* node);
    virtual void Visit(IndexExpression* node);
    virtual void Visit(GotoExpression* node);
    virtual void Visit(ConstantExpression* node);
    virtual void Visit(ConditionalExpression* node);
    virtual void Visit(BlockExpression* node);
    virtual void Visit(BinaryExpression* node);
    virtual void Visit(InlineAssemblyExpression* node);
    virtual void Reset();
    ConstantPropagation();
    virtual ~ConstantPropagation();
private:
    ConstantValue* GetConstant(Expression *e);
    void SetConstant(Expression* e, ConstantValue *v);
    Expression* Propagate(Expression* e);
};


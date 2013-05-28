#ifndef EXPRESSIONTREEDUMPER_H
#define EXPRESSIONTREEDUMPER_H

#include "ExpressionVisitor.h"
#include <string>
#include "Expression.h"


class ExpressionTreeDumper : public ExpressionVisitor
{
private:
    int _indentLevel;
    void MakeIndent();
public:
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
    virtual void Visit(CallExpression *node);
    virtual void Visit(NopExpression *node);
    virtual void Visit(AssignExpression *node);
    virtual void Visit(ProgramExpression *node);
    virtual void Visit(ArraySegmentExpression *node);
    virtual void Visit(NewRunnerExpression *node);
    virtual void Visit(CommitExpression *node);
    virtual void Visit(AbortExpression *node);
    virtual void Visit(InlineAssemblyExpression* node);
    ExpressionTreeDumper();
    virtual ~ExpressionTreeDumper();
private:
    std::string GetOpeartorString(Expression::ExpressionAction type);
};

#endif // EXPRESSIONTREEDUMPER_H

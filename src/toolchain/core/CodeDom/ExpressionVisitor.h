#ifndef EXPRESSIONVISITOR_H
#define EXPRESSIONVISITOR_H


class InlineAssemblyExpression;
class ProgramExpression;
class Expression;

class FunctionExpression;
class ReturnExpression;
class BinaryExpression;
class BlockExpression;
class ConditionalExpression;
class ConstantExpression;
class GotoExpression;
class IndexExpression;
class LabelExpression;
class LoopExpression;
class MemberExpression;
class VariableExpression;
class UnaryExpression;
class ConvertExpression;
class CallExpression;
class NopExpression;
class AssignExpression;
class CommitExpression;
class AbortExpression;
class NewRunnerExpression;
class ArraySegmentExpression;

class ExpressionVisitor
{
public:
    ExpressionVisitor();
    virtual ~ExpressionVisitor();
public:
    virtual void Visit(BinaryExpression *node) = 0;
    virtual void Visit(BlockExpression *node) = 0;
    virtual void Visit(ConditionalExpression *node) = 0;
    virtual void Visit(ConstantExpression *node) = 0;
    virtual void Visit(GotoExpression *node) = 0;
    virtual void Visit(IndexExpression *node) = 0;
    virtual void Visit(LabelExpression *node) = 0;
    virtual void Visit(LoopExpression *node) = 0;
    virtual void Visit(MemberExpression *node) = 0;
    virtual void Visit(VariableExpression *node) = 0;
    virtual void Visit(UnaryExpression *node) = 0;
    virtual void Visit(ConvertExpression *node) = 0;
    virtual void Visit(ReturnExpression *node) = 0;
    virtual void Visit(FunctionExpression *node) = 0;
    virtual void Visit(ProgramExpression *node) = 0;
    virtual void Visit(CallExpression *node) = 0;
    virtual void Visit(NopExpression *node) = 0;
    virtual void Visit(AssignExpression *node) = 0;
    virtual void Visit(ArraySegmentExpression *node) = 0;
    virtual void Visit(NewRunnerExpression *node) = 0;
    virtual void Visit(CommitExpression *node) = 0;
    virtual void Visit(AbortExpression *node) = 0;
    virtual void Visit(InlineAssemblyExpression *node) = 0;
    virtual void Reset();
};

#endif // EXPRESSIONVISITOR_H

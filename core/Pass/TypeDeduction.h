#ifndef TYPECHECKPASS_H
#define TYPECHECKPASS_H

#include <core/CodeDom/ExpressionVisitor.h>
#include <core/Misc/Location.h>

class SymbolScope;
class Type;

class TypeDeduction : public ExpressionVisitor
{
private:
    SymbolScope *_currentScope;
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
    TypeDeduction();
    virtual ~TypeDeduction();
};

#endif // TYPECHECKPASS_H

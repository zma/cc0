#ifndef ILGENERATOR_H
#define ILGENERATOR_H
#include <vector>
#include <core/CodeDom/ExpressionVisitor.h>
#include <core/IL/IL.h>
#include <core/Misc/Location.h>
#include <stack>

class ILClass;
class ILFunction;
class ILProgram;
class Type;
class Symbol;
class SymbolScope;

class ILGenerator: public ExpressionVisitor
{
public:
    static const int64_t NEWR_METADATA_ADDR = 0x200001000;
private:
    int _tempId;
    int _labelId;
    std::stack<SymbolScope *> _staticChain;
    std::stack<Expression *> _loopContext;
    ILProgram *_program;
    ILClass *_class;
    ILFunction *_function;
    int64_t _rangeCursor;
public:
    ILGenerator();
    virtual ~ILGenerator();
public:
    ILProgram *GetILProgram();
public:
    virtual void Visit(BinaryExpression *node);
    virtual void Visit(BlockExpression *node);
    virtual void Visit(ConditionalExpression *node);
    virtual void Visit(ConstantExpression *node);
    virtual void Visit(GotoExpression *node);
    virtual void Visit(IndexExpression *node);
    virtual void Visit(LabelExpression *node);
    virtual void Visit(LoopExpression *node);
    virtual void Visit(MemberExpression *node);
    virtual void Visit(VariableExpression *node);
    virtual void Visit(UnaryExpression *node);
    virtual void Visit(ConvertExpression *node);
    virtual void Visit(ReturnExpression *node);
    virtual void Visit(FunctionExpression *node);
    virtual void Visit(ProgramExpression *node);
    virtual void Visit(CallExpression *node);
    virtual void Visit(NopExpression *node);
    virtual void Visit(AssignExpression *node);
    virtual void Visit(ArraySegmentExpression *node);
    virtual void Visit(NewRunnerExpression *node);
    virtual void Visit(CommitExpression *node);
    virtual void Visit(AbortExpression *node);
    virtual void Visit(InlineAssemblyExpression *node);
    virtual void Reset();
protected:
    IL::ILOperand GenerateTemp(Type* type);
    void GenerateLabel(IL::ILOperand labelOp);
    void GenerateIL(IL::ILOpcode opcode);
    void GenerateIL(IL::ILOpcode opcode, const IL::ILOperand &operand1);
    void GenerateIL(IL::ILOpcode opcode, const IL::ILOperand &operand1, const IL::ILOperand &operand2);
    void GenerateIL(IL::ILOpcode opcode, const IL::ILOperand &operand1, const IL::ILOperand &operand2, const IL::ILOperand &operand3);
    void GenerateIL(IL::ILOpcode opcode, const IL::ILOperand &operand1, const IL::ILOperand &operand2, const IL::ILOperand &operand3, const IL::ILOperand &operand4);
    void GenerateIL(IL::ILOpcode opcode, const std::vector<IL::ILOperand> &operands);
public:
    IL::ILOperandType GetOperandType(Type *type);
};

#endif // ILGENERATOR_H

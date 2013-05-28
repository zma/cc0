#ifndef I0CODEGENERATOR_H
#define I0CODEGENERATOR_H

#include <core/Pass/CodeGenerator.h>
#include "I0Instruction.h"
#include <stack>

class SymbolScope;
struct I0Instruction;

class I0CodeGenerator : public CodeGenerator
{
public:
    static const int64_t CURRENT_STACK_BASE = 0x100000100;
    static const int64_t CURRENT_STACK_SIZE = 0x100000108;    
    // static const int64_t NEWR_STACK_BASE = 0x100000400;
    // static const int64_t NEWR_STACK_SIZE = 0x100000408;
    static const int64_t PR_ADDR_REGISTER_START = 0x200000000;
    static const int64_t FRAME_POINTER = PR_ADDR_REGISTER_START;
    static const int64_t STACK_POINTER = PR_ADDR_REGISTER_START + 0x08;
    static const int64_t GENERAL_REG_1 = PR_ADDR_REGISTER_START + 0x10;
    static const int64_t GENERAL_REG_2 = PR_ADDR_REGISTER_START + 0x18;
    static const int64_t GENERAL_REG_3 = PR_ADDR_REGISTER_START + 0x20;
    static const int64_t GENERAL_REG_4 = PR_ADDR_REGISTER_START + 0x28;
    static const int64_t GENERAL_REG_5 = PR_ADDR_REGISTER_START + 0x30;
    static const int64_t GENERAL_REG_6 = PR_ADDR_REGISTER_START + 0x38;
    static const int64_t PR_TOP = 0x400000000;
private:
    static const int64_t RETURN_ADDRESS_OFFSET = 0;
    static const int64_t PARAMETER_OFFSET = 0x10;
    static const int64_t LOCAL_VARIABLE_OFFSET = -0x8;    

private:
    int64_t _currentText;
    int64_t _localOffset;
    std::stack<SymbolScope *> _staticChain;
public:
    virtual void VisitNop(IL* il);
    virtual void VisitAdd(IL* il);
    virtual void VisitAnd(IL* il);
    virtual void VisitBrEq(IL* il);
    virtual void VisitBrGt(IL* il);
    virtual void VisitBrGe(IL* il);
    virtual void VisitBrLt(IL* il);
    virtual void VisitBrLe(IL* il);
    virtual void VisitBrNe(IL* il);
    virtual void VisitBr(IL* il);
    virtual void VisitCall(IL* il);
    virtual void VisitCallInd(IL* il);
    virtual void VisitCallVirt(IL* il);
    virtual void VisitCast(IL* il);
    virtual void VisitCompEq(IL* il);
    virtual void VisitCompGt(IL* il);
    virtual void VisitCompGe(IL* il);
    virtual void VisitCompLt(IL* il);
    virtual void VisitCompLe(IL* il);
    virtual void VisitCompNe(IL* il);
    virtual void VisitConv(IL* il);
    virtual void VisitCpblk(IL* il);
    virtual void VisitCpobj(IL* il);
    virtual void VisitDiv(IL* il);
    virtual void VisitInitblk(IL* il);
    virtual void VisitInitobj(IL* il);
    virtual void VisitLdelem(IL* il);
    virtual void VisitLdelema(IL* il);
    virtual void VisitLdfld(IL* il);
    virtual void VisitLdflda(IL* il);
    virtual void VisitLdind(IL* il);
    virtual void VisitStelem(IL* il);
    virtual void VisitStfld(IL* il);
    virtual void VisitStind(IL* il);
    virtual void VisitStackalloc(IL* il);
    virtual void VisitMul(IL* il);
    virtual void VisitNeg(IL* il);
    virtual void VisitNewarr(IL* il);
    virtual void VisitNewobj(IL* il);
    virtual void VisitNot(IL* il);
    virtual void VisitOr(IL* il);
    virtual void VisitMod(IL* il);
    virtual void VisitRet(IL* il);
    virtual void VisitShl(IL* il);
    virtual void VisitShr(IL* il);
    virtual void VisitMov(IL* il);
    virtual void VisitXor(IL* il);
    virtual void VisitAsm(IL* il);
    virtual void VisitBrNz(IL* il);
    virtual void VisitBrZ(IL* il);
    virtual void VisitLabel(IL* il);
    virtual void VisitLdstr(IL* il);
    virtual void VisitLda(IL* il);
    virtual void VisitSub(IL* il);
public:
    I0CodeGenerator();
    virtual ~I0CodeGenerator();
private:
    void GenerateBinaryArithmatic(IL *il, I0Instruction::I0Opcode opcode);    
    void GenerateComparason(IL *il, I0Instruction::BJumpMode mode, bool brValue);
    void GenerateConditionalBr(IL *il, I0Instruction::BJumpMode mode);
    void GenerateCompareBranch(IL *il, I0Instruction::BJumpMode mode, bool brValue);
    
    void Emit(I0Instruction *inst);
    void Emit(std::string disa);
    void GenerateI0Operand(IL::ILOperand ilOperand, I0Instruction *inst, int operandIndex);
I0Instruction::ConvOperandAttribute GetConvOperandAttribute(IL::ILOperandType optype);
    int64_t GetOperandSize(IL::ILOperandType optype, int64_t align);
    void ResolveSymbols();
    void GenerateCrt0();
    void GenerateSpawn0();
protected:
    virtual void EnterProgram(ILProgram* program);
    virtual void LeaveProgram(ILProgram* program);
    virtual void EnterClass(ILClass* ilclass);
    virtual void LeaveClass(ILClass* ilclass);
    virtual void EnterFunction(ILFunction* ilfunc);
    virtual void LeaveFunction(ILFunction* ilfunc);
};

#endif // I0CODEGENERATOR_H

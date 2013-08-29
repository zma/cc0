#pragma once 
#include <core/IL/IL.h>
#include "ILClass.h"

class ILProgram;
class ILVisitor
{
    friend class IL;
public:
    ILVisitor();
    virtual ~ILVisitor();
public:
    virtual void Reset() = 0;
    void Visit(ILProgram *il);
protected:
    virtual void EnterProgram(ILProgram *program) = 0;
    virtual void LeaveProgram(ILProgram *program) = 0;
    virtual void EnterClass(ILClass *ilclass) = 0;
    virtual void LeaveClass(ILClass *ilclass) = 0;
    virtual void EnterFunction(ILFunction *ilfunc) = 0;
    virtual void LeaveFunction(ILFunction *ilfunc) = 0;
protected:
    virtual void VisitNop(IL *il) = 0;
    virtual void VisitAdd(IL *il) = 0;
    virtual void VisitAnd(IL *il) = 0;
    virtual void VisitBrEq(IL *il) = 0;
    virtual void VisitBrGt(IL *il) = 0;
    virtual void VisitBrGe(IL *il) = 0;
    virtual void VisitBrLt(IL *il) = 0;
    virtual void VisitBrLe(IL *il) = 0;
    virtual void VisitBrNe(IL *il) = 0;
    virtual void VisitBr(IL *il) = 0;
    virtual void VisitBrZ(IL *il) = 0;
    virtual void VisitBrNz(IL *il) = 0;
    virtual void VisitCall(IL *il) = 0;
    virtual void VisitCallInd(IL *il) = 0;
    virtual void VisitCallVirt(IL *il) = 0;
    virtual void VisitCast(IL *il) = 0;
    virtual void VisitCompEq(IL *il) = 0;
    virtual void VisitCompGt(IL *il) = 0;
    virtual void VisitCompGe(IL *il) = 0;
    virtual void VisitCompLt(IL *il) = 0;
    virtual void VisitCompLe(IL *il) = 0;
    virtual void VisitCompNe(IL *il) = 0;
    virtual void VisitConv(IL *il) = 0;
    virtual void VisitCpblk(IL *il) = 0;
    virtual void VisitCpobj(IL *il) = 0;
    virtual void VisitDiv(IL *il) = 0;
    virtual void VisitInitblk(IL *il) = 0;
    virtual void VisitInitobj(IL *il) = 0;
    virtual void VisitLdelem(IL *il) = 0;
    virtual void VisitLdelema(IL *il) = 0;
    virtual void VisitLdfld(IL *il) = 0;
    virtual void VisitLdflda(IL *il) = 0;
    virtual void VisitLdind(IL *il) = 0;
    virtual void VisitLdstr(IL *il) = 0;
    virtual void VisitStelem(IL *il) = 0;
    virtual void VisitStfld(IL *il) = 0;
    virtual void VisitStind(IL *il) = 0;
    virtual void VisitStackalloc(IL *il) = 0;
    virtual void VisitMul(IL *il) = 0;
    virtual void VisitNeg(IL *il) = 0;
    virtual void VisitNewarr(IL *il) = 0;
    virtual void VisitNewobj(IL *il) = 0;
    virtual void VisitNot(IL *il) = 0;
    virtual void VisitOr(IL *il) = 0;
    virtual void VisitMod(IL *il) = 0;
    virtual void VisitRet(IL *il) = 0;
    virtual void VisitShl(IL *il) = 0;
    virtual void VisitShr(IL *il) = 0;
    virtual void VisitMov(IL *il) = 0;
    virtual void VisitXor(IL *il) = 0;
    virtual void VisitAsm(IL *il) = 0;
    virtual void VisitLabel(IL* il) = 0;
    virtual void VisitSub(IL *il) = 0;
    virtual void VisitLda(IL *il) = 0;
};


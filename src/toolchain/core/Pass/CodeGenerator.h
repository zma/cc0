#pragma once 

#include <core/IL/ILVisitor.h>

class TargetProgram;
class TargetInstruction;
class ILProgram;

class CodeGenerator : public ILVisitor
{
public:
    CodeGenerator();
    virtual ~CodeGenerator();
public:
    TargetProgram *Generate(ILProgram *il);
    virtual void Reset();
protected:
    virtual void EnterProgram(ILProgram* program);
    virtual void EnterClass(ILClass* ilclass);
    virtual void EnterFunction(ILFunction* ilfunc);
    virtual void LeaveProgram(ILProgram* program);
    virtual void LeaveClass(ILClass* ilclass);
    virtual void LeaveFunction(ILFunction* ilfunc);
};


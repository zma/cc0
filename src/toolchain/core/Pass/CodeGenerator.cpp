#include "CodeGenerator.h"
#include <core/Core.h>


CodeGenerator::CodeGenerator()
{

}


CodeGenerator::~CodeGenerator()
{

}

TargetProgram *CodeGenerator::Generate(ILProgram* il)
{
    Reset();
    Visit(il);
}

void CodeGenerator::Reset()
{

}

void CodeGenerator::EnterProgram(ILProgram* program)
{

}

void CodeGenerator::LeaveProgram(ILProgram* program)
{

}

void CodeGenerator::EnterClass(ILClass* ilclass)
{

}

void CodeGenerator::LeaveClass(ILClass* ilclass)
{

}

void CodeGenerator::EnterFunction(ILFunction* ilfunc)
{

}

void CodeGenerator::LeaveFunction(ILFunction* ilfunc)
{

}

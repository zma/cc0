#ifndef ILOPTIMIZER_H
#define ILOPTIMIZER_H
// #include <core/IL/ILVisitor.h>
#include <core/IL/ILProgram.h>
#include <core/IL/ILFunction.h>
#include <core/IL/ILClass.h>
#include "core/Symbol/SymbolScope.h"

class SymbolScope;

class ILOptimizer
// : public ILVisitor
{
public:
    ILProgram* Optimize(ILProgram *il);
    int OperandUsedTwice(ILFunction *f, IL::ILOperand *opr);
    int DeleteNops(ILFunction *f);
    int OptimizeFunction(ILFunction *f);
    ILProgram* _il;
public:
    ILOptimizer();
    virtual ~ILOptimizer();
};

#endif // ILOPTIMIZER_H

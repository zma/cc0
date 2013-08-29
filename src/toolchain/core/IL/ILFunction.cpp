#include "ILFunction.h"
#include "ILClass.h"

ILFunction::ILFunction(ILClass *container, Symbol *functionSymbol)
{
    this->Container = container;
    this->FunctionSymbol = functionSymbol;
}

ILFunction::~ILFunction()
{

}


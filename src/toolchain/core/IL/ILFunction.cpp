#include "ILFunction.h"
#include "ILClass.h"

ILFunction::ILFunction():Scope(NULL), Container(NULL), FunctionSymbol(NULL)
{
}

ILFunction::ILFunction(ILClass *container, Symbol *functionSymbol):Scope(NULL)
{
    this->Container = container;
    this->FunctionSymbol = functionSymbol;
}

ILFunction::~ILFunction()
{

}


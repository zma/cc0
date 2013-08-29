#include "ILClass.h"
#include "ILProgram.h"

ILClass::ILClass(ILProgram* container, Symbol *classSymbol)
{
    this->Container = container;
    this->ClassSymbol = classSymbol;
}

ILClass::~ILClass()
{

}


#include "ILClass.h"
#include "ILProgram.h"

ILClass::ILClass():Scope(NULL), Container(NULL), ClassSymbol(NULL)
{
}

ILClass::ILClass(ILProgram* container, Symbol *classSymbol):Scope(NULL)
{
    this->Container = container;
    this->ClassSymbol = classSymbol;
}

ILClass::~ILClass()
{

}


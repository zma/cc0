#ifndef I0ASSEMBLYPARSER_H
#define I0ASSEMBLYPARSER_H

#include <core/Pass/SourceParser.h>
#include <core/Symbol/SymbolScope.h>

class SymbolScope;
class I0AssemblyParser : public SourceParser
{
public:
    virtual void Parse(std::string fileName);
    virtual void Parse(std::string fileName, SymbolScope *scope = SymbolScope::GetRootScope());
public:
    I0AssemblyParser();
    virtual ~I0AssemblyParser();
};

#endif // I0ASSEMBLYPARSER_H

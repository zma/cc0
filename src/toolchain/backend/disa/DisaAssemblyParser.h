#ifndef DISAASSEMBLYPARSER_H
#define DISAASSEMBLYPARSER_H

#include <core/Pass/SourceParser.h>
#include <core/Symbol/SymbolScope.h>

class SymbolScope;
class DisaAssemblyParser : public SourceParser
{
public:
    virtual void Parse(std::string fileName);
    virtual void Parse(std::string fileName, SymbolScope *scope = SymbolScope::GetRootScope());
public:
    DisaAssemblyParser();
    virtual ~DisaAssemblyParser();
};

#endif // DISAASSEMBLYPARSER_H

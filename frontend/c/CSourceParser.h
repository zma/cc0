#ifndef CSOURCEPARSER_H
#define CSOURCEPARSER_H

#include <core/Pass/SourceParser.h>


class CSourceParser : public SourceParser
{

public:
    virtual void Parse(std::string fileName);
    CSourceParser();
    virtual ~CSourceParser();
};

#endif // CSOURCEPARSER_H

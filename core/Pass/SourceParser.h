#ifndef PARSER_H
#define PARSER_H
#include <iostream>

class Expression;
class SourceParser
{
public:
    virtual void Parse(std::string fileName) = 0;
public:
    SourceParser();
    virtual ~SourceParser();
};

#endif // PARSER_H

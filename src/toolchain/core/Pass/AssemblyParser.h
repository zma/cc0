#pragma once 
#include <stdint.h>
#include <string>

class TargetProgram;
class AssemblyParser
{
// protected:
//     int64_t _text, _data, _rodata, _bss;
public:
    virtual void Parse(std::string fileName) = 0;
public:
    AssemblyParser();
//     AssemblyParser(int64_t text, int64_t data, int64_t rodata, int64_t bss);
    virtual ~AssemblyParser();
};
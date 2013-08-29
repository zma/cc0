#pragma once 

#include <stdint.h>

#include <string>
#include <vector>
#include <map>

class Symbol;
class TargetInstruction;

class TargetProgram
{
// private:
//     int64_t _text, _data, _rodata, _bss;
public:
//     TargetProgram(int64_t text, int64_t data, int64_t rodata, int64_t bss);
    TargetProgram();
    virtual ~TargetProgram();
public:
    std::vector<TargetInstruction*> Code;
    std::basic_string<int8_t> Data;
    std::basic_string<int8_t> ReadonlyData;
    int64_t SizeOfBss;
};


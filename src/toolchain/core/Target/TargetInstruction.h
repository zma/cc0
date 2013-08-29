#pragma once 

#include <stdint.h>
#include <map>
#include <string>
#include <core/Misc/TagContainer.h>

class TargetInstruction: public TagContainer
{
public:
    TargetInstruction();
    virtual ~TargetInstruction();
public:
    virtual void Encode(char *buffer) = 0;
    virtual int64_t GetLength() = 0;    
    virtual std::string ToString() = 0;
};
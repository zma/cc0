#pragma once 

#include <stdint.h>

class Configuration
{
public:
    Configuration();
    ~Configuration();
public:
    int64_t TextStart;
    int64_t DataStart;
    int64_t RDataStart;
    int64_t BssStart;
};

#ifndef SECTIONINFO_H
#define SECTIONINFO_H
#include <stdint.h>
#include <string>


struct SectionInfo
{
    std::string Name;
    void *RawData;
    int64_t RawDataSize;
    int64_t VirtualBase;
    int64_t VirtualSize;
};

#endif // SECTIONINFO_H

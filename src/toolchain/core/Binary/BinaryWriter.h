#ifndef BINARYWRITER_H
#define BINARYWRITER_H

#include <string>
#include <stdint.h>
#include <vector>
#include <core/Binary/SectionInfo.h>

class Instruction;
class BinaryWriter
{
public:
    BinaryWriter();
    virtual ~BinaryWriter();
public:
    virtual void WriteBinaryFile(std::string filename, std::vector<SectionInfo> *sections, int64_t entryPoint) = 0;                
};

#endif // BINARYWRITER_H

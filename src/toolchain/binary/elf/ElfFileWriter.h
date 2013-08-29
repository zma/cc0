#ifndef ELFFILEWRITER_H
#define ELFFILEWRITER_H
#include <core/Binary/BinaryWriter.h>

#define EMACHINE_DISA (231);
class ElfFileWriter: public BinaryWriter
{
public:
    ElfFileWriter();
    virtual ~ElfFileWriter();
public:
    virtual void WriteBinaryFile(std::string filename, std::vector<SectionInfo> *sections, int64_t entryPoint);             

};

#endif // ELFFILEWRITER_H

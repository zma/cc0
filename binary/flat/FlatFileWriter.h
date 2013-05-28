#ifndef FLATFILEWRITER_H
#define FLATFILEWRITER_H

#include <core/Binary/BinaryWriter.h>


class FlatFileWriter : public BinaryWriter
{

public:
    FlatFileWriter();
    virtual ~FlatFileWriter();
public:
    virtual void WriteBinaryFile(std::string filename, std::vector< SectionInfo, std::allocator< SectionInfo > >* sections, int64_t entryPoint);

};

#endif // FLATFILEWRITER_H

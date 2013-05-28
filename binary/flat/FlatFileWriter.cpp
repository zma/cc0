#include "FlatFileWriter.h"
#include <stdio.h>
#include <assert.h>

FlatFileWriter::FlatFileWriter()
{

}

FlatFileWriter::~FlatFileWriter()
{

}

void FlatFileWriter::WriteBinaryFile(std::string filename, std::vector<SectionInfo, std::allocator< SectionInfo > >* sections, int64_t entryPoint)
{
    long pos = 0;

    FILE *fp = fopen(filename.c_str(), "wb");

    for (std::vector<SectionInfo>::iterator it = sections->begin(); it != sections->end(); ++it)
    {
        long newPos = it->VirtualBase - entryPoint;
        assert(newPos >= 0);

        // Append the hole with zeros, some systems may not support sparse files
        for (;pos < newPos; pos++)
        {
            char dummy = 0;
            fwrite(&dummy, 1, 1, fp);
        }

        fseek(fp, newPos, SEEK_SET);
        fwrite(it->RawData, it->RawDataSize, 1, fp);
        
         // Paddings if virtual size < raw size
         for (int i = 0; i < it->VirtualSize - it->RawDataSize; ++i)
         {
             char dummy = 0;
             fwrite(&dummy, 1, 1, fp);
         }

    }

    fclose(fp);
}

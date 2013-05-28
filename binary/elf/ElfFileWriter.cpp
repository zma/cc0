#include "ElfFileWriter.h"
#include <stdio.h>
#include <elf.h>
#include <string.h>

ElfFileWriter::ElfFileWriter()
{

}

ElfFileWriter::~ElfFileWriter()
{

}

void ElfFileWriter::WriteBinaryFile(std::string filename, std::vector< SectionInfo > *sections, int64_t entryPoint)
{
    FILE *fp = fopen(filename.c_str(), "wb");
    
    fseek(fp, SEEK_SET, 0);
    
    Elf64_Ehdr header;
    memset(&header, 0, sizeof(header));
    header.e_ident[EI_MAG0] = 0x7F;
    header.e_ident[EI_MAG1] = 'E';
    header.e_ident[EI_MAG2] = 'L';
    header.e_ident[EI_MAG3] = 'F';
    header.e_ident[EI_CLASS] = ELFCLASS64;
    header.e_ident[EI_VERSION] = EV_CURRENT;
    header.e_ident[EI_OSABI] = ELFOSABI_STANDALONE;
    header.e_ident[EI_ABIVERSION] = 1;
    header.e_machine = EMACHINE_DISA;
    header.e_type = ET_EXEC;
    header.e_version = EV_CURRENT;
    header.e_entry = entryPoint;
    header.e_phentsize = 0;
    header.e_ehsize = sizeof(header);
    header.e_shoff = sizeof(header);
    header.e_shentsize =sizeof(Elf64_Shdr);
    header.e_shnum = sections->size() + 1;
    header.e_shstrndx = sections->size();

    long pos = 0;
    
    fseek(fp, pos, SEEK_SET);
    fwrite(&header, sizeof(header), 1, fp);
    
    char snStrTable[4096];
    memset (snStrTable, 0, 4096);
    
    unsigned int snStrTableIndex = 1;
    
    pos += sizeof(header);
    
    int nSection = sections->size() + 1;
    long ptRawData = pos + sizeof(Elf64_Shdr) * nSection;
    
    for (std::vector<SectionInfo>::iterator it = sections->begin(); it != sections->end(); ++it)
    {
        Elf64_Shdr shd;
        shd.sh_name = snStrTableIndex;
        
        snStrTableIndex += it->Name.copy(&snStrTable[snStrTableIndex], 4096);
        snStrTable[snStrTableIndex++] = '\0';
        
        if (it->Name == ".bss")
        {
            shd.sh_type = SHT_NOBITS;
        }
        else
        {
            shd.sh_type = SHT_PROGBITS;
        }
        
        shd.sh_addr = it->VirtualBase;
        shd.sh_addralign = 4096;
        shd.sh_offset = ptRawData;
        shd.sh_size = it->RawDataSize;
        ptRawData += it->RawDataSize;
        
        fseek(fp, pos, SEEK_SET);
        fwrite(&shd, sizeof(shd), 1, fp);
        pos += sizeof(shd);
    }
    
    do
    {
        Elf64_Shdr shd;
        shd.sh_name = snStrTableIndex;
        std::string name = ".shstrtab";
        snStrTableIndex += name.copy(&snStrTable[snStrTableIndex], 4096);
        snStrTable[snStrTableIndex++] = '\0';

        shd.sh_type = SHT_STRTAB;
        
        shd.sh_addr = 0;
        shd.sh_addralign = 0;
        shd.sh_offset = ptRawData;
        shd.sh_size = snStrTableIndex;
        ptRawData += snStrTableIndex;
        
        fseek(fp, pos, SEEK_SET);
        fwrite(&shd, sizeof(shd), 1, fp);
        
        pos += sizeof(shd);
    } while (false);

    for (std::vector<SectionInfo>::iterator it = sections->begin(); it != sections->end(); ++it)
    {        
        fseek(fp, pos, SEEK_SET);
        fwrite(it->RawData, it->RawDataSize, 1, fp);
        pos += it->RawDataSize;
    }
    
    do
    {
        fseek(fp, pos, SEEK_SET);
        fwrite(snStrTable, 4096, 1, fp);
        pos += 4096;
    } while (false);
    
    fclose(fp);
}


#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <assert.h>
#include <iostream>
#include <fstream>

#include <map>

#include <core/Core.h>
#include <frontend/c/CSourceParser.h>
// #include <backend/disa/DisaCodeGenerator.h>
// #include <backend/disa/DisaAssemblyParser.h>
#include <backend/i0/I0CodeGenerator.h>
#include <binary/elf/ElfFileWriter.h>
#include <binary/flat/FlatFileWriter.h>
#include <core/Symbol/SymbolAddressAllocator.h>
#include <core/Pass/ConstantPropagation.h>
#include <core/Pass/TypeDeduction.h>

#include "../../../external/mem.h"
#include "../../../external/sys_config.h"

void DumpScopeTypes(SymbolScope *scope, std::ofstream &dump, std::string prefix)
{
    char buffer[100];
    dump << prefix << "SCOPE " << scope << " type " << scope->GetScopeKind() << " {" << std::endl;

    std::string cur_prefix = prefix + "  ";

    for(std::map<std::string, Symbol *>::iterator it = scope->GetSymbolTable()->begin(); it != scope->GetSymbolTable()->end(); ++it)
    {
        Symbol *symbol = it->second;
        // if (typeid(*(symbol->DeclType)) == typeid(FunctionType) || scope->GetScopeKind() == SymbolScope::Global)
        {
        	dump << cur_prefix <<
        			symbol->Name << "\t" <<
        			symbol->DeclType->ToString() << "\t" <<
        			std::endl;
        }
    }

    for(std::vector<SymbolScope *>::iterator it = scope->GetChildScopes()->begin(); it != scope->GetChildScopes()->end(); ++it)
    {
        SymbolScope *cs = *it;
        DumpScopeTypes(cs, dump, cur_prefix);
    }

    dump << prefix << "}" << std::endl;
}


void DumpScope(SymbolScope *scope, std::ofstream &dump)
{
    char buffer[100];

    for(std::map<std::string, Symbol *>::iterator it = scope->GetSymbolTable()->begin(); it != scope->GetSymbolTable()->end(); ++it)
    {
        Symbol *symbol = it->second;
        if (typeid(*(symbol->DeclType)) == typeid(FunctionType) || scope->GetScopeKind() == SymbolScope::Global)
        {
            sprintf(buffer, "%0llX\t%s", (long long)symbol->Address, symbol->Name.c_str());
            dump << buffer << std::endl;
        }
    }

    for(std::vector<SymbolScope *>::iterator it = scope->GetChildScopes()->begin(); it != scope->GetChildScopes()->end(); ++it)
    {
        SymbolScope *cs = *it;
        DumpScope(cs, dump);
    }
}

void print_usage(char *cmd)
{
    printf(
"cc0 - A c0 compiler which generates i0 code.\n"
"\n"
"Usage: \n"
"    cc0 [-g|--debug] [-c] [-Dopts] [-Idirs] [-h|--help]\n"
"        infile -o outfile\n"
"\n"
"\n"
"Options:\n"
"--debug, -g\n"
"        Output debugging information.\n"
"-c\n"
"        Compile only.\n"
"-Dopts\n"
"        Pass -Dopts to the preprocessor.\n"
"-Ddirs\n"
"        Pass -Idirs to the preprocessor.\n"
"\n"
    );

    return;
}

int main(int argc, char **argv)
{
    bool codeTypeDefined = false;
    std::string cpp_args = "";

    CompilationContext *context = CompilationContext::GetInstance();

    // context->TextStart =  0x400000000;
    // context->DataStart =  0x400004000;
    // context->RDataStart = 0x400008000;
    // Use macros from the sys_config.h
    context->TextStart =  I0_CODE_BEGIN;
    context->DataStart =  I0_CODE_BEGIN + 0x4000;
    context->RDataStart = I0_CODE_BEGIN + 0x8000;

    //NOTE: Currently, all global variables are put in the bss section and are NOT initialized with zeros, the data/rdata is not used.
    // context->BssStart =   0x440000000;
    context->BssStart =   AMR_OFFSET_BEGIN;

    // NOTE: default targe code type
    // Only CODE_TYPE_I0 is supported
    CompilationContext::GetInstance()->CodeType = CODE_TYPE_I0;
    codeTypeDefined = true;

    for(int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0)
        {
			if (argv[i + 1] != NULL && *argv[i + 1] != '-') {
            	CompilationContext::GetInstance()->OutputFile = argv[++i];
			} else {
				CompilationContext::GetInstance()->OutputFile = "a.bin";
			}
        }
        else if( (strcmp(argv[i], "--debug") == 0) || (strcmp(argv[i], "-g") == 0) )
        {
            CompilationContext::GetInstance()->Debug = true;
        }
        else if( (strcmp(argv[i], "-c") == 0))
        {
            CompilationContext::GetInstance()->CompileOnly = true;
        }
        /*
        else if (strcmp(argv[i], "--i0") == 0)
        {
            if (codeTypeDefined) {
                printf("--i0 and --disa can not be used at the same time.\n"
                        "Specify one code type only.\n");
                return -1;
            }
            CompilationContext::GetInstance()->CodeType = CODE_TYPE_I0;
            codeTypeDefined = true;
        }
        else if (strcmp(argv[i], "--disa") == 0)
        {
            if (codeTypeDefined) {
                printf("--i0 and --disa can not be used at the same time.\n"
                        "Specify one code type only.\n");
                return -11;
            }

            CompilationContext::GetInstance()->CodeType = CODE_TYPE_DISA;
            codeTypeDefined = true;
        }
        */
        else if ( strcmp(argv[i], "-I") == 0 && i + 1 < argc ) {
            cpp_args += " " + std::string(argv[i]) + std::string(argv[i+1]);
            ++i;
        }
        else if ( strlen(argv[i]) > 2 && strncmp(argv[i], "-I", 2) == 0 ) {
            cpp_args += " " + std::string(argv[i]);
        }
        else if ( strlen(argv[i]) > 2 && strncmp(argv[i], "-D", 2) == 0 ) {
            cpp_args += " " + std::string(argv[i]);
        }
        else if ( (strcmp(argv[i], "--help") == 0) || strcmp(argv[i], "-h") == 0 )
        {
            print_usage(argv[0]);
            return 0;
        }
        else if ( strncmp(argv[i], "-", 1) != 0)
        {
            CompilationContext::GetInstance()->InputFiles.push_back(argv[i]);
        }
        else {
            std::cout << "Unsupported option: " + std::string(argv[i]) << std::endl;
            return -1;
        }
    }

    ILProgram *il = NULL;

    std::vector<std::string> &inputFiles = CompilationContext::GetInstance()->InputFiles;

    if(inputFiles.size() == 0)
    {
        print_usage(argv[0]);
        return 0;
    }
    else if(inputFiles.size() != 1)
    {
        //FIXME: Multiple input files
        assert("Not implemented: multiple input files.");
    }

    for(std::vector<std::string>::iterator it = inputFiles.begin(); it != inputFiles.end(); ++it)
    {
        std::string inputFile = CompilationContext::GetInstance()->InputFiles.front();
        std::string fileExt = inputFile.substr(inputFile.find_last_of(".") + 1);
        if(fileExt == "c" || fileExt == "c0")
        {

            char tmpFileName[255];
            sprintf(tmpFileName, "%s.tmp", inputFile.c_str());

            // tmpnam(tmpFileName);
            if(CompilationContext::GetInstance()->Debug)
                printf("temp file is: %s\n", tmpFileName);

            context->CurrentFileName = inputFile;

            std::string cmdline = "cpp --sysroot ." + cpp_args + " " + inputFile + " -o " + tmpFileName;
            if(CompilationContext::GetInstance()->Debug)
                std::cout << "Preprocessing with cmd \"" + cmdline << "\"" << std::endl;

            if(system(cmdline.c_str()) != 0)
            {
               return -1;
            }

            if(CompilationContext::GetInstance()->Debug)
            {
                printf("--------------------------------------\n");
                printf("parsing...\n");
            }

            CSourceParser *frontend = new CSourceParser();
            frontend->Parse(tmpFileName);

            // Note: leave tmpFile for user to check
            // remove(tmpFileName);

            if(CompilationContext::GetInstance()->Debug)
            {
                printf("--------------------------------------\n");
                printf("ConstantPropagation...\n");
            }

            ConstantPropagation *constantPropagation = new ConstantPropagation();
            context->CodeDom->Accept(constantPropagation);

            if(CompilationContext::GetInstance()->Debug)
            {
                printf("--------------------------------------\n");
                printf("ConstantPropagation...\n");
            }

            TypeDeduction *typeDeduction = new TypeDeduction();
            context->CodeDom->Accept(typeDeduction);

            if(CompilationContext::GetInstance()->Debug)
            {
                printf("--------------------------------------\n");
                printf("codeDom Dump:\n");
                ExpressionTreeDumper *codeDomDump = new ExpressionTreeDumper();

                context->CodeDom->Accept(codeDomDump);
            }

            ILGenerator *ilgen = new ILGenerator();
            context->CodeDom->Accept(ilgen);

            il = ilgen->GetILProgram();
        }
    }

    if(il == NULL)
    {
        return -1;
    }


    if((CompilationContext::GetInstance()->Debug || CompilationContext::GetInstance()->CompileOnly) && il != NULL)
    {
        std::string baseFileName = CompilationContext::GetInstance()->OutputFile;
        int pos = baseFileName.find_last_of(".");
        if(pos != -1)
        {
            baseFileName = baseFileName.substr(0, pos) + ".il";
        }

        std::ofstream ildump(baseFileName.c_str());
        for(std::vector<ILClass *>::iterator cit = il->Claases.begin(); cit != il->Claases.end(); ++cit)
        {
            ILClass *c = *cit;

            ildump << "class " <<  c->ClassSymbol->Name << std::endl << "{" << std::endl;

            for(std::vector<ILFunction *>::iterator fit = c->Functions.begin(); fit != c->Functions.end(); ++fit)
            {
                ILFunction *f = *fit;
                ildump << "    function " <<  f->FunctionSymbol->Name << std::endl << "    {" << std::endl;
                for(std::vector<IL>::iterator iit = f->Body.begin(); iit != f->Body.end(); ++iit)
                {
                    IL &il = *iit;
                    if(il.Opcode == IL::Label)
                    {
                        ildump << "        " << il.ToString() << std::endl;
                    }
                    else
                    {
                        ildump << "            " << il.ToString() << std::endl;
                    }
                }
                ildump << "    }" << std::endl;
            }
            ildump << "}" << std::endl;
        }

        ildump.close();
    }

    context->IL = il;

    // TODO: Optimize the IL
    ILOptimizer *ilopt = NULL;

    ilopt = new ILOptimizer();

    context->IL = ilopt->Optimize(il);

    // print optimized IL
    il = context->IL;
    if((CompilationContext::GetInstance()->Debug || CompilationContext::GetInstance()->CompileOnly) && il != NULL)
    {
        // printf("--------------------------------------\n");
        // printf("Optimized IL:\n");
        std::string baseFileName = CompilationContext::GetInstance()->OutputFile;
        int pos = baseFileName.find_last_of(".");
        if(pos != -1)
        {
            baseFileName = baseFileName.substr(0, pos) + ".opt.il";
        }

        std::ofstream ildump(baseFileName.c_str());
        for(std::vector<ILClass *>::iterator cit = il->Claases.begin(); cit != il->Claases.end(); ++cit)
        {
            ILClass *c = *cit;

            ildump << "class " <<  c->ClassSymbol->Name << std::endl << "{" << std::endl;

            for(std::vector<ILFunction *>::iterator fit = c->Functions.begin(); fit != c->Functions.end(); ++fit)
            {
                ILFunction *f = *fit;
                ildump << "    function " <<  f->FunctionSymbol->Name << std::endl << "    {" << std::endl;
                for(std::vector<IL>::iterator iit = f->Body.begin(); iit != f->Body.end(); ++iit)
                {
                    IL &il = *iit;
                    if(il.Opcode == IL::Label)
                    {
                        ildump << "        " << il.ToString() << std::endl;
                    }
                    else
                    {
                        ildump << "            " << il.ToString() << std::endl;
                    }
                }
                ildump << "    }" << std::endl;
            }
            ildump << "}" << std::endl;
        }

        ildump.close();
    }

    std::string baseFileName = CompilationContext::GetInstance()->OutputFile;
    int pos = baseFileName.find_last_of(".");
    if(pos != -1)
    {
        baseFileName = baseFileName.substr(0, pos);
    }
    std::string mapFileName = baseFileName + ".var";

    std::ofstream mapdump(mapFileName.c_str());
    DumpScopeTypes(SymbolScope::GetRootScope(), mapdump, "");
    mapdump.close();

    // if it is for -c, return now
    if(CompilationContext::GetInstance()->CompileOnly) {
        return 0;
    }

    CodeGenerator *codegen = NULL;
    //Generate assembly code from IL
    /* if (CompilationContext::GetInstance()->CodeType == CODE_TYPE_DISA) {
        //Generate DISA assembly code from IL
        codegen = new DisaCodeGenerator();
        } else */
    if (CompilationContext::GetInstance()->CodeType == CODE_TYPE_I0) {
        codegen = new I0CodeGenerator();
    } else {
        printf("Error: unsupported CodeType.\n");
        return -1;
    }

    codegen->Generate(context->IL);


    for(std::vector<std::string>::iterator it = inputFiles.begin(); it != inputFiles.end(); ++it)
    {
        std::string inputFile = context->InputFiles.front();
        std::string fileExt = inputFile.substr(inputFile.find_last_of(".") + 1);
        if(fileExt == "s")
        {
            SourceParser *parser = NULL;
            /* if (CompilationContext::GetInstance()->CodeType == CODE_TYPE_DISA) {
                parser = new DisaAssemblyParser();
            } else
            */
            {
                // TODO: support i0 assembly
                printf(".s file is not supported for i0.\n");
                return -1;
            }
            parser->Parse(inputFile);
        } else if(fileExt == "c") {
            printf("WARNING: It is recommended to use .c0 instead of .c as the source file extension for c0 programs.\n");
        }
    }

    if((CompilationContext::GetInstance()->Debug || CompilationContext::GetInstance()->CompileOnly))
    {
        std::string baseFileName = CompilationContext::GetInstance()->OutputFile;
        std::string dumpFileName, mapFileName;
        int pos = baseFileName.find_last_of(".");
        if(pos != -1)
        {
            baseFileName = baseFileName.substr(0, pos);
        }

        dumpFileName = baseFileName + ".objdump";
        mapFileName = baseFileName + ".map";

        std::ofstream objdump(dumpFileName.c_str());

        int64_t currentText = context->TextStart;
        for(std::vector<TargetInstruction *>::iterator iit = context->Target->Code.begin(); iit != context->Target->Code.end(); ++iit)
        {
            TargetInstruction *inst = *iit;
            char buffer[32];
            sprintf(buffer, "%0llX> \t", (long long)currentText);
            objdump << buffer << inst->ToString().c_str() << std::endl;
            currentText += inst->GetLength();
        }
        objdump.close();


        std::ofstream mapdump(mapFileName.c_str());
        DumpScope(SymbolScope::GetRootScope(), mapdump);
        mapdump.close();
    }

    if(CompilationContext::GetInstance()->Debug)
        printf("Maximum stack frame size: 0x%llX\n", (long long )(context->MaxStackFrame));

    // TODO: Optimize the assembly code
    TargetOptimizer *targetOpt = NULL;


    char *textBuf = new char[0x100000];
    int64_t textSize = 0;
    for(std::vector<TargetInstruction *>::iterator it = context->Target->Code.begin(); it != context->Target->Code.end(); ++it)
    {
        TargetInstruction *inst = *it;
        inst->Encode(&textBuf[textSize]);
        textSize += inst->GetLength();
    }

    // Write the binary into file
    std::string outputFile = CompilationContext::GetInstance()->OutputFile;
    BinaryWriter *binwt = new FlatFileWriter();

    std::vector<SectionInfo> sections;
    SectionInfo textSection;
    textSection.Name = ".text";
    textSection.RawData = textBuf;
    textSection.RawDataSize = textSize;
    textSection.VirtualBase = context->TextStart;
    textSection.VirtualSize = textSize;
    sections.push_back(textSection);

    binwt->WriteBinaryFile(context->OutputFile, &sections, context->TextStart);

    return 0;

}

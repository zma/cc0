#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <list>
#include <iterator>
#include <unistd.h>

#include <boost/archive/xml_oarchive.hpp>

#include "core/Core.h"
#include "frontend/c/CSourceParser.h"
#include "backend/i0/I0CodeGenerator.h"
#include "binary/flat/FlatFileWriter.h"
#include "core/Pass/ConstantPropagation.h"
#include "core/Pass/TypeDeduction.h"
#include "core/Misc/FilePath.h"
#include "core/Serialization/ExportDeriveExpressions.h"
#include "core/Serialization/ExportDeriveTypes.h"
#include "core/Serialization/ObjFormat.h"

#include "../../../external/mem.h"
#include "../../../external/sys_config.h"

void DumpScopeTypes(SymbolScope *scope, std::ofstream &dump, std::string prefix) {
    char buffer[100];
    dump << prefix << "SCOPE " << scope << " type " << scope->GetScopeKind() << " {" << std::endl;

    std::string cur_prefix = prefix + "  ";

    for (std::map<std::string, Symbol *>::iterator it = scope->GetSymbolTable()->begin(); it != scope->GetSymbolTable()->end(); ++it) {
        Symbol *symbol = it->second;
        // if (typeid(*(symbol->DeclType)) == typeid(FunctionType) || scope->GetScopeKind() == SymbolScope::Global)
        {
            dump << cur_prefix << symbol->Name << "\t" << symbol->DeclType->ToString() << "\t" << std::endl;
        }
    }

    for (std::vector<SymbolScope *>::iterator it = scope->GetChildScopes()->begin(); it != scope->GetChildScopes()->end(); ++it) {
        SymbolScope *cs = *it;
        DumpScopeTypes(cs, dump, cur_prefix);
    }

    dump << prefix << "}" << std::endl;
}

void DumpScope(SymbolScope *scope, std::ofstream &dump) {
    char buffer[100];

    for (std::map<std::string, Symbol *>::iterator it = scope->GetSymbolTable()->begin(); it != scope->GetSymbolTable()->end(); ++it) {
        Symbol *symbol = it->second;
        if (typeid(*(symbol->DeclType)) == typeid(FunctionType) || scope->GetScopeKind() == SymbolScope::Global) {
            sprintf(buffer, "%0llX\t%s", (long long) symbol->Address, symbol->Name.c_str());
            dump << buffer << std::endl;
        }
    }

    for (std::vector<SymbolScope *>::iterator it = scope->GetChildScopes()->begin(); it != scope->GetChildScopes()->end(); ++it) {
        SymbolScope *cs = *it;
        DumpScope(cs, dump);
    }
}

void print_usage(char *cmd) {
    printf("cc0 - A c0 compiler which generates i0 code.\n"
            "\n"
            "Usage: \n"
            "    cc0 [-g|--debug] [-c] [-Dopts] [-Idirs] [-h|--help]\n"
            "        infile[s] -o [outfile]\n"
            "\n"
            "\n"
            "Options:\n"
            "--debug, -g\n"
            "        Output debugging information.\n"
            "-c\n"
            "        Compile only.\n"
            "-Dmacro...\n"
            "        Pass -Dmacro... to the preprocessor.\n"
            "-Idir...\n"
            "        Pass -Idir... to the preprocessor.\n"
            "\n");

    return;
}

int main(int argc, char **argv) {
    std::string cpp_args = "";
    std::vector<std::string> input_files;
    std::string output_file;
    bool compile_only = false;
    bool debug = false;

    //parse command line
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            if (argv[i + 1] != NULL && *argv[i + 1] != '-') {
                output_file = argv[++i];
            } else {
                std::cerr << "syntax error!\n";
                return -1;
            }
        } else if (strcmp(argv[i], "--debug") == 0 || strcmp(argv[i], "-g") == 0) {
            debug = true;
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
        else if (strcmp(argv[i], "-I") == 0 && i + 1 < argc) {
            cpp_args += " " + std::string(argv[i]) + std::string(argv[i + 1]);
            ++i;
        } else if (strcmp(argv[i], "-c") == 0) {
            compile_only = true;
        } else if (strncmp(argv[i], "-I", 2) == 0) {
            cpp_args += " " + std::string(argv[i]);
        } else if (strncmp(argv[i], "-D", 2) == 0) {
            cpp_args += " " + std::string(argv[i]);
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strncmp(argv[i], "-", 1) != 0) {
            input_files.push_back(argv[i]);
        } else {
            std::cerr << "Unsupported option: " + std::string(argv[i]) << std::endl;
            return -1;
        }
    }

    if (input_files.size() == 0) {
        std::cerr << "No input files applied!\n";
        return -1;
    }

    //if cc0 is used as both compiler and linker, invoke cc0 and ld0 recursively
    if (!compile_only) {
        std::vector<std::string> objfiles;
        for (std::vector<std::string>::iterator i = input_files.begin(), iE = input_files.end(); i != iE; ++i) {
            if (GetFileExtension(*i) == ".c" || GetFileExtension(*i) == ".c0") {

                //compile
                std::string obj = ReplaceFileNameExtension(*i, ".o");
                objfiles.push_back(obj);
                std::stringstream cc0_cmdline;
                cc0_cmdline << "cc0 -c " << *i << " -o " << obj << cpp_args;
                if (debug) {
                    cc0_cmdline << " -g";
                }
                std::cout << "invoking \"" << cc0_cmdline.str() << "\"\n";
                std::cout.flush();
                if (system(cc0_cmdline.str().c_str())) {
                    std::cerr << "compiling " << *i << "failed!\n";
                    return 1;
                }
            } else if (GetFileExtension(*i) == ".o") {

                //no need to compile
                objfiles.push_back(*i);
            } else {
                std::cerr << "input file " << *i << " ignored!\n";
            }
        }
        if (output_file.size() == 0) {
            output_file = "a.bin";
        }
        std::stringstream ld0_cmdline;
        ld0_cmdline << "ld0 -lcrt -o " << output_file;
        for (std::vector<std::string>::iterator i = objfiles.begin(), iE = objfiles.end(); i != iE; ++i) {
            ld0_cmdline << " " << *i;
        }
        if (debug) {
            ld0_cmdline << " -g";
        }
        std::cout << "invoking \"" << ld0_cmdline.str() << "\"\n";
        std::cout.flush();

        //link
        if (system(ld0_cmdline.str().c_str())) {
            std::cerr << "Link failed\n";
            return 1;
        }
        return 0;
    }

    if (input_files.size() > 1) {
        std::cerr << "Multiple input files applied during compiling!\n";
        return -1;
    }
    if (output_file.size() == 0) {
        output_file = ReplaceFileNameExtension(input_files.front(), ".o");
    }

    CompilationContext *context = CompilationContext::GetInstance();

    context->InputFiles = input_files;
    context->OutputFile = output_file;
    context->Debug = debug;
    context->CompileOnly = compile_only;

    // context->TextStart =  0x400000000;
    // context->DataStart =  0x400004000;
    // context->RDataStart = 0x400008000;
    // Use macros from the sys_config.h
    context->TextStart = I0_CODE_BEGIN;
    context->DataStart = I0_CODE_BEGIN + 0x4000;
    context->RDataStart = I0_CODE_BEGIN + 0x8000;

    //NOTE: Currently, all global variables are put in the bss section and are NOT initialized with zeros, the data/rdata is not used.
    // context->BssStart =   0x440000000;
    context->BssStart = AMR_OFFSET_BEGIN;

    // NOTE: default targe code type
    // Only CODE_TYPE_I0 is supported
    CompilationContext::GetInstance()->CodeType = CODE_TYPE_I0;

    //try import include paths from $CC0_INC
    {
        char* cc0_inc = getenv("CC0_INC");
        if (cc0_inc) {
            std::list<std::string> cc0_inc_paths;
            SplitAndStorePath(cc0_inc, back_inserter(cc0_inc_paths));
            for (std::list<std::string>::iterator i = cc0_inc_paths.begin(), iE = cc0_inc_paths.end(); i != iE; ++i) {
                cpp_args += " -I" + *i;
            }
        }
    }

    ILProgram *il = NULL;

    std::vector<std::string> &inputFiles = CompilationContext::GetInstance()->InputFiles;

    for (std::vector<std::string>::iterator it = inputFiles.begin(); it != inputFiles.end(); ++it) {
        std::string& inputFile = CompilationContext::GetInstance()->InputFiles.front();
        std::string fileExt = GetFileExtension(inputFile);
        if (fileExt == ".c" || fileExt == ".c0") {
            if (fileExt == ".c") {
                printf("WARNING: It is recommended to use .c0 instead of .c as the source file extension for c0 programs.\n");
            }
            std::string tmpFileName = GetFileName(inputFile) + ".tmp";

            // tmpnam(tmpFileName);
            if (CompilationContext::GetInstance()->Debug) {
                std::cout << "temp file is: " << tmpFileName << "\n";
            }

            context->CurrentFileName = inputFile;

            std::string cmdline = "cpp -nostdinc --sysroot ." + cpp_args + " " + inputFile + " -o " + tmpFileName;
            std::cout << "Preprocessing with cmd \"" + cmdline << "\"" << std::endl;

            if (system(cmdline.c_str()) != 0) {
                return 1;
            }

            if (CompilationContext::GetInstance()->Debug) {
                printf("--------------------------------------\n");
                printf("parsing...\n");
            }

            CSourceParser *frontend = new CSourceParser();
            frontend->Parse(tmpFileName);

            // Note: leave tmpFile for user to check
            // remove(tmpFileName);

            if (CompilationContext::GetInstance()->Debug) {
                printf("--------------------------------------\n");
                printf("ConstantPropagation...\n");
            }

            ConstantPropagation *constantPropagation = new ConstantPropagation();
            context->CodeDom->Accept(constantPropagation);

            if (CompilationContext::GetInstance()->Debug) {
                printf("--------------------------------------\n");
                printf("ConstantPropagation...\n");
            }

            TypeDeduction *typeDeduction = new TypeDeduction();
            context->CodeDom->Accept(typeDeduction);

            if (CompilationContext::GetInstance()->Debug) {
                printf("--------------------------------------\n");
                printf("codeDom Dump:\n");
                ExpressionTreeDumper *codeDomDump = new ExpressionTreeDumper();

                context->CodeDom->Accept(codeDomDump);
            }

            ILGenerator *ilgen = new ILGenerator();
            context->CodeDom->Accept(ilgen);

            il = ilgen->GetILProgram();
        } else {
            std::cerr << "Currently only c0 source files are supported!\n";
            return 2;
        }
    }

    if (il == NULL) {
        return 3;
    }

    if (CompilationContext::GetInstance()->Debug && il != NULL) {
        std::string baseFileName = CompilationContext::GetInstance()->OutputFile;
        baseFileName = ReplaceFilePathExtension(baseFileName, ".il");

        std::ofstream ildump(baseFileName.c_str());
        for (std::vector<ILClass *>::iterator cit = il->Claases.begin(); cit != il->Claases.end(); ++cit) {
            ILClass *c = *cit;

            ildump << "class " << c->ClassSymbol->Name << std::endl << "{" << std::endl;

            for (std::vector<ILFunction *>::iterator fit = c->Functions.begin(); fit != c->Functions.end(); ++fit) {
                ILFunction *f = *fit;
                ildump << "    function " << f->FunctionSymbol->Name << std::endl << "    {" << std::endl;
                for (std::vector<IL>::iterator iit = f->Body.begin(); iit != f->Body.end(); ++iit) {
                    IL &il = *iit;
                    if (il.Opcode == IL::Label) {
                        ildump << "        " << il.ToString() << std::endl;
                    } else {
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

    if (CompilationContext::GetInstance()->Debug && il != NULL) {
        // printf("--------------------------------------\n");
        // printf("Optimized IL:\n");
        std::string baseFileName = CompilationContext::GetInstance()->OutputFile;
        baseFileName = ReplaceFilePathExtension(baseFileName, ".opt.il");

        std::ofstream ildump(baseFileName.c_str());
        for (std::vector<ILClass *>::iterator cit = il->Claases.begin(); cit != il->Claases.end(); ++cit) {
            ILClass *c = *cit;

            ildump << "class " << c->ClassSymbol->Name << std::endl << "{" << std::endl;

            for (std::vector<ILFunction *>::iterator fit = c->Functions.begin(); fit != c->Functions.end(); ++fit) {
                ILFunction *f = *fit;
                ildump << "    function " << f->FunctionSymbol->Name << std::endl << "    {" << std::endl;
                for (std::vector<IL>::iterator iit = f->Body.begin(); iit != f->Body.end(); ++iit) {
                    IL &il = *iit;
                    if (il.Opcode == IL::Label) {
                        ildump << "        " << il.ToString() << std::endl;
                    } else {
                        ildump << "            " << il.ToString() << std::endl;
                    }
                }
                ildump << "    }" << std::endl;
            }
            ildump << "}" << std::endl;
        }

        ildump.close();
    }

    if (CompilationContext::GetInstance()->Debug) {
        std::string baseFileName = CompilationContext::GetInstance()->OutputFile;
        std::string mapFileName = ReplaceFilePathExtension(baseFileName, ".var");

        std::ofstream mapdump(mapFileName.c_str());
        DumpScopeTypes(SymbolScope::GetRootScope(), mapdump, "");
        mapdump.close();
    }

    try {
        CC0Obj obj(context->CodeDom, context->IL);
        std::ofstream filestream(CompilationContext::GetInstance()->OutputFile.c_str());
        ::boost::archive::xml_oarchive c0_obj_archive(filestream);
        c0_obj_archive & BOOST_SERIALIZATION_NVP(obj);
    } catch (std::exception& e) {
        std::cerr << "Serialization to object file failed!\n exception: " << e.what() << "\n";
        return 1;
    }
    return 0;

}

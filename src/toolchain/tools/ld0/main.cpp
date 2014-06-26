#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <queue>
#include <list>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <boost/archive/xml_iarchive.hpp>

#include "core/Core.h"
#include "backend/i0/I0CodeGenerator.h"
#include "binary/flat/FlatFileWriter.h"
#include "core/Symbol/SymbolScope.h"
#include "core/Serialization/ExportDeriveTypes.h"
#include "core/Serialization/ExportDeriveExpressions.h"
#include "core/Serialization/ObjFormat.h"
#include "core/Misc/FilePath.h"

#include "../../../external/mem.h"
#include "../../../external/sys_config.h"

namespace {
template<class Container>
class TagIterator: public ::std::pair<typename Container::const_iterator, const Container*> {
private:
    TagIterator();
public:
    typedef ::std::pair<typename Container::const_iterator, const Container*> _parent_type;
    typedef typename _parent_type::first_type first_type;
    typedef typename _parent_type::second_type second_type;
    TagIterator(const first_type& a, const second_type& b) :
            _parent_type(a, b) {
    }
    TagIterator(const second_type& container) :
            _parent_type(container->begin(), container) {
    }
    TagIterator(const TagIterator& other) :
            _parent_type(other) {
    }
    bool operator<(const TagIterator& other) const {
        return *(*this).first < *other.first;
    }
    bool operator>(const TagIterator& other) const {
        return *other.first < *(*this).first;
    }
};

template<class Container>
class TagItPool: public ::std::priority_queue<TagIterator<Container>, ::std::vector<TagIterator<Container> >, ::std::greater<TagIterator<Container> > > {
public:
    typedef ::std::priority_queue<TagIterator<Container>, ::std::vector<TagIterator<Container> >, ::std::greater<TagIterator<Container> > > _parent_type;
    typedef typename _parent_type::value_type value_type;
    typedef typename _parent_type::size_type size_type;
    void push(const value_type& tagit) {
        if (tagit.first != tagit.second->end()) {
            _parent_type::push(tagit);
        }
    }
    value_type pull() {
        value_type ret = _parent_type::top();
        _parent_type::pop();
        value_type nxt(ret);
        ++nxt.first;
        push(nxt);
        return ret;
    }
    ::std::list<value_type> next() {
        ::std::list<value_type> ret;
        ret.push_back(pull());
        while (!_parent_type::empty()) {
            if (_parent_type::top().first->first == ret.front().first->first) {
                ret.push_back(pull());
            } else {
                break;
            }
        }
        return ret;
    }
};

typedef ::std::map< ::std::string, Symbol*> symbol_map_t;
typedef TagIterator<symbol_map_t> sym_tagit_t;
typedef TagItPool<symbol_map_t> sym_tagit_pool_t;
typedef ::std::set< ::std::string> realloc_record_t;
typedef ::std::map<SymbolScope*, realloc_record_t> realloc_map_t;
typedef ::std::list<sym_tagit_t> sym_merge_list_t;
typedef ::std::map< ::std::string, ILFunction*> il_func_map_t;

}

void DumpScope(SymbolScope *scope, ::std::ofstream &dump) {
    char buffer[100];

    for (std::map<std::string, Symbol *>::iterator it = scope->GetSymbolTable()->begin(); it != scope->GetSymbolTable()->end(); ++it) {
        Symbol *symbol = it->second;
        if (typeid(*(symbol->DeclType)) == typeid(FunctionType) || scope->GetScopeKind() == SymbolScope::Global) {
            sprintf(buffer, "%0llX\t%s", (long long) symbol->Address, symbol->Name.c_str());
            dump << buffer << ::std::endl;
        }
    }

    for (std::vector<SymbolScope *>::iterator it = scope->GetChildScopes()->begin(); it != scope->GetChildScopes()->end(); ++it) {
        SymbolScope *cs = *it;
        DumpScope(cs, dump);
    }
}

CC0Obj load_file_context(const ::std::string& file)
try {
    ::std::ifstream f(file.c_str());
    ::boost::archive::xml_iarchive obj_file(f);
    CC0Obj obj;
    obj_file & BOOST_SERIALIZATION_NVP(obj);
    return obj;
}
catch (std::exception& e) {
    std::cerr << file << " not loaded!\n";
    throw;
}

void check_merge_symbol(const ::std::list<sym_tagit_t>& list) {

    Symbol* pFirstSym = list.front().first->second;
    Type* pFirstTy = pFirstSym->DeclType;
    Type::TypeSpecifier FirstSpec = pFirstTy->GetSpecifiers();
    if (dynamic_cast<FunctionType*>(pFirstTy)) {
        ::std::cerr << "[LINK]: function \"" << list.front().first->first << "\" might not be defined!\n";
        throw ::std::runtime_error("function not found!\n");
    }
    for (::std::list<sym_tagit_t>::const_iterator i = list.begin(), iE = list.end(); i != iE; ++i) {
        Symbol* pSym = i->first->second;
        if (pSym->Kind != Symbol::ObjectName) {
            ::std::cerr << pSym->Name << " symbol kind mismatch!\n";
            throw ::std::runtime_error("[LINK]: symbol kind mismatch!\n");
        }
        Type* pTy = pSym->DeclType;
        if (!pFirstTy->Equals(pTy)) {
            ::std::cerr << pSym->Name << " symbol type mismatch!\n";
            throw ::std::runtime_error("[LINK]: symbol type mismatch!\n");
        }
        Type::TypeSpecifier Spec = pTy->GetSpecifiers();
        if (FirstSpec != Spec) {
            ::std::cerr << pSym->Name << " symbol type specifiers mismatch!\n";
            throw ::std::runtime_error("[LINK]: symbol type specifiers mismatch!\n");
        }
    }
}

void ilprogram_symref_fixup(ILProgram* ilprogram) {
    SymbolScope* new_global_scope = ilprogram->Scope;
    symbol_map_t& new_sym_map = *ilprogram->Scope->GetSymbolTable();
    ILClass& ilclass = *ilprogram->Claases[0];

    //fix global symbol scope refs
    for (::std::vector<ILFunction *>::iterator i = ilclass.Functions.begin(), iE = ilclass.Functions.end(); i != iE; ++i) {
        SymbolScope* old_global_scope = (*i)->FunctionSymbol->Scope;
        for (::std::vector<IL>::iterator j = (*i)->Body.begin(), jE = (*i)->Body.end(); j != jE; ++j) {
            for (::std::vector<IL::ILOperand>::iterator p = j->Operands.begin(), pE = j->Operands.end(); p != pE; ++p) {
                if (p->OperandKind == IL::Variable) {
                    if (p->SymRef->Scope == old_global_scope) {
                        p->SymRef->Scope = new_global_scope;
                        if (CompilationContext::GetInstance()->Debug) {
                            ::std::cout << "[LINK]: relinked symbol " << p->SymRef->Name << " in function " << (*i)->FunctionSymbol->Name << "\n";
                        }
                    }
                }
            }
        }
    }

    //fix global symbol symbols parent refs (Symbol::Scope)
    for (symbol_map_t::iterator i = new_sym_map.begin(), iE = new_sym_map.end(); i != iE; ++i) {
        i->second->Scope = new_global_scope;
        if (CompilationContext::GetInstance()->Debug) {
            ::std::cout << "[LINK]: fixed " << i->second->Name << "\n";
        }
    }
}

void purge_program(ILProgram* ilprogram) {
    //build function map
    il_func_map_t func_map;
    symbol_map_t purged_map;
    ::std::vector<ILFunction*> purged_funcs;
    ::std::vector<SymbolScope*> purged_func_scopes;
    symbol_map_t& orig_map = *ilprogram->Scope->GetSymbolTable();
    ::std::vector<ILFunction*>& orig_funcs = ilprogram->Claases[0]->Functions;
    ::std::vector<SymbolScope*>& orig_func_scopes = *ilprogram->Scope->GetChildScopes();

    //construct the string -> ILFunction* map
    for (::std::vector<ILFunction*>::const_iterator i = orig_funcs.begin(), iE = orig_funcs.end(); i != iE; ++i) {
        func_map.insert(::std::make_pair((*i)->FunctionSymbol->Name, *i));
    }

    //add the root dependency "main" function
    ::std::queue<ILFunction*> func_dep_queue;
    {
        il_func_map_t::iterator i = func_map.find("main");
        if (i == func_map.end()) {
            throw ::std::runtime_error("[LINK]: main function not find!\n");
        }
        func_dep_queue.push(i->second);
    }

    //iterator over the dependency tree
    while (!func_dep_queue.empty()) {
        ILFunction* requested_func = func_dep_queue.front();
        ::std::string& func_name = requested_func->FunctionSymbol->Name;
        if (CompilationContext::GetInstance()->Debug) {
            ::std::cout << "[LINK]: Dep: " << func_name << "\n";
        }
        symbol_map_t::iterator i = purged_map.find(func_name);
        if (i == purged_map.end()) {

            //move the function Symbol to new map
            {
                purged_funcs.push_back(requested_func);
                purged_func_scopes.push_back(requested_func->Scope);
                //XXX: assuming all children scopes are function scope
                purged_map[func_name] = orig_map[func_name];
                orig_map.erase(func_name);
            }

            //collect dependencies
            ::std::vector<IL>& ilcodes = requested_func->Body;
            for (::std::vector<IL>::iterator j = ilcodes.begin(), jE = ilcodes.end(); j != jE; ++j) {
                for (::std::vector<IL::ILOperand>::iterator k = j->Operands.begin(), kE = j->Operands.end(); k != kE; ++k) {
                    if (k->OperandKind == IL::Variable) {
                        if (k->SymRef->Scope == ilprogram->Scope) {
                            symbol_map_t::iterator p = purged_map.find(k->SymRef->Name);
                            if (p == purged_map.end()) {
                                il_func_map_t::iterator r = func_map.find(k->SymRef->Name);
                                if (r != func_map.end()) {
                                    func_dep_queue.push(r->second);
                                } else {
                                    purged_map[k->SymRef->Name] = orig_map[k->SymRef->Name];
                                    orig_map.erase(k->SymRef->Name);
                                }
                            }
                        }
                    }
                }
            }
        }
        func_dep_queue.pop();
    }

    if (CompilationContext::GetInstance()->Debug) {
        for (symbol_map_t::iterator i = orig_map.begin(), iE = orig_map.end(); i != iE; ++i) {
            ::std::cout << "[LINK]: purged sym: " << i->first << "\n";
        }
    }

    //fixup ilprogram
    orig_map = purged_map;
    orig_func_scopes = purged_func_scopes;
    orig_funcs = purged_funcs;

}

ILProgram* merge(::std::vector<ILProgram*> ilprograms) {
    ILProgram* new_ilprogram = new ILProgram();
    realloc_map_t realloc_map;
    sym_tagit_pool_t pool;
    new_ilprogram->Scope = new SymbolScope(NULL, SymbolScope::Global, NULL);
    SymbolScope& new_sym_scope = *new_ilprogram->Scope;
    symbol_map_t& new_sym_map = *new_sym_scope.GetSymbolTable();
    ILClass* new_ilclass = new ILClass(new_ilprogram, new Symbol("Program", new VoidType()));
    new_ilprogram->Claases.push_back(new_ilclass);
    ::std::vector<SymbolScope*>& new_sub_scopes = *new_ilprogram->Scope->GetChildScopes();
    ::std::vector<ILFunction*>& new_global_functions = new_ilclass->Functions;

    //append all children scopes and fix parent link
    //append all global functions and insert the definition (important!) Symbol* to symbol map
    {
        for (::std::vector<ILProgram*>::iterator i = ilprograms.begin(), iE = ilprograms.end(); i != iE; ++i) {
            SymbolScope& sym_scope = *((*i)->Scope);
            symbol_map_t& sym_map = *sym_scope.GetSymbolTable();
            new_sub_scopes.insert(new_sub_scopes.end(), sym_scope.GetChildScopes()->begin(), sym_scope.GetChildScopes()->end());
            ::std::vector<ILFunction*>& funcs = (*i)->Claases[0]->Functions;
            for (::std::vector<ILFunction*>::iterator i = funcs.begin(), iE = funcs.end(); i != iE; ++i) {
                ::std::string& func_name = (*i)->FunctionSymbol->Name;
                if (new_sym_map.find(func_name) != new_sym_map.end()) {
                    throw ::std::runtime_error("[LINK]: function possibly redefined!\n");
                }
                new_sym_map[func_name] = sym_map[func_name];
                new_global_functions.push_back(*i);
            }
        }
        for (::std::vector<SymbolScope*>::iterator i = new_sub_scopes.begin(), iE = new_sub_scopes.end(); i != iE; ++i) {
            (*i)->__SetParentScope(new_ilprogram->Scope);
        }
    }

    //merge symbols in global
    {
        for (::std::vector<ILProgram*>::iterator i = ilprograms.begin(), iE = ilprograms.end(); i != iE; ++i) {
            pool.push(sym_tagit_t((*i)->Scope->GetSymbolTable()));
        }

        if (CompilationContext::GetInstance()->Debug) {
            ::std::cout << "----------\n";
        }
        while (!pool.empty()) {
            sym_merge_list_t ret(pool.next());
            if (CompilationContext::GetInstance()->Debug) {
                for (sym_merge_list_t::iterator i = ret.begin(), iE = ret.end(); i != iE; ++i) {
                    ::std::cout << i->first->first << " with symbol == " << i->first->second << " on scope " << i->second << "\n";
                }
            }
            if (new_sym_map.find(ret.front().first->first) == new_sym_map.end()) {
                check_merge_symbol(ret);
                new_sym_map.insert(*ret.front().first);
            } else {
                if (CompilationContext::GetInstance()->Debug) {
                    ::std::cout << ret.front().first->first << " already inserted\n";
                }
            }
            if (CompilationContext::GetInstance()->Debug) {
                ::std::cout << "----------\n";
            }
        }
        if (CompilationContext::GetInstance()->Debug) {
            ::std::cout << "new global symbols:\n";
            {
                symbol_map_t::iterator i = new_sym_map.begin(), iE = new_sym_map.end();
                while (1) {
                    const ::std::string& sym = i->first;
                    ++i;
                    if (i != iE) {
                        ::std::cout << "├─" << sym << "\n";
                    } else {
                        ::std::cout << "└─" << sym << "\n";
                        break;
                    }
                }
            }
        }
    }

    ilprogram_symref_fixup(new_ilprogram);

    purge_program(new_ilprogram);

    return new_ilprogram;
}

void print_usage(char *) {
    printf("ld0 (link cc0 IL) - A cc0 IL linker.\n"
            "\n"
            "Usage: \n"
            "    ld0 [-g|--debug] [-LDIRs] [-lLIBs] [-o outfile] [input files] [-h|--help]\n"
            "\n"
            "\n"
            "Options:\n"
            "--debug, -g\n"
            "        Output debugging information.\n"
            "-LDIRs...\n"
            "        Specify Library DIRectory\n"
            "-lLIBs...\n"
            "        Link with LIBrary\n"
            "-o\n"
            "        Output to specified file"
            "\n");

    return;
}

int main(int argc, char **argv)
try {
    CompilationContext *context = CompilationContext::GetInstance();

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
    ::std::list< ::std::string> cc0_obj_files;
    ::std::list< ::std::string> lib_obj_files;
    ::std::list< ::std::string> lib_paths;

    //parse command line
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output") == 0) {
            if (argv[i + 1] != NULL && *argv[i + 1] != '-') {
                CompilationContext::GetInstance()->OutputFile = argv[++i];
            } else {
                ::std::cerr << "invalid argument!\n";
                return -1;
            }
        } else if ((strcmp(argv[i], "--debug") == 0) || (strcmp(argv[i], "-g") == 0)) {
            CompilationContext::GetInstance()->Debug = true;
        } else if (strcmp(argv[i], "-L") == 0 && i + 1 < argc) {
            lib_paths.push_back(argv[i + 1]);
            ++i;
        } else if (strncmp(argv[i], "-L", 2) == 0) {
            lib_paths.push_back(argv[i] + 2);
        } else if (strncmp(argv[i], "-l", 2) == 0) {
            ::std::string library_file_name(::std::string("lib") + (argv[i] + 2) + ".o");
            lib_obj_files.push_back(library_file_name);
        } else if ((strcmp(argv[i], "--help") == 0) || strcmp(argv[i], "-h") == 0) {

            print_usage(argv[0]);
            return 1;
        } else {
            cc0_obj_files.push_back(argv[i]);
        }
    }

    //try to import library path from $CC0_LIB
    {
        char* cc0_lib_path = getenv("LD0_LIB");
        if (cc0_lib_path) {
            SplitAndStorePath(cc0_lib_path, back_inserter(lib_paths));
        }
    }

    if (CompilationContext::GetInstance()->Debug) {
        ::std::cout << "ld0 search paths:\n";
        for (::std::list< ::std::string>::iterator i = lib_paths.begin(), iE = lib_paths.end(); i != iE; ++i) {
            ::std::cout << *i << "\n";
        }
    }

    //Search for libraries
    for (::std::list< ::std::string>::iterator i = lib_obj_files.begin(), iE = lib_obj_files.end(); i != iE; ++i) {
        cc0_obj_files.push_back(FindFileInDirs(*i, lib_paths.begin(), lib_paths.end()));
    }

    if (cc0_obj_files.size() == 0) {
        ::std::cerr << "no obj input applied\n";
        return 1;
    }
    if (!CompilationContext::GetInstance()->OutputFile.size()) {
        CompilationContext::GetInstance()->OutputFile = "a.bin";
    }

    //load all object files into memory
    ::std::vector<CC0Obj> objfiles;
    ::std::transform(cc0_obj_files.begin(), cc0_obj_files.end(), ::std::back_inserter(objfiles), load_file_context);
    ::std::vector<ILProgram*> ilprograms;
    for (::std::vector<CC0Obj>::iterator i = objfiles.begin(), iE = objfiles.end(); i != iE; ++i) {
        ilprograms.push_back(i->second);
    }

    //link
    ILProgram* new_il = merge(ilprograms);
    context->IL = new_il;

    //fixup for code generation
    SymbolScope::__SetRootScopt(new_il);

    if (CompilationContext::GetInstance()->Debug) {
        ::std::ofstream ildump(ReplaceFilePathExtension(CompilationContext::GetInstance()->OutputFile, ".ildump").c_str());
        for (::std::vector<ILClass *>::iterator cit = new_il->Claases.begin(), citE = new_il->Claases.end(); cit != citE; ++cit) {
            ILClass *c = *cit;

            ildump << "class " << c->ClassSymbol->Name << ::std::endl << "{" << ::std::endl;

            for (std::vector<ILFunction *>::iterator fit = c->Functions.begin(); fit != c->Functions.end(); ++fit) {
                ILFunction *f = *fit;
                ildump << "    function " << f->FunctionSymbol->Name << ::std::endl << "    {" << ::std::endl;
                for (std::vector<IL>::iterator iit = f->Body.begin(); iit != f->Body.end(); ++iit) {
                    IL &il = *iit;
                    if (il.Opcode == IL::Label) {
                        ildump << "        " << il.ToString() << ::std::endl;
                    } else {
                        ildump << "            " << il.ToString() << ::std::endl;
                    }
                }
                ildump << "    }" << ::std::endl;
            }
            ildump << "}" << ::std::endl;
        }

        ildump.close();
    }
    CodeGenerator* codegen = new I0CodeGenerator();
    codegen->Generate(context->IL);

    if (CompilationContext::GetInstance()->Debug) {
        ::std::string dumpFileName, mapFileName;
        dumpFileName = ReplaceFilePathExtension(CompilationContext::GetInstance()->OutputFile, ".objdump");
        mapFileName = ReplaceFilePathExtension(CompilationContext::GetInstance()->OutputFile, ".map");

        ::std::ofstream objdump(dumpFileName.c_str());
        int64_t currentText = context->TextStart;
        for (std::vector<TargetInstruction *>::iterator iit = context->Target->Code.begin(); iit != context->Target->Code.end(); ++iit) {
            TargetInstruction *inst = *iit;
            char buffer[32];
            sprintf(buffer, "%0llX> \t", (long long) currentText);
            objdump << buffer << inst->ToString().c_str() << ::std::endl;
            currentText += inst->GetLength();
        }

        ::std::ofstream mapdump(mapFileName.c_str());
        DumpScope(SymbolScope::GetRootScope(), mapdump);
    }
    printf("Maximum stack frame size: 0x%llX\n", (long long) (context->MaxStackFrame));

    char *textBuf = new char[0x100000];
    int64_t textSize = 0;
    for (std::vector<TargetInstruction *>::iterator it = context->Target->Code.begin(); it != context->Target->Code.end(); ++it) {
        TargetInstruction *inst = *it;
        inst->Encode(&textBuf[textSize]);
        textSize += inst->GetLength();
    }

    // Write the binary into file
    ::std::string outputFile = CompilationContext::GetInstance()->OutputFile;
    BinaryWriter *binwt = new FlatFileWriter();

    ::std::vector<SectionInfo> sections;
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
catch (std::exception& e) {
    ::std::cerr << "exception thrown: " << e.what() << "\n";
    return 1;
}

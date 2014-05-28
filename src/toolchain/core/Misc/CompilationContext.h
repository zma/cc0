#pragma once
#include <core/Misc/Location.h>
#include <vector>
#include <stdint.h>

class ANTLR3_PARSER_struct;
class Expression;
class ILProgram;
class TargetProgram;

enum {
    CODE_TYPE_DISA = 0,
    CODE_TYPE_I0 = 1,
};

class CompilationContext
{
private:
    static CompilationContext *_instance;
public:
    static CompilationContext * GetInstance();
public:
    bool Debug;
    bool CompileOnly;
    int CodeType;
    int NumErrors;
    int NumWarnings;
    int64_t TextStart;
    int64_t DataStart;
    int64_t RDataStart;
    int64_t BssStart;
    int64_t MaxStackFrame;
    std::vector<std::string> InputFiles;
    std::string OutputFile;

    Expression *CodeDom;
    ILProgram *IL;
    TargetProgram *Target;

    std::string CurrentFileName;
    ANTLR3_PARSER_struct * CurrentParser;
protected:
    CompilationContext();
public:
    virtual ~CompilationContext();
public:
    void ReportError(Location loc, bool cont, const char* msg, ...);
    void ReportWarning(Location loc, const char* msg, ...);

};

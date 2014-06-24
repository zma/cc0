#include "CompilationContext.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string>

CompilationContext *CompilationContext::_instance = NULL;

CompilationContext* CompilationContext::GetInstance()
{
    if (_instance == NULL)
    {
        _instance = new CompilationContext();
    }

    return _instance;
}

CompilationContext::CompilationContext() :TextStart(0), DataStart(0),RDataStart(0), BssStart(0), CodeDom(NULL), IL(NULL), Target(NULL), CurrentParser(NULL)
{
    NumErrors = 0;
    NumWarnings = 0;

    MaxStackFrame = 0;

    //NOTE: Default settings
    Debug = false;

    // NOTE: Default setting
    CompileOnly = false;

    // NOTE: Default setting
    CodeType = CODE_TYPE_I0;

}

CompilationContext::~CompilationContext()
{

}

void CompilationContext::ReportError(Location loc, bool cont, const char* msg, ...)
{
    va_list args;

    va_start(args, msg);
    fprintf(stderr, "Error at %s:(%d, %d) - ", loc.FileName.c_str(), loc.Line, loc.Column);
    vfprintf(stderr, msg, args);
    fprintf(stderr,"\n");
    va_end(args);

    NumErrors++;

    if (!cont)
    {
        exit(1);
    }
}
void CompilationContext::ReportWarning(Location loc, const char* msg, ...)
{
    va_list args;

    va_start(args, msg);
    fprintf(stderr, "Warning at %s:(%d, %d) - ", loc.FileName.c_str(), loc.Line, loc.Column);
    vfprintf(stderr, msg, args);
    fprintf(stderr,"\n");
    va_end(args);

    NumWarnings++;
}

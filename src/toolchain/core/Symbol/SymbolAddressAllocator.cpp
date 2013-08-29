#include "SymbolAddressAllocator.h"
#include <core/Core.h>
#include <stdlib.h>
#include <assert.h>
#include "Symbol.h"
#include <backend/disa/DisaCodeGenerator.h>

#include <typeinfo>

SymbolAddressAllocator::SymbolAddressAllocator()
{
    _globalID = 1;
}

SymbolAddressAllocator::~SymbolAddressAllocator()
{

}

void SymbolAddressAllocator::Allocate(SymbolScope* symbolTable)
{
    switch (symbolTable->GetScopeKind())
    {
        case SymbolScope::Global:
            AllocateGlobal(symbolTable);
            break;
        case SymbolScope::Function:
            AllocateFunction(symbolTable);
            break;
        case SymbolScope::Block:
            AllocateBlock(symbolTable, 0);
            break;
        default:
            abort();
    }
}

int64_t SymbolAddressAllocator::AllocateGlobal(SymbolScope* scope)
{
    uint64_t pos = 8; // This is a padding, so 0 (or NULL) is never used
    int64_t regindex = 0; 

    std::map<std::string, Symbol *>  *symbolTable = scope->GetSymbolTable();
    for (std::map<std::string, Symbol *>::iterator it = symbolTable->begin(); it != symbolTable->end(); ++it)
    {
        Symbol *symbol = it->second;
        Type *type = symbol->DeclType;
        
        if (symbol->Kind == Symbol::TypeName)
        {
            symbol->Address = 0;
        }
        else
        {
            if (typeid(*type) == typeid(LabelType) || typeid(*type) == typeid(FunctionType))
            {
                symbol->Address = 0;
            }
            else
            {
                
                if ((type->GetSpecifiers() & Type::Standalone) != 0)
                {
                    pos = ROUND_UP(pos, 4096);
                    
                    //FIXME: Use appropriate section rather than hardcode bss section
                    symbol->Address = pos + CompilationContext::GetInstance()->BssStart;
                    
                    int64_t size = symbol->DeclType->GetSize();

                    pos += size;
                    pos = ROUND_UP(pos, 4096);
                }
                else if ((type->GetSpecifiers() & Type::Register) != 0)
                {
                    int64_t size = symbol->DeclType->GetSize();
                    if (size > PointerType::PointerSize)
                    {
                        CompilationContext::GetInstance()->ReportError(Location(), false, "Register variable type cannot be larger than pointer size.");
                    }
                    
                    if (regindex >= 4)
                    {
                        CompilationContext::GetInstance()->ReportError(Location(), false, "Too many register variables, max 4.");
                    }
                    
                    symbol->Address = DisaCodeGenerator::GENERAL_REG_3 + regindex * PointerType::PointerSize;
                    
                    regindex++;
                }
                else
                {
                    // Align with the size boundary, e.g, 32-bit integer must be aligned with 4-bytes boundary
                    int64_t size = type->GetSize();
                    pos = ROUND_UP(pos, MIN(size, PointerType::PointerSize));
                    
                    //FIXME: Use appropriate section rather than hardcode bss section
                    symbol->Address = pos + CompilationContext::GetInstance()->BssStart;
                    
                    pos += size;
                }
            }
        }
    }

    pos = ROUND_UP(pos, 4096);
    scope->SetMemorySize(pos);

    std::vector<SymbolScope *> *childScopes = scope->GetChildScopes();
    for (std::vector<SymbolScope *>::iterator it = childScopes->begin(); it != childScopes->end(); ++it)
    {
        SymbolScope *child = *it;
        switch (child->GetScopeKind())
        {
            case SymbolScope::Function:
                AllocateFunction(child);
                break;
            default:
                abort();
        }
    }

    return pos;
}

int64_t SymbolAddressAllocator::AllocateBlock(SymbolScope* scope, int64_t startOffset)
{
    int64_t totalSize = 0;
    int64_t pos = startOffset;

    std::map<std::string, Symbol *>  *symbolTable = scope->GetSymbolTable();
    for (std::map<std::string, Symbol *>::iterator it = symbolTable->begin(); it != symbolTable->end(); ++it)
    {
        Symbol *symbol = it->second;

        Type *type = symbol->DeclType;
        int64_t size = type->GetSize();
        
         // Stack is always aligned at the pointer size boundary
        size = ROUND_UP(size, PointerType::PointerSize);
        
        pos -= size;    // Stack grows to the lower address
             
        if (symbol->Kind == Symbol::TypeName)
        {
            symbol->Address = 0;
        }
        else
        {
            if (typeid(*type) == typeid(LabelType) || typeid(*type) == typeid(FunctionType))
            {
                symbol->Address = 0;
            }
            else
            {
                symbol->Address = pos;
            }
        }



        totalSize += size;
    }

    int64_t maxChildSize = 0;
    std::vector<SymbolScope *> *childScopes = scope->GetChildScopes();

    for (std::vector<SymbolScope *>::iterator it = childScopes->begin(); it != childScopes->end(); ++it)
    {
        /* NOTE: Side by side local scopes shares the same memory blocks
         *
         * e.g
         *
         * int foo()
         * {
         *      char f;
         *
         *      {
         *              long a;
         *      }
         *      {
         *              int b,c;
         *      }
         * }
         *
         * Here f will be at bp[-8], and aligned to 64-bit boundary
         * a and b will shares the same memory with bp[-16],
         * and c is bp[-24]
         *
         * The entire size of the frame is 24, (16 bytes local vars plus previous frame pointer)
         */
        SymbolScope *child = *it;
        int64_t childSize = 0;
        switch (child->GetScopeKind())
        {
            case SymbolScope::Block:
                childSize = AllocateBlock(child, pos);
                break;
        }
        if (childSize > maxChildSize)
        {
            maxChildSize = childSize;
        }
    }

    totalSize += maxChildSize;
    scope->SetMemorySize(totalSize);
    return totalSize;
}


int64_t SymbolAddressAllocator::AllocateFunction(SymbolScope* scope)
{
    /* NOTE: The parameters have positive address offset to the stack frame pointer
     * The stack looks like this
     *
     * <frame bottom of cllller>
     * parameter 3
     * parameter 2
     * parameter 1       FP[0x10 + retSize]                 (via pushing arguments)
     * space for retval  FP[0x10]
     * return address    FP[0x08]                       (via br.j)
     * caller's FP  FP[0]  <- frame pointer
     * local var 1  FP[-8]
     * local var 2
     * local var 3
     * ...
     * local var N
     * target's argument N
     * target's argument N-1
     * ...
     * target's argument 1
     * <target return address> <- stack pointer (before calling)
     *
     *
     */
    int64_t totalSize = 0; // At least it has a frame pointer chain

    FunctionExpression *function = dynamic_cast<FunctionExpression *>(scope->GetAssociatedExpression());
    assert(function != NULL);
    int64_t retSize = function->GetReturnType()->GetSize();

    //Skip caller's SP and return address
    int64_t pos = PointerType::PointerSize * 2 + ROUND_UP(retSize, PointerType::PointerSize);

    Symbol *funcSym = function->GetSymbol();
    assert(funcSym != NULL && typeid(*(funcSym->DeclType)) == typeid(FunctionType));

    funcSym->Address = 0;

    std::vector<Declaration *> *parameters = function->GetParameters();

    for (std::vector<Declaration *>::iterator it = parameters->begin(); it != parameters->end(); ++it)
    {
        Declaration *decl = *it;
        Symbol *paramSymbol = scope->LookupLocal(decl->GetName());
        assert(paramSymbol != NULL);
        assert(paramSymbol->DeclType->Equals(decl->GetType()));


        Type *type = paramSymbol->DeclType;
        if (typeid(*type) == typeid(LabelType) || typeid(*type) == typeid(FunctionType))
        {
            paramSymbol->Address = 0;
        }
        else
        {
            paramSymbol->Address = pos;
        }


        int64_t size = paramSymbol->DeclType->GetSize();

        //Alignment, NOTE: must be consistent with the block alignment
        pos = ROUND_UP(pos, PointerType::PointerSize);

        pos += size;
    }

    std::vector<SymbolScope *> *childScopes = scope->GetChildScopes();

    int64_t maxChildSize = 0;
    for (std::vector<SymbolScope *>::iterator it = childScopes->begin(); it != childScopes->end(); ++it)
    {
        SymbolScope *child = *it;
        int64_t childSize = 0;
        switch (child->GetScopeKind())
        {
            case SymbolScope::Block:
                childSize = AllocateBlock(child, 0);
                break;
        }
        if (childSize > maxChildSize)
        {
            maxChildSize = childSize;
        }
    }

    totalSize += maxChildSize;
    scope->SetMemorySize(totalSize);
    
    int64_t maxStackFrame = CompilationContext::GetInstance()->MaxStackFrame ;
    maxStackFrame = MAX(maxStackFrame, totalSize);
    
    CompilationContext::GetInstance()->MaxStackFrame = maxStackFrame; 
    
    return totalSize;
}


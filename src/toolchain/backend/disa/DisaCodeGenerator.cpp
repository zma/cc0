#include "DisaCodeGenerator.h"
#include "DisaInstruction.h"

#include <core/Core.h>
#include <core/Symbol/SymbolAddressAllocator.h>

#include <assert.h>
#include <iostream>
#include <cstdio>

DisaCodeGenerator::DisaCodeGenerator()
{
}

DisaCodeGenerator::~DisaCodeGenerator()
{

}

void DisaCodeGenerator::EnterProgram(ILProgram* program)
{
    CodeGenerator::EnterProgram(program);
    CompilationContext::GetInstance()->Target = new TargetProgram();
    SymbolAddressAllocator *allocator = new SymbolAddressAllocator();
    allocator->Allocate(SymbolScope::GetRootScope());
    SymbolScope::GetRootScope()->Dump();

    while (!_staticChain.empty())
    {
        _staticChain.pop();
    }
    
    _staticChain.push(SymbolScope::GetRootScope());
    _currentText = CompilationContext::GetInstance()->TextStart;

    GenerateCrt0();
    GenerateNewr0();
}

void DisaCodeGenerator::LeaveProgram(ILProgram* program)
{
    CodeGenerator::LeaveProgram(program);
    _staticChain.pop();

    ResolveSymbols();
}

void DisaCodeGenerator::EnterClass(ILClass* ilclass)
{
    CodeGenerator::EnterClass(ilclass);
}

void DisaCodeGenerator::LeaveClass(ILClass* ilclass)
{
    CodeGenerator::LeaveClass(ilclass);
}

void DisaCodeGenerator::EnterFunction(ILFunction* ilfunc)
{
    CodeGenerator::EnterFunction(ilfunc);
    _staticChain.push(ilfunc->Scope);
    ilfunc->FunctionSymbol->Address = _currentText;

    /*
     * Original:
     *   push %rbp
     *   mov %rsp, %rbp
     *   sub %rsp, $SIZE
     *
     * Equivilent:
     *   sub %rsp, $0x8
     *   mov %rbp, (%rsp)
     *   mov %rsp, %rbp
     *   sub %rsp, $SIZE
     *
     * Optimized:
     *   mov %rbp, -8(%rsp)
     *   sub %rsp, $8, %rbp
     *   sub %rsp, $(8 + SIZE), %rsp
     */

    DisaInstruction *inst;

    inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::MOV;
    inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, FRAME_POINTER, DisaInstruction::MovSigned64);
    inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::BaseDisplacement, STACK_POINTER, (-PointerType::PointerSize), DisaInstruction::MovSigned64);
    Emit(inst);

    inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::SUB;
    inst->Attribute = DisaInstruction::Signed64;
    inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, STACK_POINTER, DisaInstruction::MovSigned64);
    inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, (int64_t)(PointerType::PointerSize), DisaInstruction::MovSigned64);
    inst->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, FRAME_POINTER, DisaInstruction::MovSigned64);
    Emit(inst);

    inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::SUB;
    inst->Attribute = DisaInstruction::Signed64;
    inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, STACK_POINTER, DisaInstruction::MovSigned64);
    inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, (int64_t)(ilfunc->Scope->GetMemorySize() + PointerType::PointerSize), DisaInstruction::MovSigned64);
    inst->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, STACK_POINTER, DisaInstruction::MovSigned64);
    Emit(inst);

}

void DisaCodeGenerator::LeaveFunction(ILFunction* ilfunc)
{
    CodeGenerator::LeaveFunction(ilfunc);
    _staticChain.pop();
}

void DisaCodeGenerator::ResolveSymbols()
{
    TargetProgram *program = CompilationContext::GetInstance()->Target;
    for(std::vector<TargetInstruction *>::iterator it = program->Code.begin(); it != program->Code.end(); ++it)
    {
        DisaInstruction *inst = dynamic_cast<DisaInstruction *>(*it);
        for(int i = 0; i < DisaInstruction::MAX_NUM_OPERANDS; i++)
        {
            SymbolRef *symRef = inst->Operands[i].SymRef;
            if(symRef != NULL)
            {
                Symbol *symbol = symRef->Lookup();
                if(symbol == NULL || symbol->Address == 0)
                {
                    CompilationContext::GetInstance()->ReportError(Location(), false, "Cannot resolved symbol \'%s\'.", symRef->Name.c_str());
                }
                inst->Operands[i].Address = symbol->Address;
                std::cout << "Resolving symbol \'" << symbol->Name << "\' -> 0x" << std::uppercase << std::hex << symbol->Address << std::endl;
                inst->Operands[i].SymRef = NULL;
            }
        }
    }
}

void DisaCodeGenerator::GenerateCrt0()
{
    Symbol *sym = SymbolScope::GetRootScope()->Lookup("__crt0");
    if(sym == NULL)
    {
        sym = new Symbol("__crt0", new LabelType());
        SymbolScope::GetRootScope()->Add(sym);
        sym = SymbolScope::GetRootScope()->Lookup("__crt0");
    }

    assert(sym != NULL);
    sym->Address = _currentText;

    DisaInstruction *inst;

    inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::ADD;
    inst->Attribute = DisaInstruction::Signed64;
    inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, CURRENT_STACK_BASE, DisaInstruction::MovSigned64);
    inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, CURRENT_STACK_SIZE, DisaInstruction::MovSigned64);
    inst->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, STACK_POINTER, DisaInstruction::MovSigned64);
    Emit(inst);

    inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::MOV;
    inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, (int64_t)0, DisaInstruction::MovSigned64);
    inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, FRAME_POINTER, DisaInstruction::MovSigned64);
    Emit(inst);

    Symbol *mainSym = SymbolScope::GetRootScope()->Lookup("main");
    if(mainSym == NULL)
    {
        CompilationContext::GetInstance()->ReportError(Location(), false, "Cannot find \'main\' function.");
    }

    inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::BR;
    inst->JumpMode = DisaInstruction::J;
    inst->RelativeJump = false;
    inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, 0xFFFFFFFFDEADBEEF, DisaInstruction::MovSigned64);
//     if (mainSym->Address == 0)
//     {
    inst->Operands[0].SymRef = new SymbolRef(SymbolScope::GetRootScope(), "main");
//     }
    Emit(inst);

}

void DisaCodeGenerator::GenerateNewr0()
{
    Symbol *sym = SymbolScope::GetRootScope()->Lookup("__newr0");
    if(sym == NULL)
    {
        sym = new Symbol("__newr0", new LabelType());
        SymbolScope::GetRootScope()->Add(sym);
        sym = SymbolScope::GetRootScope()->Lookup("__newr0");
    }

    assert(sym != NULL);
    sym->Address = _currentText;

    DisaInstruction *inst;
    
    inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::ADD;
    inst->Attribute = DisaInstruction::Signed64;
    inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, CURRENT_STACK_BASE, DisaInstruction::MovSigned64);
    inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, CURRENT_STACK_SIZE, DisaInstruction::MovSigned64);
    inst->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, DisaInstruction::MovSigned64);
    Emit(inst);

    inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::MOV;
    inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, (int64_t)0, DisaInstruction::MovSigned64);
    inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, FRAME_POINTER, DisaInstruction::MovSigned64);
    Emit(inst);

    inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::SUB;
    inst->Attribute = DisaInstruction::Signed64;
    inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, DisaInstruction::MovSigned64);
    inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, (int64_t)(PointerType::PointerSize * 3), DisaInstruction::MovSigned64);
    inst->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, STACK_POINTER, DisaInstruction::MovSigned64);
    Emit(inst);

    inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::SUB;
    inst->Attribute = DisaInstruction::Signed64;
    inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, STACK_POINTER, DisaInstruction::MovSigned64);
    inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::BaseDisplacement, GENERAL_REG_1, (int64_t)(- PointerType::PointerSize * 2), DisaInstruction::MovSigned64);
    inst->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, STACK_POINTER, DisaInstruction::MovSigned64);
    Emit(inst);

    inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::MOV;
    inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::BaseDisplacement, GENERAL_REG_1, (int64_t)(- PointerType::PointerSize), DisaInstruction::MovSigned64);
    inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_2, DisaInstruction::MovSigned64);
    Emit(inst);

    inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::MOV;
    inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, (int64_t)0, DisaInstruction::MovSigned64);
    inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Indirect, STACK_POINTER, DisaInstruction::MovSigned64);
    Emit(inst);
    
    int64_t *pRetAddr = &inst->Operands[0].Address;
    
    inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::BR;
    inst->JumpMode = DisaInstruction::JI;
    inst->RelativeJump = false;
    inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_2, DisaInstruction::MovSigned64);
    Emit(inst);
    
    *pRetAddr = _currentText;
    
    Emit("exit:c");
}


void DisaCodeGenerator::GenerateDisaOperand(IL::ILOperand ilOperand, DisaInstruction* inst, int operandIndex)
{
    switch(ilOperand.OperandKind)
    {
        case IL::Constant:
            inst->Operands[operandIndex].AddressingMode = DisaInstruction::Immediate;
            switch(ilOperand.OperandType)
            {
                case IL::Void:
                case IL::I:
                case IL::I1:
                case IL::I2:
                case IL::I4:
                case IL::I8:
                    inst->Operands[operandIndex].IntValue = ilOperand.IValue;
                    break;
                case IL::U:
                case IL::U1:
                case IL::U2:
                case IL::U8:
                    inst->Operands[operandIndex].UIntValue = ilOperand.UValue;
                    break;
                case IL::R4:
                    inst->Operands[operandIndex].SingleValue = (float)ilOperand.RValue;
                    break;
                case IL::R8:
                    inst->Operands[operandIndex].DoubleValue = ilOperand.RValue;
                    break;
                case IL::Str:
                    assert(!"Not implemented: string literals.");
                    break;
            }

            break;
        case IL::Variable:
        {
            Symbol *symbol = ilOperand.SymRef->Lookup();
            assert(symbol != NULL);
            if(symbol->Scope->GetScopeKind() == SymbolScope::Global || typeid(*(symbol->DeclType)) == typeid(FunctionType) || typeid(*(symbol->DeclType)) == typeid(LabelType))
            {
                if(typeid(*(symbol->DeclType)) == typeid(ArrayType))
                {
                    inst->Operands[operandIndex].AddressingMode = DisaInstruction::Immediate;
                }
                else
                {
                    inst->Operands[operandIndex].AddressingMode = DisaInstruction::Direct;
                }

                inst->Operands[operandIndex].Address = 0xFFFFFFFFDEADBEEF;
                inst->Operands[operandIndex].SymRef = new SymbolRef(symbol);

            }
            else
            {
                if(typeid(*(symbol->DeclType)) == typeid(ArrayType))
                {
                    DisaInstruction *addrInst = new DisaInstruction();
                    addrInst->OpCode = DisaInstruction::ADD;
                    addrInst->Attribute = DisaInstruction::Signed64;
                    addrInst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, FRAME_POINTER, DisaInstruction::MovSigned64);
                    addrInst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, symbol->Address, DisaInstruction::MovSigned64);
                    addrInst->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_2, DisaInstruction::MovSigned64);
                    Emit(addrInst);
                    inst->Operands[operandIndex].AddressingMode = DisaInstruction::Direct;
                    inst->Operands[operandIndex].Address = GENERAL_REG_2;
                }
                else
                {
                    inst->Operands[operandIndex].AddressingMode = DisaInstruction::BaseDisplacement;
                    inst->Operands[operandIndex].Address = FRAME_POINTER;
                    inst->Operands[operandIndex].Displacement = symbol->Address;
                }
            }
            break;
        }
        case IL::Empty:
        case IL::FieldToken:
        case IL::TypeToken:
        default:
            assert(!"Invalid IL operand kind.");
    }

    inst->Operands[operandIndex].MovAttribute = GetMovOperandAttribute(ilOperand.OperandType);
}

void DisaCodeGenerator::Emit(DisaInstruction* inst)
{
    CompilationContext::GetInstance()->Target->Code.push_back(inst);
    _currentText += inst->GetLength();
}

void DisaCodeGenerator::Emit(std::string disa)
{
    DisaInstruction *inst = new DisaInstruction(_staticChain.top(), disa);
    Emit(inst);
}


void DisaCodeGenerator::GenerateBinaryArithmatic(IL* il, DisaInstruction::DisaOpcode opcode)
{
    DisaInstruction *inst = new DisaInstruction();
    DisaInstruction::MovOperandAttribute movattr;
    inst->OpCode = opcode;

    bool needsIntermediates = false;
    switch(il->Operands[0].OperandType)
    {
        case IL::I1:
        case IL::I2:
        case IL::I4:
            needsIntermediates = true;
        case IL::I8:
        case IL::Void:
        case IL::I:
        case IL::Str:
            inst->Attribute = DisaInstruction::Signed64;
            movattr = DisaInstruction::MovSigned64;
            break;
        case IL::U1:
        case IL::U2:
        case IL::U4:
            needsIntermediates = true;
        case IL::U8:
        case IL::U:
            inst->Attribute = DisaInstruction::Unsigned64;
            movattr = DisaInstruction::MovUnsigned64;
            break;
        case IL::R4:
            inst->Attribute = DisaInstruction::Single;
            movattr = DisaInstruction::MovSingle;
            break;
        case IL::R8:
            inst->Attribute = DisaInstruction::Double;
            movattr = DisaInstruction::MovDouble;
            break;
        default:
            abort();
    }

    if(needsIntermediates)
    {
        DisaInstruction *loadInst1 = new DisaInstruction();
        loadInst1->OpCode = DisaInstruction::MOV;
        GenerateDisaOperand(il->Operands[1], loadInst1, 0);
        loadInst1->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, movattr);

        DisaInstruction *loadInst2 = new DisaInstruction();
        loadInst2->OpCode = DisaInstruction::MOV;
        GenerateDisaOperand(il->Operands[2], loadInst2, 0);
        loadInst2->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_2, movattr);

        inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, movattr);
        inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_2, movattr);
        inst->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, movattr);

        DisaInstruction *saveInst = new DisaInstruction();
        saveInst->OpCode = DisaInstruction::MOV;
        saveInst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, movattr);
        GenerateDisaOperand(il->Operands[0], saveInst, 1);

        Emit(loadInst1);
        Emit(loadInst2);
        Emit(inst);
        Emit(saveInst);
    }
    else
    {
        GenerateDisaOperand(il->Operands[0], inst, 2);
        GenerateDisaOperand(il->Operands[1], inst, 0);
        GenerateDisaOperand(il->Operands[2], inst, 1);
        Emit(inst);
    }
}

void DisaCodeGenerator::GenerateComparason(IL* il, DisaInstruction::BrJumpMode mode, bool brValue)
{
    /*
     * [0]     br:xxx TL
     * [1] FL: mov 0:msq, target:msq
     * [2]     br:j EL
     * [3] TL: mov 1:msq, target:msq
     * [4] EL:
     */

    DisaInstruction *inst = new DisaInstruction();

    inst->OpCode = DisaInstruction::BR;
    inst->JumpMode = mode;
    inst->RelativeJump = false;

    bool needsIntermediates = false;
    DisaInstruction::MovOperandAttribute movattr;

    switch(il->Operands[0].OperandType)
    {
        case IL::I1:
        case IL::I2:
        case IL::I4:
            needsIntermediates = true;
        case IL::I8:
        case IL::Void:
        case IL::I:
        case IL::Str:
            inst->Attribute = DisaInstruction::Signed64;
            movattr = DisaInstruction::MovSigned64;
            break;
        case IL::U1:
        case IL::U2:
        case IL::U4:
            needsIntermediates = true;
        case IL::U8:
        case IL::U:
            inst->Attribute = DisaInstruction::Unsigned64;
            movattr = DisaInstruction::MovUnsigned64;
            break;
        case IL::R4:
            inst->Attribute = DisaInstruction::Single;
            movattr = DisaInstruction::MovSingle;
            break;
        case IL::R8:
            inst->Attribute = DisaInstruction::Double;
            movattr = DisaInstruction::MovDouble;
            break;
        default:
            abort();
    }

    int64_t *pTL, *pEL;

    if(needsIntermediates)
    {
        DisaInstruction *loadInst1 = new DisaInstruction();
        loadInst1->OpCode = DisaInstruction::MOV;
        GenerateDisaOperand(il->Operands[1], loadInst1, 0);
        loadInst1->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, movattr);

        DisaInstruction *loadInst2 = new DisaInstruction();
        loadInst2->OpCode = DisaInstruction::MOV;
        GenerateDisaOperand(il->Operands[2], loadInst2, 0);
        loadInst2->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_2, movattr);

        inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, movattr);
        inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_2, movattr);
        inst->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, (int64_t)0, movattr);

        Emit(loadInst1);
        Emit(loadInst2);
        Emit(inst);

        pTL = &(inst->Operands[2].Address);
    }
    else
    {
        GenerateDisaOperand(il->Operands[1], inst, 0);
        GenerateDisaOperand(il->Operands[2], inst, 1);
        inst->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, (int64_t)0, movattr);
        Emit(inst);
        pTL = &(inst->Operands[2].Address);
    }


    DisaInstruction *finst = new DisaInstruction();
    finst->OpCode = DisaInstruction::MOV;
    finst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, (int64_t)(brValue ? 0 : 1), DisaInstruction::MovSigned64);
    GenerateDisaOperand(il->Operands[0], finst, 1);
    Emit(finst);


    DisaInstruction *einst = new DisaInstruction();
    einst->OpCode = DisaInstruction::BR;
    einst->JumpMode = DisaInstruction::J;
    einst->RelativeJump = false;
    einst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, (int64_t)0, movattr);

    Emit(einst);
    pEL = &(einst->Operands[0].Address);
    
    *pTL = _currentText;

    DisaInstruction *tinst = new DisaInstruction();
    tinst->OpCode = DisaInstruction::MOV;
    tinst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, (int64_t)(brValue ? 1 : 0), DisaInstruction::MovSigned64);
    GenerateDisaOperand(il->Operands[0], tinst, 1);
    Emit(tinst);

    *pEL = _currentText;

}

void DisaCodeGenerator::GenerateCompareBranch(IL* il, DisaInstruction::BrJumpMode mode, bool brValue)
{
    /*
     * br = true?
     *
     * br.xxx target
     *
     * brValue = false
     *
     * br.xxx FL
     * br.j target
     * FL:
     *
     */

    DisaInstruction *inst = new DisaInstruction();

    inst->OpCode = DisaInstruction::BR;
    inst->JumpMode = mode;
    inst->RelativeJump = false;

    bool needsIntermediates = false;
    DisaInstruction::MovOperandAttribute movattr;

    switch(il->Operands[0].OperandType)
    {
        case IL::I1:
        case IL::I2:
        case IL::I4:
            needsIntermediates = true;
        case IL::I8:
        case IL::Void:
        case IL::I:
        case IL::Str:
            inst->Attribute = DisaInstruction::Signed64;
            movattr = DisaInstruction::MovSigned64;
            break;
        case IL::U1:
        case IL::U2:
        case IL::U4:
            needsIntermediates = true;
        case IL::U8:
        case IL::U:
            inst->Attribute = DisaInstruction::Unsigned64;
            movattr = DisaInstruction::MovUnsigned64;
            break;
        case IL::R4:
            inst->Attribute = DisaInstruction::Single;
            movattr = DisaInstruction::MovSingle;
            break;
        case IL::R8:
            inst->Attribute = DisaInstruction::Double;
            movattr = DisaInstruction::MovDouble;
            break;
        default:
            abort();
    }

    int64_t *pFL;
    if(brValue)
    {
        if(needsIntermediates)
        {
            DisaInstruction *loadInst1 = new DisaInstruction();
            loadInst1->OpCode = DisaInstruction::MOV;
            GenerateDisaOperand(il->Operands[0], loadInst1, 0);
            loadInst1->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, movattr);

            DisaInstruction *loadInst2 = new DisaInstruction();
            loadInst2->OpCode = DisaInstruction::MOV;
            GenerateDisaOperand(il->Operands[1], loadInst2, 0);
            loadInst2->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_2, movattr);

            inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, movattr);
            inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_2, movattr);
            GenerateDisaOperand(il->Operands[2], inst, 2);

            Emit(loadInst1);
            Emit(loadInst2);
            Emit(inst);
        }
        else
        {
            GenerateDisaOperand(il->Operands[0], inst, 0);
            GenerateDisaOperand(il->Operands[1], inst, 1);
            GenerateDisaOperand(il->Operands[2], inst, 2);
            Emit(inst);
        }
    }
    else
    {
        if(needsIntermediates)
        {
            DisaInstruction *loadInst1 = new DisaInstruction();
            loadInst1->OpCode = DisaInstruction::MOV;
            GenerateDisaOperand(il->Operands[0], loadInst1, 0);
            loadInst1->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, movattr);

            DisaInstruction *loadInst2 = new DisaInstruction();
            loadInst2->OpCode = DisaInstruction::MOV;
            GenerateDisaOperand(il->Operands[1], loadInst2, 0);
            loadInst2->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_2, movattr);

            inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, movattr);
            inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_2, movattr);
            inst->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, (int64_t)0, movattr);

            Emit(loadInst1);
            Emit(loadInst2);
            Emit(inst);

            pFL = &(inst->Operands[2].Address);
        }
        else
        {
            GenerateDisaOperand(il->Operands[0], inst, 0);
            GenerateDisaOperand(il->Operands[1], inst, 1);
            GenerateDisaOperand(il->Operands[2], inst, 2);
            Emit(inst);
            pFL = &(inst->Operands[2].Address);
        }

        DisaInstruction *binst = new DisaInstruction();
        binst->OpCode = DisaInstruction::BR;
        binst->JumpMode = DisaInstruction::J;
        binst->RelativeJump = false;
        GenerateDisaOperand(il->Operands[2], binst, 0);

        Emit(inst);

        *pFL = _currentText;
    }
}

void DisaCodeGenerator::GenerateConditionalBr(IL* il, DisaInstruction::BrJumpMode mode)
{
    DisaInstruction *inst = new DisaInstruction();

    inst->OpCode = DisaInstruction::BR;
    inst->JumpMode = mode;
    inst->RelativeJump = false;

    bool needsIntermediates = false;
    DisaInstruction::MovOperandAttribute movattr;

    switch(il->Operands[0].OperandType)
    {
        case IL::I1:
        case IL::I2:
        case IL::I4:
            needsIntermediates = true;
        case IL::I8:
        case IL::Void:
        case IL::I:
        case IL::Str:
            inst->Attribute = DisaInstruction::Signed64;
            movattr = DisaInstruction::MovSigned64;
            break;
        case IL::U1:
        case IL::U2:
        case IL::U4:
            needsIntermediates = true;
        case IL::U8:
        case IL::U:
            inst->Attribute = DisaInstruction::Unsigned64;
            movattr = DisaInstruction::MovUnsigned64;
            break;
        case IL::R4:
            inst->Attribute = DisaInstruction::Single;
            movattr = DisaInstruction::MovSingle;
            break;
        case IL::R8:
            inst->Attribute = DisaInstruction::Double;
            movattr = DisaInstruction::MovDouble;
            break;
        default:
            abort();
    }


    if(needsIntermediates)
    {
        DisaInstruction *loadInst1 = new DisaInstruction();
        loadInst1->OpCode = DisaInstruction::MOV;
        GenerateDisaOperand(il->Operands[0], loadInst1, 0);
        loadInst1->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, movattr);

        inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, movattr);
        GenerateDisaOperand(il->Operands[1], inst, 1);

        Emit(loadInst1);
        Emit(inst);
    }
    else
    {
        GenerateDisaOperand(il->Operands[0], inst, 0);
        GenerateDisaOperand(il->Operands[1], inst, 1);
        Emit(inst);
    }

}


void DisaCodeGenerator::VisitNop(IL* il)
{
    Emit("nop");
}

void DisaCodeGenerator::VisitAdd(IL* il)
{
    GenerateBinaryArithmatic(il, DisaInstruction::ADD);
}

void DisaCodeGenerator::VisitAnd(IL* il)
{
    GenerateBinaryArithmatic(il, DisaInstruction::AND);
}

void DisaCodeGenerator::VisitBrEq(IL* il)
{
    GenerateCompareBranch(il, DisaInstruction::E, true);
}

void DisaCodeGenerator::VisitBrGt(IL* il)
{
    GenerateCompareBranch(il, DisaInstruction::LE, false);
}

void DisaCodeGenerator::VisitBrGe(IL* il)
{
    GenerateCompareBranch(il, DisaInstruction::L, false);
}

void DisaCodeGenerator::VisitBrLt(IL* il)
{
    GenerateCompareBranch(il, DisaInstruction::L, true);

}

void DisaCodeGenerator::VisitBrLe(IL* il)
{
    GenerateCompareBranch(il, DisaInstruction::LE, true);
}

void DisaCodeGenerator::VisitBrNe(IL* il)
{
    GenerateCompareBranch(il, DisaInstruction::NE, true);
}

void DisaCodeGenerator::VisitBr(IL* il)
{
    DisaInstruction *inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::BR;
    inst->JumpMode = DisaInstruction::J;
    inst->RelativeJump = false;
    GenerateDisaOperand(il->Operands[0], inst, 0);
    Emit(inst);
}

void DisaCodeGenerator::VisitBrZ(IL* il)
{
    GenerateConditionalBr(il, DisaInstruction::Z);
}

void DisaCodeGenerator::VisitBrNz(IL* il)
{
    GenerateConditionalBr(il, DisaInstruction::NZ);
}

void DisaCodeGenerator::VisitCall(IL* il)
{
    int64_t pushSize = 0;
    int64_t retSize = 0;

    for(int i = 2; i < il->Operands.size(); i++)
    {
        pushSize += GetOperandSize(il->Operands[i].OperandType, PointerType::PointerSize);
    }

    if(il->Operands[1].OperandKind != IL::Empty)
    {
        retSize = GetOperandSize(il->Operands[1].OperandType, PointerType::PointerSize);
        pushSize += retSize;
    }

    pushSize += PointerType::PointerSize;       // For the return address

    DisaInstruction *allocInst = new DisaInstruction();
    allocInst->OpCode = DisaInstruction::SUB;
    allocInst->Attribute = DisaInstruction::Signed64;
    allocInst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, STACK_POINTER, DisaInstruction::MovSigned64);
    allocInst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, (int64_t)pushSize, DisaInstruction::MovSigned64);
    allocInst->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, STACK_POINTER, DisaInstruction::MovSigned64);
    Emit(allocInst);

    int64_t stackOffset = 0; //retSiz;  PointerType::PointerSize;
    
    DisaInstruction *retAddrInst = new DisaInstruction();
    retAddrInst->OpCode = DisaInstruction::MOV;
    retAddrInst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, (int64_t)0, DisaInstruction::MovSigned64);
    retAddrInst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::BaseDisplacement, STACK_POINTER, stackOffset, DisaInstruction::MovSigned64);
    Emit(retAddrInst);
    
    stackOffset += PointerType::PointerSize;
    
    
    int64_t *pRetAddr = &(retAddrInst->Operands[0].IntValue);

    
    // Allocate space for return value
    stackOffset += retSize;

    // Push parameters
    for(int i = 2; i < il->Operands.size(); i++)
    {
        DisaInstruction *pushInst = NULL;
        pushInst = new DisaInstruction();
        pushInst->OpCode = DisaInstruction::MOV;
        GenerateDisaOperand(il->Operands[i], pushInst, 0);
        pushInst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::BaseDisplacement, STACK_POINTER, stackOffset, pushInst->Operands[0].MovAttribute);
        Emit(pushInst);
        stackOffset += GetOperandSize(il->Operands[i].OperandType, PointerType::PointerSize);
    }



    DisaInstruction *brInst = new DisaInstruction();
    brInst->OpCode = DisaInstruction::BR;
    brInst->JumpMode = DisaInstruction::J;
    brInst->RelativeJump = false;
    GenerateDisaOperand(il->Operands[0], brInst, 0);
    Emit(brInst);

    *pRetAddr = _currentText;
    
    
    // Get the return value
    if (il->Operands[1].OperandKind != IL::Empty)
    {
        DisaInstruction *getRetvInst = new DisaInstruction();
        getRetvInst->OpCode = DisaInstruction::MOV;
        getRetvInst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::BaseDisplacement, STACK_POINTER, PointerType::PointerSize, GetMovOperandAttribute(il->Operands[1].OperandType));
        GenerateDisaOperand(il->Operands[1], getRetvInst, 1);
        Emit(getRetvInst);
    }
    
    DisaInstruction *deallocInst = new DisaInstruction();
    deallocInst->OpCode = DisaInstruction::ADD;
    deallocInst->Attribute = DisaInstruction::Signed64;
    deallocInst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, STACK_POINTER, DisaInstruction::MovSigned64);
    deallocInst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, (int64_t)pushSize, DisaInstruction::MovSigned64);
    deallocInst->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, STACK_POINTER, DisaInstruction::MovSigned64);
    Emit(deallocInst);
}

void DisaCodeGenerator::VisitCallInd(IL* il)
{
    assert(!"Not supported yet: indirect call");
}

void DisaCodeGenerator::VisitCallVirt(IL* il)
{
    assert(!"Not supported yet: virtual call");
}

void DisaCodeGenerator::VisitCast(IL* il)
{
    assert(!"Not supported yet: casting");
}

void DisaCodeGenerator::VisitCompEq(IL* il)
{
    GenerateComparason(il, DisaInstruction::E, true);
}

void DisaCodeGenerator::VisitCompGt(IL* il)
{
    GenerateComparason(il, DisaInstruction::LE, false);
}

void DisaCodeGenerator::VisitCompGe(IL* il)
{
    GenerateComparason(il, DisaInstruction::L, false);
}

void DisaCodeGenerator::VisitCompLt(IL* il)
{
    GenerateComparason(il, DisaInstruction::L, true);
}

void DisaCodeGenerator::VisitCompLe(IL* il)
{
    GenerateComparason(il, DisaInstruction::LE, true);
}

void DisaCodeGenerator::VisitCompNe(IL* il)
{
    GenerateComparason(il, DisaInstruction::NE, true);
}

void DisaCodeGenerator::VisitConv(IL* il)
{
    DisaInstruction *inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::MOV;
    GenerateDisaOperand(il->Operands[1], inst, 0);
    GenerateDisaOperand(il->Operands[0], inst, 1);
    Emit(inst);
}

void DisaCodeGenerator::VisitCpblk(IL* il)
{
    assert(!"Not implemented.");
}

void DisaCodeGenerator::VisitCpobj(IL* il)
{
    assert(!"Not implemented.");
}

void DisaCodeGenerator::VisitDiv(IL* il)
{
    GenerateBinaryArithmatic(il, DisaInstruction::DIV);
}

void DisaCodeGenerator::VisitInitblk(IL* il)
{
    assert(!"Not implemented.");
}

void DisaCodeGenerator::VisitInitobj(IL* il)
{
    assert(!"Not implemented.");
}

void DisaCodeGenerator::VisitLdelem(IL* il)
{
    assert(il->Operands[1].OperandKind == IL::Variable);
    Symbol *sym = il->Operands[1].SymRef->Lookup();

    int64_t elementSize;
    Type *containerType = sym->DeclType;
    if(typeid(*containerType) == typeid(PointerType))
    {
        PointerType *t = dynamic_cast<PointerType *>(containerType);
        elementSize = t->GetUnderlyingType()->GetSize();
    }
    else
    {
        ArrayType *t = dynamic_cast<ArrayType *>(containerType);
        elementSize = t->GetElementType()->GetSize();
    }

    DisaInstruction *instOffset = new DisaInstruction();
    instOffset->OpCode = DisaInstruction::MUL;
    instOffset->Attribute = DisaInstruction::Signed64;
    GenerateDisaOperand(il->Operands[2], instOffset, 0);
    instOffset->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, elementSize, DisaInstruction::MovSigned64);
    instOffset->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, DisaInstruction::MovSigned64);
    Emit(instOffset);


    DisaInstruction *instAddr = new DisaInstruction();
    instAddr->OpCode = DisaInstruction::ADD;
    instAddr->Attribute = DisaInstruction::Signed64;
    GenerateDisaOperand(il->Operands[1], instAddr, 0);
    instAddr->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, DisaInstruction::MovSigned64);
    instAddr->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_2, DisaInstruction::MovSigned64);
    Emit(instAddr);

    DisaInstruction *instLoad = new DisaInstruction();
    instLoad->OpCode = DisaInstruction::MOV;
    
    GenerateDisaOperand(il->Operands[0], instLoad, 1);
    // instLoad->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Indirect, GENERAL_REG_2, DisaInstruction::MovSigned64);
    instLoad->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Indirect, GENERAL_REG_2, 
                                                         GetMovOperandAttribute(il->Operands[0].OperandType));
    Emit(instLoad);

}

void DisaCodeGenerator::VisitLdelema(IL* il)
{
    assert(il->Operands[1].OperandKind == IL::Variable);
    Symbol *sym = il->Operands[1].SymRef->Lookup();

    int64_t elementSize;
    Type *containerType = sym->DeclType;
    if(typeid(*containerType) == typeid(PointerType))
    {
        PointerType *t = dynamic_cast<PointerType *>(containerType);
        elementSize = t->GetUnderlyingType()->GetSize();
    }
    else
    {
        ArrayType *t = dynamic_cast<ArrayType *>(containerType);
        elementSize = t->GetElementType()->GetSize();
    }

    DisaInstruction *instOffset = new DisaInstruction();
    instOffset->OpCode = DisaInstruction::MUL;
    instOffset->Attribute = DisaInstruction::Signed64;
    GenerateDisaOperand(il->Operands[2], instOffset, 0);
    instOffset->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, elementSize, DisaInstruction::MovSigned64);
    instOffset->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, DisaInstruction::MovSigned64);
    Emit(instOffset);


    DisaInstruction *instAddr = new DisaInstruction();
    instAddr->OpCode = DisaInstruction::ADD;
    instAddr->Attribute = DisaInstruction::Signed64;
    GenerateDisaOperand(il->Operands[1], instAddr, 0);
    instAddr->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, DisaInstruction::MovSigned64);
    GenerateDisaOperand(il->Operands[0], instAddr, 2);
    Emit(instAddr);
}

void DisaCodeGenerator::VisitLdfld(IL* il)
{
    // TODO: used by struct
}

void DisaCodeGenerator::VisitLdflda(IL* il)
{
    // TODO: used by struct
}

void DisaCodeGenerator::VisitLdind(IL* il)
{
    switch(il->Operands[1].OperandKind)
    {
        case IL::Constant:
        {
            DisaInstruction *inst = new DisaInstruction();
            inst->OpCode = DisaInstruction::MOV;
            inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, il->Operands[1].IValue, GetMovOperandAttribute(il->Operands[1].OperandType));
            GenerateDisaOperand(il->Operands[0], inst, 1);
            inst->Operands[1].MovAttribute = GetMovOperandAttribute(il->Operands[1].OperandType);
            Emit(inst);
            break;
        }
        case IL::Variable:
        {
            Symbol *sym = il->Operands[1].SymRef->Lookup();
            assert(sym != NULL);

            if(sym->Scope->GetScopeKind() == SymbolScope::Global)
            {
                DisaInstruction *instAddr = new DisaInstruction();
                instAddr->OpCode = DisaInstruction::MOV;
                instAddr->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Indirect, 0xFFFFFFFFDEADBEEF, GetMovOperandAttribute(il->Operands[1].OperandType));
                GenerateDisaOperand(il->Operands[0], instAddr, 1);
                instAddr->Operands[1].MovAttribute = GetMovOperandAttribute(il->Operands[1].OperandType);
                Emit(instAddr);

//                 if(sym->Address == 0)
//                 {
                instAddr->Operands[0].SymRef = new SymbolRef(sym);
//                 }
//                 else
//                 {
//                     instAddr->Operands[0].SymRef = NULL;
//                 }
            }
            else
            {
                DisaInstruction *instBase = new DisaInstruction();
                instBase->OpCode = DisaInstruction::MOV;
                instBase->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::BaseDisplacement, FRAME_POINTER, sym->Address, DisaInstruction::MovSigned64);
                instBase->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, DisaInstruction::MovSigned64);
                Emit(instBase);

                DisaInstruction *instAddr = new DisaInstruction();
                instAddr->OpCode = DisaInstruction::MOV;
                instAddr->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Indirect, GENERAL_REG_1, GetMovOperandAttribute(il->Operands[1].OperandType));
                GenerateDisaOperand(il->Operands[0], instAddr, 1);
                instAddr->Operands[1].MovAttribute = GetMovOperandAttribute(il->Operands[1].OperandType);
                Emit(instAddr);
            }
            break;
        }
        default:
            abort();
    }
}

void DisaCodeGenerator::VisitLdstr(IL* il)
{
    assert(!"Not implemented.");
}

void DisaCodeGenerator::VisitStelem(IL* il)
{
    assert(il->Operands[0].OperandKind == IL::Variable);
    Symbol *sym = il->Operands[0].SymRef->Lookup();

    int64_t elementSize;
    Type *containerType = sym->DeclType;
    if(typeid(*containerType) == typeid(PointerType))
    {
        PointerType *t = dynamic_cast<PointerType *>(containerType);
        elementSize = t->GetUnderlyingType()->GetSize();
    }
    else
    {
        ArrayType *t = dynamic_cast<ArrayType *>(containerType);
        elementSize = t->GetElementType()->GetSize();
    }

    DisaInstruction *instOffset = new DisaInstruction();
    instOffset->OpCode = DisaInstruction::MUL;
    instOffset->Attribute = DisaInstruction::Signed64;
    GenerateDisaOperand(il->Operands[1], instOffset, 0);
    instOffset->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, elementSize, DisaInstruction::MovSigned64);
    instOffset->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, DisaInstruction::MovSigned64);
    Emit(instOffset);


    DisaInstruction *instAddr = new DisaInstruction();
    instAddr->OpCode = DisaInstruction::ADD;
    instAddr->Attribute = DisaInstruction::Signed64;
    GenerateDisaOperand(il->Operands[0], instAddr, 0);
    instAddr->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, DisaInstruction::MovSigned64);
    instAddr->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_2, DisaInstruction::MovSigned64);
    Emit(instAddr);

    DisaInstruction *instStore = new DisaInstruction();
    instStore->OpCode = DisaInstruction::MOV;
    GenerateDisaOperand(il->Operands[2], instStore, 0);
    instStore->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Indirect, GENERAL_REG_2, instStore->Operands[0].MovAttribute);
    Emit(instStore);
}

void DisaCodeGenerator::VisitStfld(IL* il)
{
    // TODO: used by struct
}

void DisaCodeGenerator::VisitStind(IL* il)
{
    switch(il->Operands[0].OperandKind)
    {
        case IL::Constant:
        {
            DisaInstruction *inst = new DisaInstruction();
            inst->OpCode = DisaInstruction::MOV;
            GenerateDisaOperand(il->Operands[1], inst, 0);
            inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, il->Operands[0].IValue, GetMovOperandAttribute(il->Operands[1].OperandType));
            Emit(inst);
            break;
        }
        case IL::Variable:
        {
            Symbol *sym = il->Operands[0].SymRef->Lookup();
            assert(sym != NULL);

            if(sym->Scope->GetScopeKind() == SymbolScope::Global)
            {
                DisaInstruction *instAddr = new DisaInstruction();
                instAddr->OpCode = DisaInstruction::MOV;
                GenerateDisaOperand(il->Operands[1], instAddr, 0);
                instAddr->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Indirect, 0xFFFFFFFFDEADBEEF, GetMovOperandAttribute(il->Operands[1].OperandType));

//                 if(sym->Address == 0)
//                 {
                instAddr->Operands[1].SymRef = new SymbolRef(sym);
//                 }
//                 else
//                 {
//                     instAddr->Operands[1].SymRef = NULL;
//                 }
                Emit(instAddr);
            }
            else
            {
                DisaInstruction *instBase = new DisaInstruction();
                instBase->OpCode = DisaInstruction::MOV;
                instBase->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::BaseDisplacement, FRAME_POINTER, sym->Address, DisaInstruction::MovSigned64);
                instBase->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, DisaInstruction::MovSigned64);
                Emit(instBase);

                DisaInstruction *instAddr = new DisaInstruction();
                instAddr->OpCode = DisaInstruction::MOV;
                GenerateDisaOperand(il->Operands[1], instAddr, 0);
                instAddr->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Indirect, GENERAL_REG_1, GetMovOperandAttribute(il->Operands[1].OperandType));
                Emit(instAddr);
            }
            break;
        }
        default:
            abort();
    }
}

void DisaCodeGenerator::VisitStackalloc(IL* il)
{
    assert(!"Not implemented.");
}

void DisaCodeGenerator::VisitMul(IL* il)
{
    GenerateBinaryArithmatic(il, DisaInstruction::MUL);
}

void DisaCodeGenerator::VisitNeg(IL* il)
{
    DisaInstruction *inst = new DisaInstruction();
    DisaInstruction::MovOperandAttribute movattr;
    inst->OpCode = DisaInstruction::SUB;

    bool needsIntermediates = false;
    switch(il->Operands[0].OperandType)
    {
        case IL::I1:
        case IL::I2:
        case IL::I4:
            needsIntermediates = true;
        case IL::I8:
        case IL::Void:
        case IL::I:
        case IL::Str:
            inst->Attribute = DisaInstruction::Signed64;
            movattr = DisaInstruction::MovSigned64;
            break;
        case IL::U1:
        case IL::U2:
        case IL::U4:
            needsIntermediates = true;
        case IL::U8:
        case IL::U:
            inst->Attribute = DisaInstruction::Unsigned64;
            movattr = DisaInstruction::MovUnsigned64;
            break;
        case IL::R4:
            inst->Attribute = DisaInstruction::Single;
            movattr = DisaInstruction::MovSingle;
            break;
        case IL::R8:
            inst->Attribute = DisaInstruction::Double;
            movattr = DisaInstruction::MovDouble;
            break;
        default:
            abort();
    }

    if(needsIntermediates)
    {
        DisaInstruction *loadInst2 = new DisaInstruction();
        loadInst2->OpCode = DisaInstruction::MOV;
        GenerateDisaOperand(il->Operands[2], loadInst2, 0);
        loadInst2->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, movattr);

        inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, (int64_t)0, movattr);
        inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, movattr);
        inst->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, movattr);

        DisaInstruction *saveInst = new DisaInstruction();
        saveInst->OpCode = DisaInstruction::MOV;
        saveInst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, GENERAL_REG_1, movattr);
        GenerateDisaOperand(il->Operands[0], saveInst, 1);

        Emit(loadInst2);
        Emit(inst);
        Emit(saveInst);
    }
    else
    {
        inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, (int64_t)0, movattr);
        GenerateDisaOperand(il->Operands[1], inst, 1);
        GenerateDisaOperand(il->Operands[0], inst, 2);
        Emit(inst);
    }
}

void DisaCodeGenerator::VisitNewarr(IL* il)
{
    assert(!"Not implemented.");

}

void DisaCodeGenerator::VisitNewobj(IL* il)
{
    assert(!"Not implemented.");

}

void DisaCodeGenerator::VisitNot(IL* il)
{
    assert(!"Not implemented.");
}

void DisaCodeGenerator::VisitOr(IL* il)
{
    GenerateBinaryArithmatic(il, DisaInstruction::OR);
}

void DisaCodeGenerator::VisitMod(IL* il)
{
    assert(!"Not implemented.");

}

void DisaCodeGenerator::VisitRet(IL* il)
{
    //NOTE: Step 1. store the return value, if exists
    if(il->Operands.size() > 0 && il->Operands[0].OperandKind != IL::Empty)
    {
        DisaInstruction *stInst = new DisaInstruction();
        stInst->OpCode = DisaInstruction::MOV;
        GenerateDisaOperand(il->Operands[0], stInst, 0);
        stInst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::BaseDisplacement, FRAME_POINTER, PARAMETER_OFFSET, stInst->Operands[0].MovAttribute);
        Emit(stInst);
    }


    /* NOTE: Step 2. Leave the stack frame, after that the return address is on the stack top
     * Original:
     *     mov %rbp, %rsp
     *     pop %rbp
     */

    DisaInstruction *inst;

    inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::MOV;
    inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, FRAME_POINTER, DisaInstruction::MovSigned64);
    inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, STACK_POINTER, DisaInstruction::MovSigned64);
    inst->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Indirect, STACK_POINTER, DisaInstruction::MovSigned64);
    Emit(inst);

    inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::MOV;
    inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Indirect, STACK_POINTER, DisaInstruction::MovSigned64);
    inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Direct, FRAME_POINTER, DisaInstruction::MovSigned64);
    Emit(inst);

    inst = new DisaInstruction();
    inst->OpCode = DisaInstruction::ADD;
    inst->Attribute = DisaInstruction::Signed64;
    inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, STACK_POINTER, DisaInstruction::MovSigned64);
    inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, (int64_t)(PointerType::PointerSize), DisaInstruction::MovSigned64);
    inst->Operands[2] = DisaInstruction::DisaOperand(DisaInstruction::Direct, STACK_POINTER, DisaInstruction::MovSigned64);
    Emit(inst);

    // NOTE: Step 3, jump to the caller
    DisaInstruction *brInst = new DisaInstruction();
    brInst->OpCode = DisaInstruction::BR;
    brInst->JumpMode = DisaInstruction::JI;
    brInst->RelativeJump = false;
    brInst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Indirect, STACK_POINTER, DisaInstruction::MovSigned64);

    Emit(brInst);
}

void DisaCodeGenerator::VisitShl(IL* il)
{
    assert(!"Not implemented.");

}

void DisaCodeGenerator::VisitShr(IL* il)
{
    assert(!"Not implemented.");
}

void DisaCodeGenerator::VisitSub(IL* il)
{
    GenerateBinaryArithmatic(il, DisaInstruction::SUB);
}

void DisaCodeGenerator::VisitMov(IL* il)
{
    VisitConv(il);
}

void DisaCodeGenerator::VisitXor(IL* il)
{
    GenerateBinaryArithmatic(il, DisaInstruction::XOR);
}

void DisaCodeGenerator::VisitAsm(IL* il)
{
    std::string asmLines = il->Operands[0].SValue;
    while(asmLines.size() > 0)
    {
        int pos = asmLines.find('\n');
        if(pos == -1)
        {
            Emit(asmLines);
            asmLines = "";
        }
        else
        {
            Emit(asmLines.substr(0, pos));
            asmLines = asmLines.substr(pos + 1);
        }
    }
}

void DisaCodeGenerator::VisitLabel(IL* il)
{
    assert(il->Operands[0].OperandKind == IL::Variable);
    Symbol *symbol = il->Operands[0].SymRef->Lookup();
    assert(symbol != NULL);
    symbol->Address = _currentText;
}

void DisaCodeGenerator::VisitLda(IL* il)
{
    assert(il->Operands[1].OperandKind == IL::Variable);

    Symbol *symbol = il->Operands[1].SymRef->Lookup();
    assert(symbol != NULL);

    if(symbol->Scope->GetScopeKind() == SymbolScope::Global || typeid(*(symbol->DeclType)) == typeid(FunctionType) || typeid(*(symbol->DeclType)) == typeid(LabelType))
    {
        // debug
        // printf("C\n");

        DisaInstruction *inst = new DisaInstruction();
        inst->OpCode = DisaInstruction::MOV;
        inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, 0xFFFFFFFFDEADBEEF, DisaInstruction::MovSigned64);
        inst->Operands[0].SymRef = new SymbolRef(symbol);
        GenerateDisaOperand(il->Operands[0], inst, 1);
        Emit(inst);
    }
    else
    {
        // debug
        // printf("D\n");

        DisaInstruction *inst = new DisaInstruction();
        inst->OpCode = DisaInstruction::ADD;
        inst->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Direct, FRAME_POINTER, DisaInstruction::MovSigned64);
        inst->Operands[1] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, symbol->Address, DisaInstruction::MovSigned64);
        GenerateDisaOperand(il->Operands[0], inst, 2);
        Emit(inst);
    }
}

DisaInstruction::MovOperandAttribute DisaCodeGenerator::GetMovOperandAttribute(IL::ILOperandType optype)
{
    DisaInstruction::MovOperandAttribute elementSize;
    switch(optype)
    {
        case IL::Void:
        case IL::I:
        case IL::I8:
            elementSize = DisaInstruction::MovSigned64;
            break;
        case IL::U:
        case IL::U8:
            elementSize = DisaInstruction::MovUnsigned64;
            break;
        case IL::I1:
            elementSize = DisaInstruction::MovSigned8;
            break;
        case IL::U1:
            elementSize = DisaInstruction::MovUnsigned8;
            break;
        case IL::I2:
            abort();
        case IL::U2:
            abort();
        case IL::I4:
            elementSize = DisaInstruction::MovSigned32;
            break;
        case IL::U4:
            elementSize = DisaInstruction::MovUnsigned32;
            break;
        case IL::R4:
            elementSize = DisaInstruction::MovSingle;
            break;
        case IL::R8:
            elementSize = DisaInstruction::MovDouble;
            break;
        case IL::Str:
            elementSize = DisaInstruction::MovSigned64;
            break;

        default:
            abort();
    }

    return elementSize;
}
int64_t DisaCodeGenerator::GetOperandSize(IL::ILOperandType optype, int64_t align)
{
    int64_t elementSize;
    switch(optype)
    {
        case IL::Void:
        case IL::I:
        case IL::U:
        case IL::I8:
        case IL::U8:
            elementSize = 8;
            break;
        case IL::I1:
        case IL::U1:
            elementSize = 1;
            break;
        case IL::I2:
        case IL::U2:
            elementSize = 2;
            break;
        case IL::I4:
            elementSize = 4;
            break;
        case IL::R4:
            elementSize = 4;
            break;
        case IL::R8:
            elementSize = 8;
            break;
        case IL::Str:
            elementSize = 8;
            break;
        default:
            abort();
    }

    elementSize = ROUND_UP(elementSize, align);
    return elementSize;
}




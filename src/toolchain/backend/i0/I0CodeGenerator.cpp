#include "I0CodeGenerator.h"
#include "I0Instruction.h"

#include <core/Core.h>
#include <core/Symbol/SymbolAddressAllocator.h>

#include <assert.h>
#include <iostream>
#include <cstdio>

I0CodeGenerator::I0CodeGenerator()
{
}

I0CodeGenerator::~I0CodeGenerator()
{

}

void I0CodeGenerator::EnterProgram(ILProgram* program)
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
    GenerateSpawn0();
}

void I0CodeGenerator::LeaveProgram(ILProgram* program)
{
    CodeGenerator::LeaveProgram(program);
    _staticChain.pop();

    ResolveSymbols();
}

void I0CodeGenerator::EnterClass(ILClass* ilclass)
{
    CodeGenerator::EnterClass(ilclass);
}

void I0CodeGenerator::LeaveClass(ILClass* ilclass)
{
    CodeGenerator::LeaveClass(ilclass);
}

void I0CodeGenerator::EnterFunction(ILFunction* ilfunc)
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

    I0Instruction *inst;

    inst = new I0Instruction();
    inst->OpCode = I0Instruction::CONV;
    inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, FRAME_POINTER, I0Instruction::ConvSigned64);
    inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::BaseDisplacement, STACK_POINTER, (-PointerType::PointerSize), I0Instruction::ConvSigned64);
    Emit(inst);

    inst = new I0Instruction();
    inst->OpCode = I0Instruction::SUB;
    inst->Attribute = I0Instruction::Signed64;
    inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, STACK_POINTER, I0Instruction::ConvSigned64);
    inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Immediate, (int64_t)(PointerType::PointerSize), I0Instruction::ConvSigned64);
    inst->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, FRAME_POINTER, I0Instruction::ConvSigned64);
    Emit(inst);

    inst = new I0Instruction();
    inst->OpCode = I0Instruction::SUB;
    inst->Attribute = I0Instruction::Signed64;
    inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, STACK_POINTER, I0Instruction::ConvSigned64);
    inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Immediate, (int64_t)(ilfunc->Scope->GetMemorySize() + PointerType::PointerSize), I0Instruction::ConvSigned64);
    inst->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, STACK_POINTER, I0Instruction::ConvSigned64);
    Emit(inst);

}

void I0CodeGenerator::LeaveFunction(ILFunction* ilfunc)
{
    CodeGenerator::LeaveFunction(ilfunc);
    _staticChain.pop();
}

void I0CodeGenerator::ResolveSymbols()
{
    TargetProgram *program = CompilationContext::GetInstance()->Target;
    for(std::vector<TargetInstruction *>::iterator it = program->Code.begin(); it != program->Code.end(); ++it)
    {
        I0Instruction *inst = dynamic_cast<I0Instruction *>(*it);
        for(int i = 0; i < I0Instruction::MAX_NUM_OPERANDS; i++)
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
                if(CompilationContext::GetInstance()->Debug)
                    std::cout << "Resolving symbol \'" << symbol->Name << "\' -> 0x" << std::uppercase << std::hex << symbol->Address << std::endl;
                inst->Operands[i].SymRef = NULL;
            }
        }
    }
}

void I0CodeGenerator::GenerateCrt0()
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

    I0Instruction *inst;

    inst = new I0Instruction();
    inst->OpCode = I0Instruction::ADD;
    inst->Attribute = I0Instruction::Signed64;
    inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, CURRENT_STACK_BASE, I0Instruction::ConvSigned64);
    inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, CURRENT_STACK_SIZE, I0Instruction::ConvSigned64);
    inst->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, STACK_POINTER, I0Instruction::ConvSigned64);
    Emit(inst);

    inst = new I0Instruction();
    inst->OpCode = I0Instruction::CONV;
    inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Immediate, (int64_t)0, I0Instruction::ConvSigned64);
    inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, FRAME_POINTER, I0Instruction::ConvSigned64);
    Emit(inst);

    Symbol *mainSym = SymbolScope::GetRootScope()->Lookup("main");
    if(mainSym == NULL)
    {
        CompilationContext::GetInstance()->ReportError(Location(), false, "Cannot find \'main\' function.");
    }

    inst = new I0Instruction();
    inst->OpCode = I0Instruction::B;
    inst->JumpMode = I0Instruction::J;
    inst->RelativeJump = false;
    inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Immediate, 0xFFFFFFFFDEADBEEF, I0Instruction::ConvSigned64);
//     if (mainSym->Address == 0)
//     {
    inst->Operands[0].SymRef = new SymbolRef(SymbolScope::GetRootScope(), "main");
//     }
    Emit(inst);

}

void I0CodeGenerator::GenerateSpawn0()
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

    I0Instruction *inst;

    inst = new I0Instruction();
    inst->OpCode = I0Instruction::ADD;
    inst->Attribute = I0Instruction::Signed64;
    inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, CURRENT_STACK_BASE, I0Instruction::ConvSigned64);
    inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, CURRENT_STACK_SIZE, I0Instruction::ConvSigned64);
    inst->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, I0Instruction::ConvSigned64);
    Emit(inst);

    inst = new I0Instruction();
    inst->OpCode = I0Instruction::CONV;
    inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Immediate, (int64_t)0, I0Instruction::ConvSigned64);
    inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, FRAME_POINTER, I0Instruction::ConvSigned64);
    Emit(inst);

    inst = new I0Instruction();
    inst->OpCode = I0Instruction::SUB;
    inst->Attribute = I0Instruction::Signed64;
    inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, I0Instruction::ConvSigned64);
    inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Immediate, (int64_t)(PointerType::PointerSize * 3), I0Instruction::ConvSigned64);
    inst->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, STACK_POINTER, I0Instruction::ConvSigned64);
    Emit(inst);

    inst = new I0Instruction();
    inst->OpCode = I0Instruction::SUB;
    inst->Attribute = I0Instruction::Signed64;
    inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, STACK_POINTER, I0Instruction::ConvSigned64);
    inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::BaseDisplacement, GENERAL_REG_1, (int64_t)(- PointerType::PointerSize * 2), I0Instruction::ConvSigned64);
    inst->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, STACK_POINTER, I0Instruction::ConvSigned64);
    Emit(inst);

    inst = new I0Instruction();
    inst->OpCode = I0Instruction::CONV;
    inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::BaseDisplacement, GENERAL_REG_1, (int64_t)(- PointerType::PointerSize), I0Instruction::ConvSigned64);
    inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_2, I0Instruction::ConvSigned64);
    Emit(inst);

    inst = new I0Instruction();
    inst->OpCode = I0Instruction::CONV;
    inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Immediate, (int64_t)0, I0Instruction::ConvSigned64);
    inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Indirect, STACK_POINTER, I0Instruction::ConvSigned64);
    Emit(inst);

    int64_t *pRetAddr = &inst->Operands[0].Address;

    inst = new I0Instruction();
    inst->OpCode = I0Instruction::B;
    inst->JumpMode = I0Instruction::JI;
    inst->RelativeJump = false;
    inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_2, I0Instruction::ConvSigned64);
    Emit(inst);

    *pRetAddr = _currentText;

    Emit("exit:c");
}


void I0CodeGenerator::GenerateI0Operand(IL::ILOperand ilOperand, I0Instruction* inst, int operandIndex)
{
    switch(ilOperand.OperandKind)
    {
        case IL::Constant:
            inst->Operands[operandIndex].AddressingMode = I0Instruction::Immediate;
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
                    inst->Operands[operandIndex].AddressingMode = I0Instruction::Immediate;
                }
                else
                {
                    inst->Operands[operandIndex].AddressingMode = I0Instruction::Direct;
                }

                inst->Operands[operandIndex].Address = 0xFFFFFFFFDEADBEEF;
                inst->Operands[operandIndex].SymRef = new SymbolRef(symbol);

            }
            else
            {
                if(typeid(*(symbol->DeclType)) == typeid(ArrayType))
                {
                    I0Instruction *addrInst = new I0Instruction();
                    addrInst->OpCode = I0Instruction::ADD;
                    addrInst->Attribute = I0Instruction::Signed64;
                    addrInst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, FRAME_POINTER, I0Instruction::ConvSigned64);
                    addrInst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Immediate, symbol->Address, I0Instruction::ConvSigned64);
                    addrInst->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_2, I0Instruction::ConvSigned64);
                    Emit(addrInst);
                    inst->Operands[operandIndex].AddressingMode = I0Instruction::Direct;
                    inst->Operands[operandIndex].Address = GENERAL_REG_2;
                }
                else
                {
                    inst->Operands[operandIndex].AddressingMode = I0Instruction::BaseDisplacement;
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

    inst->Operands[operandIndex].ConvAttribute = GetConvOperandAttribute(ilOperand.OperandType);
}

void I0CodeGenerator::Emit(I0Instruction* inst)
{
    // debug
    // printf("U\n");
    CompilationContext::GetInstance()->Target->Code.push_back(inst);
    // debug
    // printf("O\n");
    _currentText += inst->GetLength();
    // debug
    // printf("P\n");
}

void I0CodeGenerator::Emit(std::string str)
{
    I0Instruction *inst = new I0Instruction(_staticChain.top(), str);
    Emit(inst);
}


void I0CodeGenerator::GenerateBinaryArithmatic(IL* il, I0Instruction::I0Opcode opcode)
{
    I0Instruction *inst = new I0Instruction();
    I0Instruction::ConvOperandAttribute convattr;
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
            inst->Attribute = I0Instruction::Signed64;
            convattr = I0Instruction::ConvSigned64;
            break;
        case IL::U1:
        case IL::U2:
        case IL::U4:
            needsIntermediates = true;
        case IL::U8:
        case IL::U:
            inst->Attribute = I0Instruction::Unsigned64;
            convattr = I0Instruction::ConvUnsigned64;
            break;
        case IL::R4:
            inst->Attribute = I0Instruction::Single;
            convattr = I0Instruction::ConvSingle;
            break;
        case IL::R8:
            inst->Attribute = I0Instruction::Double;
            convattr = I0Instruction::ConvDouble;
            break;
        default:
            abort();
    }

    if(needsIntermediates)
    {
        I0Instruction *loadInst1 = new I0Instruction();
        loadInst1->OpCode = I0Instruction::CONV;
        GenerateI0Operand(il->Operands[1], loadInst1, 0);
        loadInst1->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, convattr);

        I0Instruction *loadInst2 = new I0Instruction();
        loadInst2->OpCode = I0Instruction::CONV;
        GenerateI0Operand(il->Operands[2], loadInst2, 0);
        loadInst2->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_2, convattr);

        inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, convattr);
        inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_2, convattr);
        inst->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, convattr);

        I0Instruction *saveInst = new I0Instruction();
        saveInst->OpCode = I0Instruction::CONV;
        saveInst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, convattr);
        GenerateI0Operand(il->Operands[0], saveInst, 1);

        Emit(loadInst1);
        Emit(loadInst2);
        Emit(inst);
        Emit(saveInst);
    }
    else
    {
        GenerateI0Operand(il->Operands[0], inst, 2);
        GenerateI0Operand(il->Operands[1], inst, 0);
        GenerateI0Operand(il->Operands[2], inst, 1);
        Emit(inst);
    }
}

void I0CodeGenerator::GenerateComparason(IL* il, I0Instruction::BJumpMode mode, bool brValue)
{
    /*
     * [0]     br:xxx TL
     * [1] FL: mov 0:msq, target:msq
     * [2]     br:j EL
     * [3] TL: mov 1:msq, target:msq
     * [4] EL:
     */

    I0Instruction *inst = new I0Instruction();

    inst->OpCode = I0Instruction::B;
    inst->JumpMode = mode;
    inst->RelativeJump = false;

    bool needsIntermediates = false;
    I0Instruction::ConvOperandAttribute convattr;

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
            inst->Attribute = I0Instruction::Signed64;
            convattr = I0Instruction::ConvSigned64;
            break;
        case IL::U1:
        case IL::U2:
        case IL::U4:
            needsIntermediates = true;
        case IL::U8:
        case IL::U:
            inst->Attribute = I0Instruction::Unsigned64;
            convattr = I0Instruction::ConvUnsigned64;
            break;
        case IL::R4:
            inst->Attribute = I0Instruction::Single;
            convattr = I0Instruction::ConvSingle;
            break;
        case IL::R8:
            inst->Attribute = I0Instruction::Double;
            convattr = I0Instruction::ConvDouble;
            break;
        default:
            abort();
    }

    int64_t *pTL, *pEL;

    if(needsIntermediates)
    {
        I0Instruction *loadInst1 = new I0Instruction();
        loadInst1->OpCode = I0Instruction::CONV;
        GenerateI0Operand(il->Operands[1], loadInst1, 0);
        loadInst1->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, convattr);

        I0Instruction *loadInst2 = new I0Instruction();
        loadInst2->OpCode = I0Instruction::CONV;
        GenerateI0Operand(il->Operands[2], loadInst2, 0);
        loadInst2->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_2, convattr);

        inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, convattr);
        inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_2, convattr);
        inst->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, (int64_t)0, convattr);

        Emit(loadInst1);
        Emit(loadInst2);
        Emit(inst);

        pTL = &(inst->Operands[2].Address);
    }
    else
    {
        GenerateI0Operand(il->Operands[1], inst, 0);
        GenerateI0Operand(il->Operands[2], inst, 1);
        inst->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, (int64_t)0, convattr);
        Emit(inst);
        pTL = &(inst->Operands[2].Address);
    }


    I0Instruction *finst = new I0Instruction();
    finst->OpCode = I0Instruction::CONV;
    finst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Immediate, (int64_t)(brValue ? 0 : 1), I0Instruction::ConvSigned64);
    GenerateI0Operand(il->Operands[0], finst, 1);
    Emit(finst);


    I0Instruction *einst = new I0Instruction();
    einst->OpCode = I0Instruction::B;
    einst->JumpMode = I0Instruction::J;
    einst->RelativeJump = false;
    einst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, (int64_t)0, convattr);

    Emit(einst);
    pEL = &(einst->Operands[0].Address);

    *pTL = _currentText;

    I0Instruction *tinst = new I0Instruction();
    tinst->OpCode = I0Instruction::CONV;
    tinst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Immediate, (int64_t)(brValue ? 1 : 0), I0Instruction::ConvSigned64);
    GenerateI0Operand(il->Operands[0], tinst, 1);
    Emit(tinst);

    *pEL = _currentText;

}

void I0CodeGenerator::GenerateCompareBranch(IL* il, I0Instruction::BJumpMode mode, bool brValue)
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

    I0Instruction *inst = new I0Instruction();

    inst->OpCode = I0Instruction::B;
    inst->JumpMode = mode;
    inst->RelativeJump = false;

    bool needsIntermediates = false;
    I0Instruction::ConvOperandAttribute convattr;

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
            inst->Attribute = I0Instruction::Signed64;
            convattr = I0Instruction::ConvSigned64;
            break;
        case IL::U1:
        case IL::U2:
        case IL::U4:
            needsIntermediates = true;
        case IL::U8:
        case IL::U:
            inst->Attribute = I0Instruction::Unsigned64;
            convattr = I0Instruction::ConvUnsigned64;
            break;
        case IL::R4:
            inst->Attribute = I0Instruction::Single;
            convattr = I0Instruction::ConvSingle;
            break;
        case IL::R8:
            inst->Attribute = I0Instruction::Double;
            convattr = I0Instruction::ConvDouble;
            break;
        default:
            abort();
    }

    int64_t *pFL;
    if(brValue)
    {
        if(needsIntermediates)
        {
            I0Instruction *loadInst1 = new I0Instruction();
            loadInst1->OpCode = I0Instruction::CONV;
            GenerateI0Operand(il->Operands[0], loadInst1, 0);
            loadInst1->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, convattr);

            I0Instruction *loadInst2 = new I0Instruction();
            loadInst2->OpCode = I0Instruction::CONV;
            GenerateI0Operand(il->Operands[1], loadInst2, 0);
            loadInst2->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_2, convattr);

            inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, convattr);
            inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_2, convattr);
            GenerateI0Operand(il->Operands[2], inst, 2);

            Emit(loadInst1);
            Emit(loadInst2);
            Emit(inst);
        }
        else
        {
            GenerateI0Operand(il->Operands[0], inst, 0);
            GenerateI0Operand(il->Operands[1], inst, 1);
            GenerateI0Operand(il->Operands[2], inst, 2);
            Emit(inst);
        }
    }
    else
    {
        if(needsIntermediates)
        {
            I0Instruction *loadInst1 = new I0Instruction();
            loadInst1->OpCode = I0Instruction::CONV;
            GenerateI0Operand(il->Operands[0], loadInst1, 0);
            loadInst1->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, convattr);

            I0Instruction *loadInst2 = new I0Instruction();
            loadInst2->OpCode = I0Instruction::CONV;
            GenerateI0Operand(il->Operands[1], loadInst2, 0);
            loadInst2->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_2, convattr);

            inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, convattr);
            inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_2, convattr);
            inst->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, (int64_t)0, convattr);

            Emit(loadInst1);
            Emit(loadInst2);
            Emit(inst);

            pFL = &(inst->Operands[2].Address);
        }
        else
        {
            GenerateI0Operand(il->Operands[0], inst, 0);
            GenerateI0Operand(il->Operands[1], inst, 1);
            GenerateI0Operand(il->Operands[2], inst, 2);
            Emit(inst);
            pFL = &(inst->Operands[2].Address);
        }

        I0Instruction *binst = new I0Instruction();
        binst->OpCode = I0Instruction::B;
        binst->JumpMode = I0Instruction::J;
        binst->RelativeJump = false;
        GenerateI0Operand(il->Operands[2], binst, 0);

        Emit(inst);

        *pFL = _currentText;
    }
}

void I0CodeGenerator::GenerateConditionalBr(IL* il, I0Instruction::BJumpMode mode)
{
    I0Instruction *inst = new I0Instruction();

    inst->OpCode = I0Instruction::B;
    inst->JumpMode = mode;
    inst->RelativeJump = false;

    bool needsIntermediates = false;
    I0Instruction::ConvOperandAttribute convattr;

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
            inst->Attribute = I0Instruction::Signed64;
            convattr = I0Instruction::ConvSigned64;
            break;
        case IL::U1:
        case IL::U2:
        case IL::U4:
            needsIntermediates = true;
        case IL::U8:
        case IL::U:
            inst->Attribute = I0Instruction::Unsigned64;
            convattr = I0Instruction::ConvUnsigned64;
            break;
        case IL::R4:
            inst->Attribute = I0Instruction::Single;
            convattr = I0Instruction::ConvSingle;
            break;
        case IL::R8:
            inst->Attribute = I0Instruction::Double;
            convattr = I0Instruction::ConvDouble;
            break;
        default:
            abort();
    }


    if(needsIntermediates)
    {
        I0Instruction *loadInst1 = new I0Instruction();
        loadInst1->OpCode = I0Instruction::CONV;
        GenerateI0Operand(il->Operands[0], loadInst1, 0);
        loadInst1->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, convattr);

        inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, convattr);
        GenerateI0Operand(il->Operands[1], inst, 1);

        Emit(loadInst1);
        Emit(inst);
    }
    else
    {
        GenerateI0Operand(il->Operands[0], inst, 0);
        GenerateI0Operand(il->Operands[1], inst, 1);
        Emit(inst);
    }

}


void I0CodeGenerator::VisitNop(IL* il)
{
    Emit("nop");
}

void I0CodeGenerator::VisitAdd(IL* il)
{
    GenerateBinaryArithmatic(il, I0Instruction::ADD);
}

void I0CodeGenerator::VisitAnd(IL* il)
{
    GenerateBinaryArithmatic(il, I0Instruction::AND);
}

void I0CodeGenerator::VisitBrEq(IL* il)
{
    GenerateCompareBranch(il, I0Instruction::E, true);
}

void I0CodeGenerator::VisitBrGt(IL* il)
{
    GenerateCompareBranch(il, I0Instruction::LE, false);
}

void I0CodeGenerator::VisitBrGe(IL* il)
{
    GenerateCompareBranch(il, I0Instruction::L, false);
}

void I0CodeGenerator::VisitBrLt(IL* il)
{
    GenerateCompareBranch(il, I0Instruction::L, true);

}

void I0CodeGenerator::VisitBrLe(IL* il)
{
    GenerateCompareBranch(il, I0Instruction::LE, true);
}

void I0CodeGenerator::VisitBrNe(IL* il)
{
    GenerateCompareBranch(il, I0Instruction::NE, true);
}

void I0CodeGenerator::VisitBr(IL* il)
{
    I0Instruction *inst = new I0Instruction();
    inst->OpCode = I0Instruction::B;
    inst->JumpMode = I0Instruction::J;
    inst->RelativeJump = false;
    GenerateI0Operand(il->Operands[0], inst, 0);
    Emit(inst);
}

void I0CodeGenerator::VisitBrZ(IL* il)
{
    GenerateConditionalBr(il, I0Instruction::Z);
}

void I0CodeGenerator::VisitBrNz(IL* il)
{
    GenerateConditionalBr(il, I0Instruction::NZ);
}

void I0CodeGenerator::VisitCall(IL* il)
{
    int64_t pushSize = 0;
    int64_t retSize = 0;

    for(size_t i = 2; i < il->Operands.size(); i++)
    {
        pushSize += GetOperandSize(il->Operands[i].OperandType, PointerType::PointerSize);
    }

    if(il->Operands[1].OperandKind != IL::Empty)
    {
        retSize = GetOperandSize(il->Operands[1].OperandType, PointerType::PointerSize);
        pushSize += retSize;
    }

    pushSize += PointerType::PointerSize;       // For the return address

    I0Instruction *allocInst = new I0Instruction();
    allocInst->OpCode = I0Instruction::SUB;
    allocInst->Attribute = I0Instruction::Signed64;
    allocInst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, STACK_POINTER, I0Instruction::ConvSigned64);
    allocInst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Immediate, (int64_t)pushSize, I0Instruction::ConvSigned64);
    allocInst->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, STACK_POINTER, I0Instruction::ConvSigned64);
    Emit(allocInst);

    int64_t stackOffset = 0; //retSiz;  PointerType::PointerSize;

    I0Instruction *retAddrInst = new I0Instruction();
    retAddrInst->OpCode = I0Instruction::CONV;
    retAddrInst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Immediate, (int64_t)0, I0Instruction::ConvSigned64);
    retAddrInst->Operands[1] = I0Instruction::I0Operand(I0Instruction::BaseDisplacement, STACK_POINTER, stackOffset, I0Instruction::ConvSigned64);
    Emit(retAddrInst);

    stackOffset += PointerType::PointerSize;


    int64_t *pRetAddr = &(retAddrInst->Operands[0].IntValue);


    // Allocate space for return value
    stackOffset += retSize;

    // Push parameters
    for(size_t i = 2; i < il->Operands.size(); i++)
    {
        I0Instruction *pushInst = NULL;
        pushInst = new I0Instruction();
        pushInst->OpCode = I0Instruction::CONV;
        GenerateI0Operand(il->Operands[i], pushInst, 0);
        pushInst->Operands[1] = I0Instruction::I0Operand(I0Instruction::BaseDisplacement, STACK_POINTER, stackOffset, pushInst->Operands[0].ConvAttribute);
        Emit(pushInst);
        stackOffset += GetOperandSize(il->Operands[i].OperandType, PointerType::PointerSize);
    }



    I0Instruction *brInst = new I0Instruction();
    brInst->OpCode = I0Instruction::B;
    brInst->JumpMode = I0Instruction::J;
    brInst->RelativeJump = false;
    GenerateI0Operand(il->Operands[0], brInst, 0);
    Emit(brInst);

    *pRetAddr = _currentText;


    // Get the return value
    if (il->Operands[1].OperandKind != IL::Empty)
    {
        I0Instruction *getRetvInst = new I0Instruction();
        getRetvInst->OpCode = I0Instruction::CONV;
        getRetvInst->Operands[0] = I0Instruction::I0Operand(I0Instruction::BaseDisplacement, STACK_POINTER, PointerType::PointerSize, GetConvOperandAttribute(il->Operands[1].OperandType));
        GenerateI0Operand(il->Operands[1], getRetvInst, 1);
        Emit(getRetvInst);
    }

    I0Instruction *deallocInst = new I0Instruction();
    deallocInst->OpCode = I0Instruction::ADD;
    deallocInst->Attribute = I0Instruction::Signed64;
    deallocInst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, STACK_POINTER, I0Instruction::ConvSigned64);
    deallocInst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Immediate, (int64_t)pushSize, I0Instruction::ConvSigned64);
    deallocInst->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, STACK_POINTER, I0Instruction::ConvSigned64);
    Emit(deallocInst);
}

void I0CodeGenerator::VisitCallInd(IL* il)
{
    assert(!"Not supported yet: indirect call");
}

void I0CodeGenerator::VisitCallVirt(IL* il)
{
    assert(!"Not supported yet: virtual call");
}

void I0CodeGenerator::VisitCast(IL* il)
{
    assert(!"Not supported yet: casting");
}

void I0CodeGenerator::VisitCompEq(IL* il)
{
    GenerateComparason(il, I0Instruction::E, true);
}

void I0CodeGenerator::VisitCompGt(IL* il)
{
    GenerateComparason(il, I0Instruction::LE, false);
}

void I0CodeGenerator::VisitCompGe(IL* il)
{
    GenerateComparason(il, I0Instruction::L, false);
}

void I0CodeGenerator::VisitCompLt(IL* il)
{
    GenerateComparason(il, I0Instruction::L, true);
}

void I0CodeGenerator::VisitCompLe(IL* il)
{
    GenerateComparason(il, I0Instruction::LE, true);
}

void I0CodeGenerator::VisitCompNe(IL* il)
{
    GenerateComparason(il, I0Instruction::NE, true);
}

void I0CodeGenerator::VisitMov(IL* il)
{
    I0Instruction *inst = new I0Instruction();
    inst->OpCode = I0Instruction::CONV;
    GenerateI0Operand(il->Operands[1], inst, 0);
    GenerateI0Operand(il->Operands[0], inst, 1);
    Emit(inst);
}

void I0CodeGenerator::VisitCpblk(IL* il)
{
    assert(!"Not implemented.");
}

void I0CodeGenerator::VisitCpobj(IL* il)
{
    assert(!"Not implemented.");
}

void I0CodeGenerator::VisitDiv(IL* il)
{
    GenerateBinaryArithmatic(il, I0Instruction::DIV);
}

void I0CodeGenerator::VisitInitblk(IL* il)
{
    assert(!"Not implemented.");
}

void I0CodeGenerator::VisitInitobj(IL* il)
{
    assert(!"Not implemented.");
}

void I0CodeGenerator::VisitLdelem(IL* il)
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

    I0Instruction *instOffset = new I0Instruction();
    instOffset->OpCode = I0Instruction::MUL;
    instOffset->Attribute = I0Instruction::Signed64;
    GenerateI0Operand(il->Operands[2], instOffset, 0);
    instOffset->Operands[1] = I0Instruction::I0Operand(I0Instruction::Immediate, elementSize, I0Instruction::ConvSigned64);
    instOffset->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, I0Instruction::ConvSigned64);
    Emit(instOffset);


    I0Instruction *instAddr = new I0Instruction();
    instAddr->OpCode = I0Instruction::ADD;
    instAddr->Attribute = I0Instruction::Signed64;
    GenerateI0Operand(il->Operands[1], instAddr, 0);
    instAddr->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, I0Instruction::ConvSigned64);
    instAddr->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_2, I0Instruction::ConvSigned64);
    Emit(instAddr);

    I0Instruction *instLoad = new I0Instruction();
    instLoad->OpCode = I0Instruction::CONV;

    GenerateI0Operand(il->Operands[0], instLoad, 1);
    // instLoad->Operands[0] = I0Instruction::I0Operand(I0Instruction::Indirect, GENERAL_REG_2, I0Instruction::ConvSigned64);
    instLoad->Operands[0] = I0Instruction::I0Operand(I0Instruction::Indirect, GENERAL_REG_2,
                                                         GetConvOperandAttribute(il->Operands[0].OperandType));
    Emit(instLoad);

}

void I0CodeGenerator::VisitLdelema(IL* il)
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

    I0Instruction *instOffset = new I0Instruction();
    instOffset->OpCode = I0Instruction::MUL;
    instOffset->Attribute = I0Instruction::Signed64;
    GenerateI0Operand(il->Operands[2], instOffset, 0);
    instOffset->Operands[1] = I0Instruction::I0Operand(I0Instruction::Immediate, elementSize, I0Instruction::ConvSigned64);
    instOffset->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, I0Instruction::ConvSigned64);
    Emit(instOffset);


    I0Instruction *instAddr = new I0Instruction();
    instAddr->OpCode = I0Instruction::ADD;
    instAddr->Attribute = I0Instruction::Signed64;
    GenerateI0Operand(il->Operands[1], instAddr, 0);
    instAddr->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, I0Instruction::ConvSigned64);
    GenerateI0Operand(il->Operands[0], instAddr, 2);
    Emit(instAddr);
}

void I0CodeGenerator::VisitLdfld(IL* il)
{
    // TODO: used by struct
}

void I0CodeGenerator::VisitLdflda(IL* il)
{
    // TODO: used by struct
}

void I0CodeGenerator::VisitLdind(IL* il)
{
    switch(il->Operands[1].OperandKind)
    {
        case IL::Constant:
        {
            I0Instruction *inst = new I0Instruction();
            inst->OpCode = I0Instruction::CONV;
            inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, il->Operands[1].IValue, GetConvOperandAttribute(il->Operands[1].OperandType));
            GenerateI0Operand(il->Operands[0], inst, 1);
            inst->Operands[1].ConvAttribute = GetConvOperandAttribute(il->Operands[1].OperandType);
            Emit(inst);
            break;
        }
        case IL::Variable:
        {
            Symbol *sym = il->Operands[1].SymRef->Lookup();
            assert(sym != NULL);

            if(sym->Scope->GetScopeKind() == SymbolScope::Global)
            {
                I0Instruction *instAddr = new I0Instruction();
                instAddr->OpCode = I0Instruction::CONV;
                instAddr->Operands[0] = I0Instruction::I0Operand(I0Instruction::Indirect, 0xFFFFFFFFDEADBEEF, GetConvOperandAttribute(il->Operands[1].OperandType));
                GenerateI0Operand(il->Operands[0], instAddr, 1);
                instAddr->Operands[1].ConvAttribute = GetConvOperandAttribute(il->Operands[1].OperandType);
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
                I0Instruction *instBase = new I0Instruction();
                instBase->OpCode = I0Instruction::CONV;
                instBase->Operands[0] = I0Instruction::I0Operand(I0Instruction::BaseDisplacement, FRAME_POINTER, sym->Address, I0Instruction::ConvSigned64);
                instBase->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, I0Instruction::ConvSigned64);
                Emit(instBase);

                I0Instruction *instAddr = new I0Instruction();
                instAddr->OpCode = I0Instruction::CONV;
                instAddr->Operands[0] = I0Instruction::I0Operand(I0Instruction::Indirect, GENERAL_REG_1, GetConvOperandAttribute(il->Operands[1].OperandType));
                GenerateI0Operand(il->Operands[0], instAddr, 1);
                instAddr->Operands[1].ConvAttribute = GetConvOperandAttribute(il->Operands[1].OperandType);
                Emit(instAddr);
            }
            break;
        }
        default:
            abort();
    }
}

void I0CodeGenerator::VisitLdstr(IL* il)
{
    assert(!"Not implemented.");
}

void I0CodeGenerator::VisitStelem(IL* il)
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

    I0Instruction *instOffset = new I0Instruction();
    instOffset->OpCode = I0Instruction::MUL;
    instOffset->Attribute = I0Instruction::Signed64;
    GenerateI0Operand(il->Operands[1], instOffset, 0);
    instOffset->Operands[1] = I0Instruction::I0Operand(I0Instruction::Immediate, elementSize, I0Instruction::ConvSigned64);
    instOffset->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, I0Instruction::ConvSigned64);
    Emit(instOffset);


    I0Instruction *instAddr = new I0Instruction();
    instAddr->OpCode = I0Instruction::ADD;
    instAddr->Attribute = I0Instruction::Signed64;
    GenerateI0Operand(il->Operands[0], instAddr, 0);
    instAddr->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, I0Instruction::ConvSigned64);
    instAddr->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_2, I0Instruction::ConvSigned64);
    Emit(instAddr);

    I0Instruction *instStore = new I0Instruction();
    instStore->OpCode = I0Instruction::CONV;
    GenerateI0Operand(il->Operands[2], instStore, 0);
    instStore->Operands[1] = I0Instruction::I0Operand(I0Instruction::Indirect, GENERAL_REG_2, instStore->Operands[0].ConvAttribute);
    Emit(instStore);
}

void I0CodeGenerator::VisitStfld(IL* il)
{
    // TODO: used by struct
}

void I0CodeGenerator::VisitStind(IL* il)
{
    switch(il->Operands[0].OperandKind)
    {
        case IL::Constant:
        {
            I0Instruction *inst = new I0Instruction();
            inst->OpCode = I0Instruction::CONV;
            GenerateI0Operand(il->Operands[1], inst, 0);
            inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, il->Operands[0].IValue, GetConvOperandAttribute(il->Operands[1].OperandType));
            Emit(inst);
            break;
        }
        case IL::Variable:
        {
            Symbol *sym = il->Operands[0].SymRef->Lookup();
            assert(sym != NULL);

            if(sym->Scope->GetScopeKind() == SymbolScope::Global)
            {
                I0Instruction *instAddr = new I0Instruction();
                instAddr->OpCode = I0Instruction::CONV;
                GenerateI0Operand(il->Operands[1], instAddr, 0);
                instAddr->Operands[1] = I0Instruction::I0Operand(I0Instruction::Indirect, 0xFFFFFFFFDEADBEEF, GetConvOperandAttribute(il->Operands[1].OperandType));

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
                I0Instruction *instBase = new I0Instruction();
                instBase->OpCode = I0Instruction::CONV;
                instBase->Operands[0] = I0Instruction::I0Operand(I0Instruction::BaseDisplacement, FRAME_POINTER, sym->Address, I0Instruction::ConvSigned64);
                instBase->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, I0Instruction::ConvSigned64);
                Emit(instBase);

                I0Instruction *instAddr = new I0Instruction();
                instAddr->OpCode = I0Instruction::CONV;
                GenerateI0Operand(il->Operands[1], instAddr, 0);
                instAddr->Operands[1] = I0Instruction::I0Operand(I0Instruction::Indirect, GENERAL_REG_1, GetConvOperandAttribute(il->Operands[1].OperandType));
                Emit(instAddr);
            }
            break;
        }
        default:
            abort();
    }
}

void I0CodeGenerator::VisitStackalloc(IL* il)
{
    assert(!"Not implemented.");
}

void I0CodeGenerator::VisitMul(IL* il)
{
    GenerateBinaryArithmatic(il, I0Instruction::MUL);
}

void I0CodeGenerator::VisitNeg(IL* il)
{
    I0Instruction *inst = new I0Instruction();
    I0Instruction::ConvOperandAttribute convattr;
    inst->OpCode = I0Instruction::SUB;

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
            inst->Attribute = I0Instruction::Signed64;
            convattr = I0Instruction::ConvSigned64;
            break;
        case IL::U1:
        case IL::U2:
        case IL::U4:
            needsIntermediates = true;
        case IL::U8:
        case IL::U:
            inst->Attribute = I0Instruction::Unsigned64;
            convattr = I0Instruction::ConvUnsigned64;
            break;
        case IL::R4:
            inst->Attribute = I0Instruction::Single;
            convattr = I0Instruction::ConvSingle;
            break;
        case IL::R8:
            inst->Attribute = I0Instruction::Double;
            convattr = I0Instruction::ConvDouble;
            break;
        default:
            abort();
    }

    if(needsIntermediates)
    {
        I0Instruction *loadInst2 = new I0Instruction();
        loadInst2->OpCode = I0Instruction::CONV;
        GenerateI0Operand(il->Operands[2], loadInst2, 0);
        loadInst2->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, convattr);

        inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Immediate, (int64_t)0, convattr);
        inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, convattr);
        inst->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, convattr);

        I0Instruction *saveInst = new I0Instruction();
        saveInst->OpCode = I0Instruction::CONV;
        saveInst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, GENERAL_REG_1, convattr);
        GenerateI0Operand(il->Operands[0], saveInst, 1);

        Emit(loadInst2);
        Emit(inst);
        Emit(saveInst);
    }
    else
    {
        inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Immediate, (int64_t)0, convattr);
        GenerateI0Operand(il->Operands[1], inst, 1);
        GenerateI0Operand(il->Operands[0], inst, 2);
        Emit(inst);
    }
}

void I0CodeGenerator::VisitNewarr(IL* il)
{
    assert(!"Not implemented.");

}

void I0CodeGenerator::VisitNewobj(IL* il)
{
    assert(!"Not implemented.");

}

void I0CodeGenerator::VisitNot(IL* il)
{
    assert(!"Not implemented.");
}

void I0CodeGenerator::VisitOr(IL* il)
{
    GenerateBinaryArithmatic(il, I0Instruction::OR);
}

void I0CodeGenerator::VisitMod(IL* il)
{
    assert(!"Not implemented.");

}

void I0CodeGenerator::VisitRet(IL* il)
{
    //NOTE: Step 1. store the return value, if exists
    if(il->Operands.size() > 0 && il->Operands[0].OperandKind != IL::Empty)
    {
        I0Instruction *stInst = new I0Instruction();
        stInst->OpCode = I0Instruction::CONV;
        GenerateI0Operand(il->Operands[0], stInst, 0);
        stInst->Operands[1] = I0Instruction::I0Operand(I0Instruction::BaseDisplacement, FRAME_POINTER, PARAMETER_OFFSET, stInst->Operands[0].ConvAttribute);
        Emit(stInst);
    }


    /* NOTE: Step 2. Leave the stack frame, after that the return address is on the stack top
     * Original:
     *     mov %rbp, %rsp
     *     pop %rbp
     */

    I0Instruction *inst;

    inst = new I0Instruction();
    inst->OpCode = I0Instruction::CONV;
    inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, FRAME_POINTER, I0Instruction::ConvSigned64);
    inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, STACK_POINTER, I0Instruction::ConvSigned64);
    inst->Operands[2] = I0Instruction::I0Operand(I0Instruction::Indirect, STACK_POINTER, I0Instruction::ConvSigned64);
    Emit(inst);

    inst = new I0Instruction();
    inst->OpCode = I0Instruction::CONV;
    inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Indirect, STACK_POINTER, I0Instruction::ConvSigned64);
    inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Direct, FRAME_POINTER, I0Instruction::ConvSigned64);
    Emit(inst);

    inst = new I0Instruction();
    inst->OpCode = I0Instruction::ADD;
    inst->Attribute = I0Instruction::Signed64;
    inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, STACK_POINTER, I0Instruction::ConvSigned64);
    inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Immediate, (int64_t)(PointerType::PointerSize), I0Instruction::ConvSigned64);
    inst->Operands[2] = I0Instruction::I0Operand(I0Instruction::Direct, STACK_POINTER, I0Instruction::ConvSigned64);
    Emit(inst);

    // NOTE: Step 3, jump to the caller
    I0Instruction *brInst = new I0Instruction();
    brInst->OpCode = I0Instruction::B;
    brInst->JumpMode = I0Instruction::JI;
    brInst->RelativeJump = false;
    brInst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Indirect, STACK_POINTER, I0Instruction::ConvSigned64);

    Emit(brInst);
}

void I0CodeGenerator::VisitShl(IL* il)
{
    assert(!"Not implemented.");

}

void I0CodeGenerator::VisitShr(IL* il)
{
    assert(!"Not implemented.");
}

void I0CodeGenerator::VisitSub(IL* il)
{
    GenerateBinaryArithmatic(il, I0Instruction::SUB);
}

void I0CodeGenerator::VisitConv(IL* il)
{
    VisitMov(il);
}

void I0CodeGenerator::VisitXor(IL* il)
{
    GenerateBinaryArithmatic(il, I0Instruction::XOR);
}

void I0CodeGenerator::VisitAsm(IL* il)
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

void I0CodeGenerator::VisitLabel(IL* il)
{
    assert(il->Operands[0].OperandKind == IL::Variable);
    Symbol *symbol = il->Operands[0].SymRef->Lookup();
    assert(symbol != NULL);
    symbol->Address = _currentText;
}

void I0CodeGenerator::VisitLda(IL* il)
{
    assert(il->Operands[1].OperandKind == IL::Variable);

    // debug
    // printf("VisitLda\n");


    Symbol *symbol = il->Operands[1].SymRef->Lookup();
    assert(symbol != NULL);

    // debug
    // printf("A\n");

    if(symbol->Scope->GetScopeKind() == SymbolScope::Global || typeid(*(symbol->DeclType)) == typeid(FunctionType) || typeid(*(symbol->DeclType)) == typeid(LabelType))
    {
        // debug
        // printf("C\n");

        I0Instruction *inst = new I0Instruction();
        inst->OpCode = I0Instruction::CONV;
        inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Immediate, 0xFFFFFFFFDEADBEEF, I0Instruction::ConvSigned64);
        inst->Operands[0].SymRef = new SymbolRef(symbol);
        GenerateI0Operand(il->Operands[0], inst, 1);
        Emit(inst);
    }
    else
    {
        // debug
        // printf("D\n");

        I0Instruction *inst = new I0Instruction();
        inst->OpCode = I0Instruction::ADD;
        // bugfix by zma.
        inst->Attribute = I0Instruction::Signed64;
        inst->Operands[0] = I0Instruction::I0Operand(I0Instruction::Direct, FRAME_POINTER, I0Instruction::ConvSigned64);
        inst->Operands[1] = I0Instruction::I0Operand(I0Instruction::Immediate, symbol->Address, I0Instruction::ConvSigned64);
        // debug
        // printf("E\n");
        GenerateI0Operand(il->Operands[0], inst, 2);
        // debug
        // printf("F\n");
        Emit(inst);
        // debug
        // printf("G\n");
    }
}

I0Instruction::ConvOperandAttribute I0CodeGenerator::GetConvOperandAttribute(IL::ILOperandType optype)
{
    I0Instruction::ConvOperandAttribute elementSize;
    switch(optype)
    {
        case IL::Void:
        case IL::I:
        case IL::I8:
            elementSize = I0Instruction::ConvSigned64;
            break;
        case IL::U:
        case IL::U8:
            elementSize = I0Instruction::ConvUnsigned64;
            break;
        case IL::I1:
            elementSize = I0Instruction::ConvSigned8;
            break;
        case IL::U1:
            elementSize = I0Instruction::ConvUnsigned8;
            break;
        case IL::I2:
            abort();
        case IL::U2:
            abort();
        case IL::I4:
            elementSize = I0Instruction::ConvSigned32;
            break;
        case IL::U4:
            elementSize = I0Instruction::ConvUnsigned32;
            break;
        case IL::R4:
            elementSize = I0Instruction::ConvSingle;
            break;
        case IL::R8:
            elementSize = I0Instruction::ConvDouble;
            break;
        case IL::Str:
            elementSize = I0Instruction::ConvSigned64;
            break;

        default:
            abort();
    }

    return elementSize;
}
int64_t I0CodeGenerator::GetOperandSize(IL::ILOperandType optype, int64_t align)
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

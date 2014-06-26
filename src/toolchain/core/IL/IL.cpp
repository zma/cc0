#include "IL.h"
#include "ILVisitor.h"
#include <core/Core.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

IL::IL():Opcode(Nop)
{
}

IL::IL(IL::ILOpcode opcode)
{
    this->Opcode = opcode;
    this->Operands.clear();
}

IL::IL(IL::ILOpcode opcode, const IL::ILOperand& operand1)
{
    this->Opcode = opcode;
    this->Operands.clear();
    this->Operands.push_back(operand1);
}

IL::IL(IL::ILOpcode opcode, const IL::ILOperand& operand1, const IL::ILOperand& operand2)
{
    this->Opcode = opcode;
    this->Operands.clear();
    this->Operands.push_back(operand1);
    this->Operands.push_back(operand2);
}


IL::IL(IL::ILOpcode opcode, const IL::ILOperand& operand1, const IL::ILOperand& operand2, const IL::ILOperand& operand3)
{
    this->Opcode = opcode;
    this->Operands.clear();
    this->Operands.push_back(operand1);
    this->Operands.push_back(operand2);
    this->Operands.push_back(operand3);
}

IL::IL(IL::ILOpcode opcode, const IL::ILOperand& operand1, const IL::ILOperand& operand2, const IL::ILOperand& operand3, const IL::ILOperand& operand4)
{
    this->Opcode = opcode;
    this->Operands.clear();
    this->Operands.push_back(operand1);
    this->Operands.push_back(operand2);
    this->Operands.push_back(operand3);
    this->Operands.push_back(operand4);
}

IL::IL(IL::ILOpcode opcode, const std::vector< IL::ILOperand >& operands)
{
    this->Opcode = opcode;
    this->Operands = operands;
}

IL::ILOperand::ILOperand()
{
    OperandKind = IL::Empty;
    OperandType = IL::I;
    IValue = 0;
    UValue = 0;
    RValue = 0;
    SValue = "";
    SymRef = NULL;
    ObjectType = NULL;
}


IL::ILOperand::ILOperand(IL::ILOperandKind kind, IL::ILOperandType type, intmax_t v)
{
    OperandKind = kind;
    OperandType = type;
    IValue = (intmax_t)v;
    UValue = (uintmax_t)v;
    RValue = (double)v;
    SValue = "";
    SymRef = NULL;
    ObjectType = NULL;
}

IL::ILOperand::ILOperand(IL::ILOperandKind kind, IL::ILOperandType type, uintmax_t v)
{
    OperandKind = kind;
    OperandType = type;
    IValue = (intmax_t)v;
    UValue = (uintmax_t)v;
    RValue = (double)v;
    SValue = "";
    SymRef = NULL;
    ObjectType = NULL;
}

IL::ILOperand::ILOperand(IL::ILOperandKind kind, IL::ILOperandType type, double v)
{
    OperandKind = kind;
    OperandType = type;
    IValue = (intmax_t)v;
    UValue = (uintmax_t)v;
    RValue = (double)v;
    SValue = "";
    SymRef = NULL;
    ObjectType = NULL;
}

IL::ILOperand::ILOperand(IL::ILOperandKind kind, IL::ILOperandType type, std::string v)
{
    OperandKind = kind;
    OperandType = type;
    IValue = 0;
    UValue = 0;
    RValue = 0;
    SValue = v;
    SymRef = NULL;
    ObjectType = NULL;
}


IL::ILOperand::ILOperand(IL::ILOperandKind kind, IL::ILOperandType type, Symbol* v)
{
    OperandKind = kind;
    OperandType = type;
    IValue = 0;
    UValue = 0;
    RValue = 0;
    SValue = "";
    SymRef = new SymbolRef(v);
    ObjectType = NULL;
}


IL::ILOperand::ILOperand(IL::ILOperandKind kind, IL::ILOperandType type, SymbolRef* v)
{
    OperandKind = kind;
    OperandType = type;
    IValue = 0;
    UValue = 0;
    RValue = 0;
    SValue = "";
    SymRef = v;
    ObjectType = NULL;
}

int IL::ILOperand::Equal(IL::ILOperand* operand)
{
    if (OperandKind != operand->OperandKind) return 0;
    if (OperandType != operand->OperandType) return 0;
    if (IValue != operand->IValue) return 0;
    if (UValue != operand->UValue) return 0;
    if (RValue != operand->RValue) return 0;
    if (SValue != operand->SValue) return 0;
    if (SymRef != NULL) {
        if (SymRef->Scope != operand->SymRef->Scope) return 0;
        if (SymRef->Name != operand->SymRef->Name) return 0;
    }
    if (ObjectType != operand->ObjectType) return 0;
    return 1;
}


std::string IL::GetOperandString(const IL::ILOperand& operand)
{
    std::string opstr = "";
    char buffer[200];
    switch(operand.OperandKind)
    {
        case Empty:
            sprintf(buffer, "<missing>");
            opstr += buffer;
            break;
        case Constant:
            opstr = "";
            switch(operand.OperandType)
            {
                case Void:
                    sprintf(buffer, "void");
                    break;
                case I:
                    sprintf(buffer, "0x%llX:i", (long long)operand.IValue);
                    break;
                case I1:
                    sprintf(buffer, "0x%llX:i1", (long long)operand.IValue);
                    break;
                case I2:
                    sprintf(buffer, "0x%llX:i2", (long long)operand.IValue);
                    break;
                case I4:
                    sprintf(buffer, "0x%llX:i4", (long long)operand.IValue);
                    break;
                case I8:
                    sprintf(buffer, "0x%llX:i8", (long long)operand.IValue);
                    break;
                case U:
                    sprintf(buffer, "0x%llX:u", (long long)operand.UValue);
                    break;
                case U1:
                    sprintf(buffer, "0x%llX:u1", (long long)operand.UValue);
                    break;
                case U2:
                    sprintf(buffer, "0x%llX:u2", (long long)operand.UValue);
                    break;
                case U4:
                    sprintf(buffer, "0x%llX:u4", (long long)operand.UValue);
                    break;
                case U8:
                    sprintf(buffer, "0x%llX:u8", (long long)operand.UValue);
                    break;
                case R4:
                    sprintf(buffer, "%f:r4", (float)operand.RValue);
                    break;
                case R8:
                    sprintf(buffer, "%lf:r8", (double)operand.RValue);
                    break;
                case Str:
                    sprintf(buffer, "\"%s\"", operand.SValue.c_str());
                    break;
                default:
                    abort();
            }
            opstr += buffer;
            break;
        case Variable:
            sprintf(buffer, "%s$%p", operand.SymRef->Name.c_str(), operand.SymRef->Scope);
            opstr += buffer;
            break;
        case FieldToken:
            sprintf(buffer, "%s", operand.SValue.c_str());
            opstr += buffer;
            break;            
        default:
            abort();
    }

    return std::string(opstr);
}

std::string IL::ToString()
{
    std::string opstr;
    switch(Opcode)
    {
        case Nop:
            opstr = "nop";
            break;
        case Add:
            opstr = "add";
            break;
        case And:
            opstr = "and";
            break;
        case BrEq:
            opstr = "beq";
            break;
        case BrGt:
            opstr = "bgt";
            break;
        case BrGe:
            opstr = "bge";
            break;
        case BrLt:
            opstr = "blt";
            break;
        case BrLe:
            opstr = "ble";
            break;
        case BrNe:
            opstr = "bne";
            break;
        case Br:
            opstr = "br";
            break;
        case BrZ:
            opstr = "bz";
            break;
        case BrNz:
            opstr = "bnz";
            break;
        case Call:
            opstr = "call";
            break;
        case CallInd:
            opstr = "calli";
            break;
        case CallVirt:
            opstr = "callvirt";
            break;
        case Cast:
            opstr = "cast";
            break;
        case CompEq:
            opstr = "ceq";
            break;
        case CompGt:
            opstr = "cgt";
            break;
        case CompGe:
            opstr = "cge";
            break;
        case CompLt:
            opstr = "clt";
            break;
        case CompLe:
            opstr = "cle";
            break;
        case CompNe:
            opstr = "cne";
            break;
        case Conv:
            opstr = "conv";
            break;
        case Cpblk:
            opstr = "cpblk";
            break;
        case Cpobj:
            opstr = "cpobj";
            break;
        case Div:
            opstr = "div";
            break;
        case Initblk:
            opstr = "initblk";
            break;
        case Initobj:
            opstr = "initobj";
            break;
        case Ldelem:
            opstr = "ldelem";
            break;
        case Ldelema:
            opstr = "ldelema";
            break;
        case Ldfld:
            opstr = "ldfld";
            break;
        case Ldflda:
            opstr = "ldflda";
            break;
        case Ldind:
            opstr = "ldi";
            break;
        case Stelem:
            opstr = "stelem";
            break;
        case Stfld:
            opstr = "stfld";
            break;
        case Stind:
            opstr = "sti";
            break;
        case Stackalloc:
            opstr = "stackalloc";
            break;
        case Mul:
            opstr = "mul";
            break;
        case Neg:
            opstr = "neg";
            break;
        case Newarr:
            opstr = "newarr";
            break;
        case Newobj:
            opstr = "newobj";
            break;
        case Not:
            opstr = "not";
            break;
        case Or:
            opstr = "or";
            break;
        case Mod:
            opstr = "mod";
            break;
        case Ret:
            opstr = "ret";
            break;
        case Shl:
            opstr = "shl";
            break;
        case Shr:
            opstr = "shr";
            break;
        case Mov:
            opstr = "mov";
            break;
        case Xor:
            opstr = "xor";
            break;
        case Asm:
            opstr = "asm";
            break;
        case Label:
            opstr = "";
            break;
        case Sub:
            opstr = "sub";
            break;
        case Lda:
            opstr = "lda";
            break;
        default:
            abort();
            break;
    }

    for(std::vector<ILOperand>::iterator it = Operands.begin(); it != Operands.end(); ++it)
    {
        if(it == Operands.begin())
        {
            if(Opcode != IL::Label)
            {
                opstr += " ";
            }
        }
        else
        {
            opstr += ", ";
        }

        opstr += GetOperandString(*it);
    }

    if(Opcode == IL::Label)
    {
        opstr += ":";
    }
    return opstr;

}

void IL::Accept(ILVisitor* visitor)
{
    switch(Opcode)
    {
        case Nop:
            visitor->VisitNop(this);
            break;
        case Add:
            visitor->VisitAdd(this);
            break;
        case And:
            visitor->VisitAnd(this);
            break;
        case BrEq:
            visitor->VisitBrEq(this);
            break;
        case BrGt:
            visitor->VisitBrGt(this);
            break;
        case BrGe:
            visitor->VisitBrGe(this);
            break;
        case BrLt:
            visitor->VisitBrLt(this);
            break;
        case BrLe:
            visitor->VisitBrLe(this);
            break;
        case BrNe:
            visitor->VisitBrNe(this);
            break;
        case Br:
            visitor->VisitBr(this);
            break;
        case BrZ:
            visitor->VisitBrZ(this);
            break;
        case BrNz:
            visitor->VisitBrNz(this);
            break;
        case Call:
            visitor->VisitCall(this);
            break;
        case CallInd:
            visitor->VisitCallInd(this);
            break;
        case CallVirt:
            visitor->VisitCallVirt(this);
            break;
        case Cast:
            visitor->VisitCast(this);
            break;
        case CompEq:
            visitor->VisitCompEq(this);
            break;
        case CompGt:
            visitor->VisitCompGt(this);
            break;
        case CompGe:
            visitor->VisitCompGe(this);
            break;
        case CompLt:
            visitor->VisitCompLt(this);
            break;
        case CompLe:
            visitor->VisitCompLe(this);
            break;
        case CompNe:
            visitor->VisitCompNe(this);
            break;
        case Conv:
            visitor->VisitConv(this);
            break;
        case Cpblk:
            visitor->VisitCpblk(this);
            break;
        case Cpobj:
            visitor->VisitCpobj(this);
            break;
        case Div:
            visitor->VisitDiv(this);
            break;
        case Initblk:
            visitor->VisitInitblk(this);
            break;
        case Initobj:
            visitor->VisitInitobj(this);
            break;
        case Ldelem:
            visitor->VisitLdelem(this);
            break;
        case Ldelema:
            visitor->VisitLdelema(this);
            break;
        case Ldfld:
            visitor->VisitLdfld(this);
            break;
        case Ldflda:
            visitor->VisitLdflda(this);
            break;
        case Ldind:
            visitor->VisitLdind(this);
            break;
        case Ldstr:
            visitor->VisitLdstr(this);
            break;
        case Stelem:
            visitor->VisitStelem(this);
            break;
        case Stfld:
            visitor->VisitStfld(this);
            break;
        case Stind:
            visitor->VisitStind(this);
            break;
        case Stackalloc:
            visitor->VisitStackalloc(this);
            break;
        case Mul:
            visitor->VisitMul(this);
            break;
        case Neg:
            visitor->VisitNeg(this);
            break;
        case Newarr:
            visitor->VisitNewarr(this);
            break;
        case Newobj:
            visitor->VisitNewobj(this);
            break;
        case Not:
            visitor->VisitNot(this);
            break;
        case Or:
            visitor->VisitOr(this);
            break;
        case Mod:
            visitor->VisitMod(this);
            break;
        case Ret:
            visitor->VisitRet(this);
            break;
        case Shl:
            visitor->VisitShl(this);
            break;
        case Shr:
            visitor->VisitShr(this);
            break;
        case Mov:
            visitor->VisitMov(this);
            break;
        case Xor:
            visitor->VisitXor(this);
            break;
        case Asm:
            visitor->VisitAsm(this);
            break;
        case Label:
            visitor->VisitLabel(this);
            break;
        case Sub:
            visitor->VisitSub(this);
            break;
        case Lda:
            visitor->VisitLda(this);
            break;
        default:
            abort();
    }
}


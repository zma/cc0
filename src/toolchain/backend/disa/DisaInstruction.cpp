#include "DisaInstruction.h"
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <DisaLexer.h>
#include <DisaParser.h>

#undef ADD

DisaInstruction::DisaOperand::DisaOperand()
{
    Resolving = 0;
    AddressingMode = (OperandAddressingMode)0;
    MovAttribute = (MovOperandAttribute)0;;
    IntValue = 0;
    UIntValue = 0;
    DoubleValue = 0;
    SingleValue = 0;
    Address = 0;
    Displacement = 0;
    SymRef = NULL;
}

DisaInstruction::DisaOperand::DisaOperand(DisaInstruction::OperandAddressingMode addrm, int64_t v, DisaInstruction::MovOperandAttribute movattr)
{
    Resolving = 0;
    AddressingMode = (OperandAddressingMode)0;
    MovAttribute = (MovOperandAttribute)0;;
    IntValue = 0;
    UIntValue = 0;
    DoubleValue = 0;
    SingleValue = 0;
    Address = 0;
    Displacement = 0;
    SymRef = NULL;

    Resolving = false;
    AddressingMode = addrm;
    IntValue = v;
    MovAttribute = movattr;
}

DisaInstruction::DisaOperand::DisaOperand(DisaInstruction::OperandAddressingMode addrm, uint64_t v, DisaInstruction::MovOperandAttribute movattr)
{
    Resolving = 0;
    AddressingMode = (OperandAddressingMode)0;
    MovAttribute = (MovOperandAttribute)0;;
    IntValue = 0;
    UIntValue = 0;
    DoubleValue = 0;
    SingleValue = 0;
    Address = 0;
    Displacement = 0;
    SymRef = NULL;

    Resolving = false;
    AddressingMode = addrm;
    UIntValue = v;
    MovAttribute = movattr;
}

DisaInstruction::DisaOperand::DisaOperand(DisaInstruction::OperandAddressingMode addrm, float v)
{
    Resolving = 0;
    AddressingMode = (OperandAddressingMode)0;
    MovAttribute = (MovOperandAttribute)0;;
    IntValue = 0;
    UIntValue = 0;
    DoubleValue = 0;
    SingleValue = 0;
    Address = 0;
    Displacement = 0;
    SymRef = NULL;

    MovAttribute = DisaInstruction::MovSingle;
    Resolving = false;
    AddressingMode = addrm;
    SingleValue = v;
}

DisaInstruction::DisaOperand::DisaOperand(DisaInstruction::OperandAddressingMode addrm, double v)
{
    Resolving = 0;
    AddressingMode = (OperandAddressingMode)0;
    MovAttribute = (MovOperandAttribute)0;;
    IntValue = 0;
    UIntValue = 0;
    DoubleValue = 0;
    SingleValue = 0;
    Address = 0;
    Displacement = 0;
    SymRef = NULL;

    MovAttribute = DisaInstruction::MovDouble;
    Resolving = false;
    AddressingMode = addrm;
    DoubleValue = v;
}

DisaInstruction::DisaOperand::DisaOperand(DisaInstruction::OperandAddressingMode addrm, int64_t base, int64_t displacement, DisaInstruction::MovOperandAttribute movattr)
{
    Resolving = 0;
    AddressingMode = (OperandAddressingMode)0;
    MovAttribute = (MovOperandAttribute)0;;
    IntValue = 0;
    UIntValue = 0;
    DoubleValue = 0;
    SingleValue = 0;
    Address = 0;
    Displacement = 0;
    SymRef = NULL;

    Resolving = false;
    AddressingMode = addrm;
    Address = base;
    Displacement = (int32_t)displacement;
    MovAttribute = movattr;
}

DisaInstruction::DisaInstruction()
{

}

DisaInstruction::~DisaInstruction()
{

}

int64_t DisaInstruction::GetOperandEncodingLength(DisaInstruction::DisaOperand operand)
{
    int64_t n;
    switch(operand.AddressingMode)
    {
        case Immediate:

            switch(this->OpCode)
            {
                case DisaInstruction::MOV:
                    switch(operand.MovAttribute)
                    {
                        case MovSigned64:
                        case MovUnsigned64:
                            n = 8;
                            break;
                        case MovSigned32:
                        case MovUnsigned32:
                            n = 4;
                            break;
                        case MovSigned8:
                        case MovUnsigned8:
                            n = 1;
                            break;
                        case MovDouble:
                            n = 8;
                            break;
                        case MovSingle:
                            n = 4;
                            break;
                        default:
                            abort();
                    }
                    break;
                case DisaInstruction::ADD:
                case DisaInstruction::SUB:
                case DisaInstruction::MUL:
                case DisaInstruction::DIV:
                case DisaInstruction::AND:
                case DisaInstruction::OR:
                case DisaInstruction::XOR:
                    switch(this->Attribute)
                    {
                        case Signed64:
                        case Unsigned64:
                            n = 8;
                            break;
                        case Double:
                            n = 8;
                            break;
                        case Single:
                            n = 4;
                            break;
                        default:
                            abort();
                    }
                    break;
                case DisaInstruction::NEWR:
                case DisaInstruction::BR:
                case DisaInstruction::STRCMP:
                    n = 8;
                    break;
                case DisaInstruction::EXIT:
                case DisaInstruction::NOP:
                default:
                    abort();
            }
            break;
        case Direct:
        case Indirect:
            n = 8;
            break;
        case BaseDisplacement:
            n = 8 + 4;
            break;
        default:
            abort();
    }
    return n;
}

char* DisaInstruction::EncodeOperandData(char* buffer, DisaInstruction::DisaOperand operand)
{
    char *p = buffer;

    switch(operand.AddressingMode)
    {
        case Immediate:
            switch(this->OpCode)
            {
                case DisaInstruction::MOV:
                case DisaInstruction::ADD:
                case DisaInstruction::SUB:
                case DisaInstruction::MUL:
                case DisaInstruction::DIV:
                case DisaInstruction::AND:
                case DisaInstruction::OR:
                case DisaInstruction::XOR:
                case DisaInstruction::NEWR:
                case DisaInstruction::BR:
                case DisaInstruction::STRCMP:
                    // NOTE: I do not assume the host machine is little-endian, so I explicitly encode each byte of integers
                    // FIXME: Does floating-point encoding has endianess? I assume no, but I'm not sure.
                    switch(operand.MovAttribute)
                    {
                        case MovSigned64:
                            *p++ = (operand.IntValue >> 0x00) & 0xFF;
                            *p++ = (operand.IntValue >> 0x08) & 0xFF;
                            *p++ = (operand.IntValue >> 0x10) & 0xFF;
                            *p++ = (operand.IntValue >> 0x18) & 0xFF;
                            *p++ = (operand.IntValue >> 0x20) & 0xFF;
                            *p++ = (operand.IntValue >> 0x28) & 0xFF;
                            *p++ = (operand.IntValue >> 0x30) & 0xFF;
                            *p++ = (operand.IntValue >> 0x38) & 0xFF;
                            break;
                        case MovSigned32:
                            *p++ = (operand.IntValue >> 0x00) & 0xFF;
                            *p++ = (operand.IntValue >> 0x08) & 0xFF;
                            *p++ = (operand.IntValue >> 0x10) & 0xFF;
                            *p++ = (operand.IntValue >> 0x18) & 0xFF;
                            break;
                        case MovSigned8:
                            *p++ = (operand.IntValue >> 0x00) & 0xFF;
                            break;
                        case MovUnsigned64:
                            *p++ = (operand.UIntValue >> 0x00) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x08) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x10) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x18) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x20) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x28) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x30) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x38) & 0xFF;
                            break;
                        case MovUnsigned32:
                            *p++ = (operand.UIntValue >> 0x00) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x08) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x10) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x18) & 0xFF;
                            break;
                        case MovUnsigned8:
                            *p++ = (operand.UIntValue >> 0x00) & 0xFF;
                            break;
                        case MovDouble:
                            memcpy(p, &operand.DoubleValue, 8);
                            p += 8;
                            break;
                        case MovSingle:
                            memcpy(p, &operand.SingleValue, 4);
                            p += 4;
                            break;
                        default:
                            abort();
                    }
                    break;
                case DisaInstruction::EXIT:
                case DisaInstruction::NOP:
                default:
                    abort();
            }
            break;
        case Direct:
        case Indirect:
            *p++ = (operand.Address >> 0x00) & 0xFF;
            *p++ = (operand.Address >> 0x08) & 0xFF;
            *p++ = (operand.Address >> 0x10) & 0xFF;
            *p++ = (operand.Address >> 0x18) & 0xFF;
            *p++ = (operand.Address >> 0x20) & 0xFF;
            *p++ = (operand.Address >> 0x28) & 0xFF;
            *p++ = (operand.Address >> 0x30) & 0xFF;
            *p++ = (operand.Address >> 0x38) & 0xFF;
            break;
        case BaseDisplacement:
            *p++ = (operand.Displacement >> 0x00) & 0xFF;
            *p++ = (operand.Displacement >> 0x08) & 0xFF;
            *p++ = (operand.Displacement >> 0x10) & 0xFF;
            *p++ = (operand.Displacement >> 0x18) & 0xFF;
            *p++ = (operand.Address >> 0x00) & 0xFF;
            *p++ = (operand.Address >> 0x08) & 0xFF;
            *p++ = (operand.Address >> 0x10) & 0xFF;
            *p++ = (operand.Address >> 0x18) & 0xFF;
            *p++ = (operand.Address >> 0x20) & 0xFF;
            *p++ = (operand.Address >> 0x28) & 0xFF;
            *p++ = (operand.Address >> 0x30) & 0xFF;
            *p++ = (operand.Address >> 0x38) & 0xFF;
            break;
        default:
            abort();
    }
    return p;
}


void DisaInstruction::Encode(char* buffer)
{
    char *p = buffer;

    *p++ = OpCode & 0xFF;

    switch(OpCode)
    {
        case MOV:
            *p++ = (Operands[0].MovAttribute << 4) | (Operands[1].MovAttribute << 0);
            *p++ = (Operands[0].AddressingMode << 5) | (Operands[1].AddressingMode << 2);
            p = EncodeOperandData(p, Operands[0]);
            p = EncodeOperandData(p, Operands[1]);
            break;
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case AND:
        case OR:
        case XOR:
            *p++ = (Attribute << 5) | (Operands[0].AddressingMode << 2) | ((Operands[1].AddressingMode >> 1) & 0x3);
            *p++ = ((Operands[1].AddressingMode & 0x1) << 7) | (Operands[2].AddressingMode << 4);
            p = EncodeOperandData(p, Operands[0]);
            p = EncodeOperandData(p, Operands[1]);
            p = EncodeOperandData(p, Operands[2]);
            break;
        case BR:
            if(JumpMode == J)
            {
                *p++ = (JumpMode << 4) | ((RelativeJump ? 1 : 0) << 3);
                p = EncodeOperandData(p, Operands[0]);
            }
            else if(JumpMode == JI)
            {
                *p++ = (JumpMode << 4) | (Operands[0].AddressingMode << 1);
                p = EncodeOperandData(p, Operands[0]);
            }
            else if(JumpMode == NE || JumpMode == E || JumpMode == L || JumpMode == LE || JumpMode == SL)
            {
                *p++ = (JumpMode << 4) | (Attribute << 1) | ((Operands[0].AddressingMode >> 2) & 1);
                *p++ = ((Operands[0].AddressingMode & 3) << 6) | (Operands[1].AddressingMode << 3) | ((RelativeJump ? 1 : 0) << 2);
                p = EncodeOperandData(p, Operands[0]);
                p = EncodeOperandData(p, Operands[1]);
                p = EncodeOperandData(p, Operands[2]);
            }
            else if(JumpMode == NZ || JumpMode == Z)
            {
                *p++ = (JumpMode << 4) | (Attribute << 1) | ((Operands[0].AddressingMode >> 2) & 1);
                *p++ = ((Operands[0].AddressingMode & 3) << 6) | ((RelativeJump ? 1 : 0) << 5);
                p = EncodeOperandData(p, Operands[0]);
                p = EncodeOperandData(p, Operands[1]);
            }
            else
            {
                abort();
            }

            break;
        case NEWR:
            *p++ = (Operands[0].AddressingMode << 5) | (Operands[1].AddressingMode << 2) | ((Operands[2].AddressingMode >> 1) & 0x3);
            *p++ = ((Operands[2].AddressingMode & 1) << 7) | (Operands[3].AddressingMode << 4);
            p = EncodeOperandData(p, Operands[0]);
            p = EncodeOperandData(p, Operands[1]);
            p = EncodeOperandData(p, Operands[2]);
            p = EncodeOperandData(p, Operands[3]);
            break;
        case EXIT:
            *p++ = (ExitMode << 6);
            break;
        case NOP:
            break;
        case INT:
            *p++ = (Operands[0].IntValue & 0xFF);
            break;
        case STRCMP:
            *p++ = (Operands[0].AddressingMode << 5) | (Operands[1].AddressingMode << 2) | ((Operands[2].AddressingMode >> 1) & 0x3);
            *p++ = ((Operands[2].AddressingMode & 1) << 7) | (Operands[3].AddressingMode << 4) | (Operands[4].AddressingMode << 1);
            p = EncodeOperandData(p, Operands[0]);
            p = EncodeOperandData(p, Operands[1]);
            p = EncodeOperandData(p, Operands[2]);
            p = EncodeOperandData(p, Operands[3]);
            p = EncodeOperandData(p, Operands[4]);
            break;
        default:
            abort();
    }
}

std::string DisaInstruction::GetMovOperand(DisaInstruction::DisaOperand op)
{
    char buf[50];
    const char *suffix;



    switch(op.AddressingMode)
    {
        case DisaInstruction::Immediate:
            if(op.SymRef == NULL)
            {
                switch(op.MovAttribute)
                {
                    case MovSigned64:
                        sprintf(buf, "0x%llX", (long long)(int64_t)op.IntValue);
                        break;
                    case MovUnsigned64:
                        sprintf(buf, "0x%llX", (long long)(uint64_t)op.UIntValue);
                        break;
                    case MovSigned32:
                        sprintf(buf, "0x%X", (int32_t)op.IntValue);
                        break;
                    case MovUnsigned32:
                        sprintf(buf, "0x%X", (uint32_t)op.UIntValue);
                        break;
                    case MovSigned8 :
                        sprintf(buf, "0x%X", (int8_t)op.IntValue);
                        break;
                    case MovUnsigned8:
                        sprintf(buf, "0x%X", (uint8_t)op.UIntValue);
                        break;
                    case MovDouble:
                        sprintf(buf, "%lf", op.DoubleValue);
                        break;
                    case MovSingle:
                        sprintf(buf, "%f", op.SingleValue);
                        break;
                    default:
                        abort();
                }
            }
            else
            {
         sprintf(buf, "%s", op.SymRef->Name.c_str());
            }
            return "$" + std::string(buf);
        case DisaInstruction::Direct:
            if(op.SymRef == NULL)
            {
                sprintf(buf, "0x%llX", (long long)(int64_t)op.Address);
            }
            else
            {
                sprintf(buf, "%s", op.SymRef->Name.c_str());
            }

            break;
        case DisaInstruction::Indirect:
            if(op.SymRef== NULL)
            {
                sprintf(buf, "(0x%llX)", (long long)(int64_t)op.Address);
            }
            else
            {
                sprintf(buf, "(%s)", op.SymRef->Name.c_str());
            }

            break;
        case DisaInstruction::BaseDisplacement:
            if(op.SymRef == NULL)
            {
                sprintf(buf, "0x%llX(0x%llX)", (long long)(op.Displacement & 0x00000000FFFFFFFF), (long long)(int64_t)op.Address);
            }
            else
            {
                sprintf(buf, "0x%llX(%s)", (long long)(op.Displacement & 0x00000000FFFFFFFF), op.SymRef->Name.c_str());
            }


            break;
        default:
            abort();
    }
    
                switch(op.MovAttribute)
            {
                case MovSigned64:
                    return std::string(buf) + ":msq";
                case MovUnsigned64:
                    return std::string(buf) + ":muq";
                case MovSigned32:
                    return std::string(buf) + ":msd";
                case MovUnsigned32:
                    return std::string(buf) + ":mud";
                case MovSigned8 :
                    return std::string(buf) + ":msb";
                case MovUnsigned8:
                    return std::string(buf) + ":mub";
                case MovDouble:
                    return std::string(buf) + ":mfd";
                case MovSingle:
                    return std::string(buf) + ":mfs";
                default:
                    abort();
            }

}

std::string DisaInstruction::GetOperand(DisaInstruction::DisaOperand op)
{
    char buf[150];
    switch(op.AddressingMode)
    {

        case DisaInstruction::Immediate:
            if(op.SymRef != NULL)
            {
                switch(this->Attribute)
                {
                    case Unsigned64:
                    case Signed64:
                    default:
                        sprintf(buf, "%s", op.SymRef->Name.c_str());
                        break;
                }
            }
            else
            {
                switch(this->Attribute)
                {
                    case Unsigned64:
                        sprintf(buf, "0x%llX", (long long)op.UIntValue);
                        break;
                    case Double:
                        sprintf(buf, "%lf", op.DoubleValue);
                        break;
                    case Single:
                        sprintf(buf, "%f", op.SingleValue);
                        break;
                    case Signed64:
                    default:
                        sprintf(buf, "0x%llX", (long long)op.IntValue);
                        break;
                }
            }
            return "$" + std::string(buf);
        case DisaInstruction::Indirect:
            if(op.SymRef == NULL)
            {
                sprintf(buf, "(0x%llX)", (long long)op.Address);
            }
            else
            {
                sprintf(buf, "(%s)", op.SymRef->Name.c_str());
            }
                    return std::string(buf);
        case DisaInstruction::Direct:
            if(op.SymRef == NULL)
            {
                sprintf(buf, "0x%llX", (long long)op.Address);
            }
            else
            {
                sprintf(buf, "%s", op.SymRef->Name.c_str());
            }
                    return std::string(buf);
        case DisaInstruction::BaseDisplacement:
            if(op.SymRef == NULL)
            {
                sprintf(buf, "0x%llX(0x%llX)", (long long)(op.Displacement & 0x00000000FFFFFFFF), (long long)op.Address);
            }
            else
            {
                sprintf(buf, "0x%llX(0x%s)", (long long)(op.Displacement & 0x00000000FFFFFFFF), op.SymRef->Name.c_str());
            }

            return std::string(buf);
        default:
            abort();
    }

}

std::string DisaInstruction::ToString()
{
    std::string opstr;

    switch(OpCode)
    {

        case MOV:
            return
                "mov\t"
                + GetMovOperand(Operands[0])
                + ", "
                + GetMovOperand(Operands[1])
                ;

        case ADD:
            opstr = "add";
            goto arithmatic;

        case SUB:
            opstr = "sub";
            goto arithmatic;

        case MUL:
            opstr = "mul";
            goto arithmatic;

        case DIV:
            opstr = "div";
            goto arithmatic;

        case AND:
            opstr = "and";
            goto arithmatic;

        case OR:
            opstr = "or";
            goto arithmatic;

        case XOR:
            opstr = "xor";
            goto arithmatic;

        arithmatic:
            {
                std:: string suffix;

                switch(Attribute)
                {
                    case DisaInstruction::Unsigned64:
                        suffix = "uq";
                        break;
                    case DisaInstruction::Signed64:
                        suffix = "sq";
                        break;
                    case DisaInstruction::Double:
                        suffix = "fd";
                        break;
                    case DisaInstruction::Single:
                        suffix = "fs";
                        break;

                    default:
                        abort();
                }

                return opstr + ":" + suffix + "\t"
                       + GetOperand(Operands[0])
                       + ", "
                       + GetOperand(Operands[1])
                       + ", "
                       + GetOperand(Operands[2])
                       ;
            }

        case BR:
            switch(this->JumpMode)
            {
                case J:
                    return std::string("br:j ")
                           + (RelativeJump ? "r" : "a") + " \t"
                           + GetOperand(Operands[0]);
                case JI:
                    return std::string("br:ji \t")
                           + GetOperand(Operands[0]);
                case LE:
                    opstr = "br:le";
                    goto conditional_jump;
                case E:
                    opstr = "br:e";
                    goto conditional_jump;
                case NE:
                    opstr = "br:ne";
                    goto conditional_jump;
                case L:
                    opstr = "br:l";
                    goto conditional_jump;
                case SL:
                    opstr = "br:sl";
                    goto conditional_jump;

                case Z:
                    opstr = "br:z";
                    goto conditional_jump2;
                case NZ:
                    opstr = "br:nz";
                    goto conditional_jump2;

                conditional_jump:
                    return opstr + " "
                           + (RelativeJump ? "r" : "a") + " \t"
                           + GetOperand(Operands[0])
                           + ", "
                           + GetOperand(Operands[1])
                           + ", "
                           + GetOperand(Operands[2])
                           ;

                conditional_jump2:
                    return opstr + " "
                           + (RelativeJump ? "r" : "a") + " \t"
                           + GetOperand(Operands[0])
                           + ", "
                           + GetOperand(Operands[1])
                           ;
                default:
                    abort();
            }
        case NEWR:

            return
                "newr\t"
                + GetOperand(Operands[0])
                + ", "
                + GetOperand(Operands[1])
                + ", "
                + GetOperand(Operands[2])
                + ", "
                + GetOperand(Operands[3])
                ;

        case EXIT:
            opstr = std::string("exit");
            switch(this->ExitMode)
            {
                case Commit:
                    return opstr + ":c";
                case Abort:
                    return opstr + ":a";
                case CommitDelete:
                    return opstr + ":cd";
                case AbortDelete:
                    return opstr + ":ad";
                default:
                    abort();
            }
        case INT:
            return "int " + GetOperand(Operands[0]);
        case NOP:
            return "nop";
        case STRCMP:
            return
                "strcmp \t"
                + GetOperand(Operands[0])
                + ", "
                + GetOperand(Operands[1])
                + ", "
                + GetOperand(Operands[2])
                + ", "
                + GetOperand(Operands[3])
                + ", "
                + GetOperand(Operands[4])
                ;
        default:
            abort();
    }
}

int64_t DisaInstruction::GetLength()
{
    int64_t n = 1;
    switch(OpCode)
    {
        case MOV:
            //TODO: Not implemented.
            n += 2;
            n += GetOperandEncodingLength(Operands[0]);
            n += GetOperandEncodingLength(Operands[1]);
            break;
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case AND:
        case OR:
        case XOR:
            n += 2;
            n += GetOperandEncodingLength(Operands[0]);
            n += GetOperandEncodingLength(Operands[1]);
            n += GetOperandEncodingLength(Operands[2]);
            break;
        case BR:
            if(JumpMode == J)
            {
                n += 1;
                n += GetOperandEncodingLength(Operands[0]);
            }
            else if(JumpMode == JI)
            {
                n += 1;
                n += GetOperandEncodingLength(Operands[0]);
            }
            else if(JumpMode == Z || JumpMode == NZ)
            {
                n += 2;
                n += GetOperandEncodingLength(Operands[0]);
                n += GetOperandEncodingLength(Operands[1]);
            }
            else
            {
                n += 2;
                n += GetOperandEncodingLength(Operands[0]);
                n += GetOperandEncodingLength(Operands[1]);
                n += GetOperandEncodingLength(Operands[2]);
            }
            break;
        case NEWR:
            n += 2;
            n += GetOperandEncodingLength(Operands[0]);
            n += GetOperandEncodingLength(Operands[1]);
            n += GetOperandEncodingLength(Operands[2]);
            n += GetOperandEncodingLength(Operands[3]);
            break;
        case EXIT:
            n += 1;
            break;
        case NOP:
            break;
        case INT:
            n += 1;
            break;
        case STRCMP:
            n += 2;
            n += GetOperandEncodingLength(Operands[0]);
            n += GetOperandEncodingLength(Operands[1]);
            n += GetOperandEncodingLength(Operands[2]);
            n += GetOperandEncodingLength(Operands[3]);
            n += GetOperandEncodingLength(Operands[4]);
            break;
        default:
            abort();
    }
    return n;
}

DisaInstruction::DisaInstruction(SymbolScope* scope, std::string fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    VParse(scope, fmt, args);
    va_end(args);
}

void DisaInstruction::Parse(SymbolScope *scope, std::string fmt)
{
    pANTLR3_INPUT_STREAM  input  = antlr3StringStreamNew((pANTLR3_UINT8)fmt.c_str(), ANTLR3_ENC_UTF8, fmt.size(), (pANTLR3_UINT8)"$$__inline_assembly");

    pDisaLexer  lxr  = DisaLexerNew(input);      // CLexerNew is generated by ANTLR
    assert(lxr != NULL);

    pANTLR3_COMMON_TOKEN_STREAM  tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lxr));
    assert(tstream != NULL);

    pDisaParser psr = DisaParserNew(tstream);
    assert(psr != NULL);

    //TODO: multiple instructions
    std::vector<TargetInstruction *> *il = psr->translation_unit(psr, scope );
    DisaInstruction *inst = dynamic_cast<DisaInstruction *>(il->front());
    assert(inst != NULL);

    this->OpCode = inst->OpCode;
    this->Attribute = inst->Attribute;
    this->JumpMode = inst->JumpMode;
    this->RelativeJump = inst->RelativeJump;
    this->ExitMode = inst->ExitMode;

    for(int i = 0; i < MAX_NUM_OPERANDS; i++)
    {
        this->Operands[i] = inst->Operands[i];
    }
}

void DisaInstruction::VParse(SymbolScope *scope,std::string fmt, va_list args)
{
    //TODO: args
    std::string str = fmt;

    Parse(scope, str);
}



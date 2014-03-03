#include "I0Instruction.h"
#include <iostream>
#include <iomanip>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <stdarg.h>
#include <I0Lexer.h>
#include <I0Parser.h>

#undef ADD

I0Instruction::I0Operand::I0Operand()
{
    Resolving = 0;
    AddressingMode = (OperandAddressingMode)0;
    ConvAttribute = (ConvOperandAttribute)0;;
    IntValue = 0;
    UIntValue = 0;
    DoubleValue = 0;
    SingleValue = 0;
    Address = 0;
    Displacement = 0;
    SymRef = NULL;
}

I0Instruction::I0Operand::I0Operand(I0Instruction::OperandAddressingMode addrm, int64_t v, I0Instruction::ConvOperandAttribute convattr)
{
    Resolving = 0;
    AddressingMode = (OperandAddressingMode)0;
    ConvAttribute = (ConvOperandAttribute)0;;
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
    ConvAttribute = convattr;
}

I0Instruction::I0Operand::I0Operand(I0Instruction::OperandAddressingMode addrm, uint64_t v, I0Instruction::ConvOperandAttribute convattr)
{
    Resolving = 0;
    AddressingMode = (OperandAddressingMode)0;
    ConvAttribute = (ConvOperandAttribute)0;;
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
    ConvAttribute = convattr;
}

I0Instruction::I0Operand::I0Operand(I0Instruction::OperandAddressingMode addrm, float v)
{
    Resolving = 0;
    AddressingMode = (OperandAddressingMode)0;
    ConvAttribute = (ConvOperandAttribute)0;;
    IntValue = 0;
    UIntValue = 0;
    DoubleValue = 0;
    SingleValue = 0;
    Address = 0;
    Displacement = 0;
    SymRef = NULL;

    ConvAttribute = I0Instruction::ConvSingle;
    Resolving = false;
    AddressingMode = addrm;
    SingleValue = v;
}

I0Instruction::I0Operand::I0Operand(I0Instruction::OperandAddressingMode addrm, double v)
{
    Resolving = 0;
    AddressingMode = (OperandAddressingMode)0;
    ConvAttribute = (ConvOperandAttribute)0;;
    IntValue = 0;
    UIntValue = 0;
    DoubleValue = 0;
    SingleValue = 0;
    Address = 0;
    Displacement = 0;
    SymRef = NULL;

    ConvAttribute = I0Instruction::ConvDouble;
    Resolving = false;
    AddressingMode = addrm;
    DoubleValue = v;
}

I0Instruction::I0Operand::I0Operand(I0Instruction::OperandAddressingMode addrm, int64_t base, int64_t displacement, I0Instruction::ConvOperandAttribute convattr)
{
    Resolving = 0;
    AddressingMode = (OperandAddressingMode)0;
    ConvAttribute = (ConvOperandAttribute)0;;
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
    ConvAttribute = convattr;
}

I0Instruction::I0Instruction()
{
    AddrSizePrefix = EBM;  // Eightbyte mode is the default one
}

I0Instruction::~I0Instruction()
{

}

int64_t I0Instruction::GetOperandEncodingLength(I0Instruction::I0Operand operand)
{
    int64_t n;
    switch(operand.AddressingMode)
    {
        case Immediate:

            switch(this->OpCode)
            {
                case I0Instruction::CONV:
                    switch(operand.ConvAttribute)
                    {
                        case ConvSigned64:
                        case ConvUnsigned64:
                            n = 8;
                            break;
                        case ConvSigned32:
                        case ConvUnsigned32:
                            n = 4;
                            break;
                        case ConvSigned8:
                        case ConvUnsigned8:
                            n = 1;
                            break;
                        case ConvDouble:
                            n = 8;
                            break;
                        case ConvSingle:
                            n = 4;
                            break;
                        default:
                            abort();
                    }
                    break;
                case I0Instruction::ADD:
                case I0Instruction::SUB:
                case I0Instruction::MUL:
                case I0Instruction::DIV:
                case I0Instruction::AND:
                case I0Instruction::OR:
                case I0Instruction::XOR:
                    // printf("GL:\n");
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
                            printf("GL abort.\n");
                            printf("Attribute: %#lx.\n", this->Attribute);
                            abort();
                    }
                    break;
                case I0Instruction::SPAWN:
                case I0Instruction::SPAWNX:
                case I0Instruction::B:
                case I0Instruction::SCMP:
                case I0Instruction::GREP:
                    n = 8;
                    break;
                case I0Instruction::EXIT:
                case I0Instruction::NOP:
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

char* I0Instruction::EncodeOperandData(char* buffer, I0Instruction::I0Operand operand, I0AddrSizePrefix AddrSizePrefix)
{
    char *p = buffer;

    if (AddrSizePrefix == SBM) {
        fprintf(stderr, "SBM operand encoding is not supported yet.\n");
        abort();
    }
    
    switch(operand.AddressingMode)
    {
        case Immediate:
            switch(this->OpCode)
            {
                case I0Instruction::CONV:
                case I0Instruction::ADD:
                case I0Instruction::SUB:
                case I0Instruction::MUL:
                case I0Instruction::DIV:
                case I0Instruction::AND:
                case I0Instruction::OR:
                case I0Instruction::XOR:
                case I0Instruction::SPAWN:
                case I0Instruction::SPAWNX:
                case I0Instruction::B:
                case I0Instruction::SCMP:
                case I0Instruction::GREP:
                    // NOTE: I do not assume the host machine is little-endian, so I explicitly encode each byte of integers
                    // FIXME: Does floating-point encoding has endianess? I assume no, but I'm not sure.
                    switch(operand.ConvAttribute)
                    {
                        case ConvSigned64:
                            *p++ = (operand.IntValue >> 0x00) & 0xFF;
                            *p++ = (operand.IntValue >> 0x08) & 0xFF;
                            *p++ = (operand.IntValue >> 0x10) & 0xFF;
                            *p++ = (operand.IntValue >> 0x18) & 0xFF;
                            *p++ = (operand.IntValue >> 0x20) & 0xFF;
                            *p++ = (operand.IntValue >> 0x28) & 0xFF;
                            *p++ = (operand.IntValue >> 0x30) & 0xFF;
                            *p++ = (operand.IntValue >> 0x38) & 0xFF;
                            break;
                        case ConvSigned32:
                            *p++ = (operand.IntValue >> 0x00) & 0xFF;
                            *p++ = (operand.IntValue >> 0x08) & 0xFF;
                            *p++ = (operand.IntValue >> 0x10) & 0xFF;
                            *p++ = (operand.IntValue >> 0x18) & 0xFF;
                            break;
                        case ConvSigned8:
                            *p++ = (operand.IntValue >> 0x00) & 0xFF;
                            break;
                        case ConvUnsigned64:
                            *p++ = (operand.UIntValue >> 0x00) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x08) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x10) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x18) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x20) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x28) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x30) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x38) & 0xFF;
                            break;
                        case ConvUnsigned32:
                            *p++ = (operand.UIntValue >> 0x00) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x08) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x10) & 0xFF;
                            *p++ = (operand.UIntValue >> 0x18) & 0xFF;
                            break;
                        case ConvUnsigned8:
                            *p++ = (operand.UIntValue >> 0x00) & 0xFF;
                            break;
                        case ConvDouble:
                            memcpy(p, &operand.DoubleValue, 8);
                            p += 8;
                            break;
                        case ConvSingle:
                            memcpy(p, &operand.SingleValue, 4);
                            p += 4;
                            break;
                        default:
                            abort();
                    }
                    break;
                case I0Instruction::EXIT:
                case I0Instruction::NOP:
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


void I0Instruction::Encode(char* buffer)
{
    char *p = buffer;
    char _p = '\0';

    // bug caused here on g++ 4.8.2
    // In the cc0 source code, there are places uses char *p like this:
    // p++ = p | 0x1;
    // The sequence of operations (p++, and *p) is ambiguous. Check a discussion at: http://pic.dhe.ibm.com/infocenter/tpfhelp/current/index.jsp?topic=%2Fcom.ibm.ztpf-ztpfdf.doc_put.cur%2Fcommon%2Fm1rhoseq.html and also http://en.cppreference.com/w/cpp/language/eval_order .
    // To fix it, I changed code above to:
    // char _p = *p;
    // *p++ = _p | 0x1;
    // to force (*p) to be evaluated first.

    *p = (char)((AddrSizePrefix << 7) & 0x80); // bit 0
    _p = *p;
    *p++ = (int)_p | ((OpCode >> 3) & 0x7F); // bit 1 - 7

    // std::cout << "OpCode: 1st " << OpCode << " -> " << std::hex << (int)(*(p-1)) << std::endl;

    *p = (char)(((OpCode & 0x7) << 5) & 0xF0); // bit 8 - 10

    // std::cout << " ==> 2nd " << std::hex << (int)(*p) << std::endl;

    switch(OpCode)
    {
        case CONV:
            _p = *p;
            // *p++ = (int)*p | (Operands[0].ConvAttribute << 1) | (Operands[1].ConvAttribute >> 3);
            *p++ = _p | (Operands[0].ConvAttribute << 1) | (Operands[1].ConvAttribute >> 3);
            *p++ = (Operands[1].ConvAttribute << 5) | (Operands[0].AddressingMode << 2) | (Operands[1].AddressingMode >> 1);
            *p++ = Operands[1].AddressingMode << 7;
            p = EncodeOperandData(p, Operands[0], AddrSizePrefix);
            p = EncodeOperandData(p, Operands[1], AddrSizePrefix);
            break;
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case AND:
        case OR:
        case XOR:
            // std::cout << " ==> " << OpCode << " -> " << std::hex << (int)(*p) << std::endl;
            // std::cout << " ==> Attribute << 1 " << " -> " << std::hex << (Attribute << 1) << std::endl;
            // std::cout << " ==> Operands[0].AddressingMode >> 2 " << " -> " << (Operands[0].AddressingMode >> 2) << std::endl;

            _p = *p;

            // std::cout << " ==> _p " << OpCode << " -> " << std::hex << (int)_p << std::endl;

            // bug caused here on g++ 4.8.2
            // *p++ = (char)((int)(*p) | (Attribute << 1) | (Operands[0].AddressingMode >> 2));
            *p++ = (char)((int)(_p) | (Attribute << 1) | (Operands[0].AddressingMode >> 2));

            // std::cout << " ==> " << OpCode << " -> " << std::hex << (int)(*(p-1)) << std::endl;

            *p++ = (Operands[0].AddressingMode << 6) | (Operands[1].AddressingMode << 3) | (Operands[2].AddressingMode);
            p = EncodeOperandData(p, Operands[0], AddrSizePrefix);
            p = EncodeOperandData(p, Operands[1], AddrSizePrefix);
            p = EncodeOperandData(p, Operands[2], AddrSizePrefix);
            break;
        case B:
            _p = *p;

            // *p = *p | (JumpMode << 1);
            *p = (char)((int)_p | (JumpMode << 1));

            if(JumpMode == J)
            {
                _p = *p;
                // *p++ = *p | (RelativeJump ? 1 : 0);
                *p++ = (char)((int)_p | (RelativeJump ? 1 : 0));
                p = EncodeOperandData(p, Operands[0], AddrSizePrefix);
            }
            else if(JumpMode == JI)
            {
                _p = *p;
                // *p++ = *p | (Operands[0].AddressingMode >> 2);
                *p++ = (char)((int)_p | (Operands[0].AddressingMode >> 2));
                *p++ = Operands[0].AddressingMode << 6;
                p = EncodeOperandData(p, Operands[0], AddrSizePrefix);
            }
            else if(JumpMode == NE || JumpMode == E || JumpMode == L || JumpMode == LE || JumpMode == SL)
            {
                _p = *p;
                // *p++ = *p | (Attribute >> 3);
                *p++ = (char)((int)_p | (Attribute >> 3));
                *p++ = (Attribute << 5) | (Operands[0].AddressingMode << 2) | (Operands[1].AddressingMode >> 1);
                *p++ = (Operands[1].AddressingMode << 7) | ((RelativeJump ? 1 : 0) << 6);
                p = EncodeOperandData(p, Operands[0], AddrSizePrefix);
                p = EncodeOperandData(p, Operands[1], AddrSizePrefix);
                p = EncodeOperandData(p, Operands[2], AddrSizePrefix);
            }
            else if(JumpMode == NZ || JumpMode == Z)
            {
                _p = *p;
                *p++ = _p | (Attribute >> 3);
                *p++ = (Attribute << 5) | (Operands[0].AddressingMode << 2) | ((RelativeJump ? 1 : 0) << 1);
                p = EncodeOperandData(p, Operands[0], AddrSizePrefix);
                p = EncodeOperandData(p, Operands[1], AddrSizePrefix);
            }
            else
            {
                abort();
            }

            break;
        case SPAWN:
            _p = *p;
            *p++ = _p | (Operands[0].AddressingMode << 2) | (Operands[1].AddressingMode >> 1);
            *p++ = (Operands[2].AddressingMode << 7) | (Operands[2].AddressingMode << 4) | (Operands[3].AddressingMode << 1);
            p = EncodeOperandData(p, Operands[0], AddrSizePrefix);
            p = EncodeOperandData(p, Operands[1], AddrSizePrefix);
            p = EncodeOperandData(p, Operands[2], AddrSizePrefix);
            p = EncodeOperandData(p, Operands[3], AddrSizePrefix);
            break;
        case SPAWNX:
            // TODO: debug here
            _p = *p;
            *p++ = _p | (Operands[0].AddressingMode << 2) | (Operands[1].AddressingMode >> 1);
            *p++ = (Operands[2].AddressingMode << 7) | (Operands[2].AddressingMode << 4) | (Operands[3].AddressingMode << 1) | (Operands[4].AddressingMode >> 2);
            *p++ = Operands[4].AddressingMode << 6;
            p = EncodeOperandData(p, Operands[0], AddrSizePrefix);
            p = EncodeOperandData(p, Operands[1], AddrSizePrefix);
            p = EncodeOperandData(p, Operands[2], AddrSizePrefix);
            p = EncodeOperandData(p, Operands[3], AddrSizePrefix);
            p = EncodeOperandData(p, Operands[4], AddrSizePrefix);
            break;

        case EXIT:
            _p = *p;
            *p++ = _p | (ExitMode << 3);
            break;
        case NOP:
            p++;
            break;
        case INT:
            p++;
            *p++ = (Operands[0].IntValue & 0xFF);
            break;
        case SCMP:
        case GREP:
            _p = *p;
            *p++ = _p | (Operands[0].AddressingMode << 2) | (Operands[1].AddressingMode >> 1);
            *p++ = (Operands[1].AddressingMode << 7) | (Operands[2].AddressingMode << 4) | (Operands[3].AddressingMode << 1) | (Operands[4].AddressingMode >> 2);
            *p++ = (Operands[4].AddressingMode << 6);
            p = EncodeOperandData(p, Operands[0], AddrSizePrefix);
            p = EncodeOperandData(p, Operands[1], AddrSizePrefix);
            p = EncodeOperandData(p, Operands[2], AddrSizePrefix);
            p = EncodeOperandData(p, Operands[3], AddrSizePrefix);
            p = EncodeOperandData(p, Operands[4], AddrSizePrefix);
            break;
        default:
            abort();
    }
}

std::string I0Instruction::GetConvOperand(I0Instruction::I0Operand op)
{
    char buf[50];
    const char *suffix;



    switch(op.AddressingMode)
    {
        case I0Instruction::Immediate:
            if(op.SymRef == NULL)
            {
                switch(op.ConvAttribute)
                {
                    case ConvSigned64:
                        sprintf(buf, "$0x%llX", (long long)(int64_t)op.IntValue);
                        break;
                    case ConvUnsigned64:
                        sprintf(buf, "$0x%llX", (long long)(uint64_t)op.UIntValue);
                        break;
                    case ConvSigned32:
                        sprintf(buf, "$0x%X", (int32_t)op.IntValue);
                        break;
                    case ConvUnsigned32:
                        sprintf(buf, "$0x%X", (uint32_t)op.UIntValue);
                        break;
                    case ConvSigned8 :
                        sprintf(buf, "$0x%X", (int8_t)op.IntValue);
                        break;
                    case ConvUnsigned8:
                        sprintf(buf, "$0x%X", (uint8_t)op.UIntValue);
                        break;
                    case ConvDouble:
                        sprintf(buf, "$%lf", op.DoubleValue);
                        break;
                    case ConvSingle:
                        sprintf(buf, "$%f", op.SingleValue);
                        break;
                    default:
                        abort();
                }
            }
            else
            {
                sprintf(buf, "$%s", op.SymRef->Name.c_str());
            }
            // return "$" + std::string(buf);
            break;
        case I0Instruction::Direct:
            if(op.SymRef == NULL)
            {
                sprintf(buf, "0x%llX", (long long)(int64_t)op.Address);
            }
            else
            {
                sprintf(buf, "%s", op.SymRef->Name.c_str());
            }

            break;
        case I0Instruction::Indirect:
            if(op.SymRef== NULL)
            {
                sprintf(buf, "(0x%llX)", (long long)(int64_t)op.Address);
            }
            else
            {
                sprintf(buf, "(%s)", op.SymRef->Name.c_str());
            }

            break;
        case I0Instruction::BaseDisplacement:
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
    
                switch(op.ConvAttribute)
            {
                case ConvSigned64:
                    return std::string(buf) + ":msq";
                case ConvUnsigned64:
                    return std::string(buf) + ":muq";
                case ConvSigned32:
                    return std::string(buf) + ":msd";
                case ConvUnsigned32:
                    return std::string(buf) + ":mud";
                case ConvSigned8 :
                    return std::string(buf) + ":msb";
                case ConvUnsigned8:
                    return std::string(buf) + ":mub";
                case ConvDouble:
                    return std::string(buf) + ":mfd";
                case ConvSingle:
                    return std::string(buf) + ":mfs";
                default:
                    abort();
            }

}

std::string I0Instruction::GetOperand(I0Instruction::I0Operand op)
{
    char buf[150];
    switch(op.AddressingMode)
    {

        case I0Instruction::Immediate:
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
        case I0Instruction::Indirect:
            if(op.SymRef == NULL)
            {
                sprintf(buf, "(0x%llX)", (long long)op.Address);
            }
            else
            {
                sprintf(buf, "(%s)", op.SymRef->Name.c_str());
            }
                    return std::string(buf);
        case I0Instruction::Direct:
            if(op.SymRef == NULL)
            {
                sprintf(buf, "0x%llX", (long long)op.Address);
            }
            else
            {
                sprintf(buf, "%s", op.SymRef->Name.c_str());
            }
                    return std::string(buf);
        case I0Instruction::BaseDisplacement:
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

// zma
// TODO: format
//
std::string I0Instruction::ToString()
{
    std::string opstr;
    std::string asp;

    if (AddrSizePrefix == EBM) asp = "ebm ";
    if (AddrSizePrefix == SBM) asp = "sbm ";

    switch(OpCode)
    {

        case CONV:
            return
                asp +
                "conv\t"
                + GetConvOperand(Operands[0])
                + ", "
                + GetConvOperand(Operands[1])
                ;

        case ADD:
            opstr = asp + "add";
            goto arithmatic;

        case SUB:
            opstr = asp + "sub";
            goto arithmatic;

        case MUL:
            opstr = asp + "mul";
            goto arithmatic;

        case DIV:
            opstr = asp + "div";
            goto arithmatic;

        case AND:
            opstr = asp + "and";
            goto arithmatic;

        case OR:
            opstr = asp + "or";
            goto arithmatic;

        case XOR:
            opstr = asp + "xor";
            goto arithmatic;

        arithmatic:
            {
                std:: string suffix;

                switch(Attribute)
                {
                    case I0Instruction::Unsigned64:
                        suffix = "ue";
                        break;
                    case I0Instruction::Signed64:
                        suffix = "se";
                        break;
                    case I0Instruction::Double:
                        suffix = "fd";
                        break;
                    case I0Instruction::Single:
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

        case B:
            switch(this->JumpMode)
            {
                case J:
                    return asp + std::string("b:j ")
                           + (RelativeJump ? "r" : "a") + " \t"
                           + GetOperand(Operands[0]);
                case JI:
                    return asp + std::string("b:ij \t")
                           + GetOperand(Operands[0]);
                case LE:
                    opstr = asp + "b:le";
                    goto conditional_jump;
                case E:
                    opstr = asp + "b:e";
                    goto conditional_jump;
                case NE:
                    opstr = asp + "b:ne";
                    goto conditional_jump;
                case L:
                    opstr = asp + "b:l";
                    goto conditional_jump;
                case SL:
                    opstr = asp + "b:sl";
                    goto conditional_jump;

                case Z:
                    opstr = asp + "b:z";
                    goto conditional_jump2;
                case NZ:
                    opstr = asp + "b:nz";
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
        case SPAWN:

            return asp + 
                "spawn\t"
                + GetOperand(Operands[0])
                + ", "
                + GetOperand(Operands[1])
                + ", "
                + GetOperand(Operands[2])
                + ", "
                + GetOperand(Operands[3])
                ;
        case SPAWNX:

            return asp + 
                "spawnx\t"
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

        case EXIT:
            opstr = asp + std::string("exit");
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
            return asp + "int " + GetOperand(Operands[0]);
        case NOP:
            return asp + "nop";
        case SCMP:
            return asp + 
                "scmp \t"
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
        case GREP:
            return asp + 
                "grep \t"
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

int64_t I0Instruction::GetLength()
{
    int64_t n = 0;
    // printf("GetLength\n");
    switch(OpCode)
    {
        case CONV:
            // printf("D\n");
            n += 4;
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
            n += 3;
            // printf("E\n");
            n += GetOperandEncodingLength(Operands[0]);
            // printf("F\n");
            n += GetOperandEncodingLength(Operands[1]);
            // printf("G\n");
            n += GetOperandEncodingLength(Operands[2]);
            // printf("H\n");
            break;
        case B:
            if(JumpMode == J)
            {
                n += 2;
                n += GetOperandEncodingLength(Operands[0]);
            }
            else if(JumpMode == JI)
            {
                n += 3;
                n += GetOperandEncodingLength(Operands[0]);
            }
            else if(JumpMode == Z || JumpMode == NZ)
            {
                n += 3;
                n += GetOperandEncodingLength(Operands[0]);
                n += GetOperandEncodingLength(Operands[1]);
            }
            else
            {
                // L/LE/E/NE/SL/
                n += 4;
                n += GetOperandEncodingLength(Operands[0]);
                n += GetOperandEncodingLength(Operands[1]);
                n += GetOperandEncodingLength(Operands[2]);
            }
            break;
        case SPAWN:
            n += 3;
            n += GetOperandEncodingLength(Operands[0]);
            n += GetOperandEncodingLength(Operands[1]);
            n += GetOperandEncodingLength(Operands[2]);
            n += GetOperandEncodingLength(Operands[3]);
            break;
        case SPAWNX:
            n += 4;
            n += GetOperandEncodingLength(Operands[0]);
            n += GetOperandEncodingLength(Operands[1]);
            n += GetOperandEncodingLength(Operands[2]);
            n += GetOperandEncodingLength(Operands[3]);
            n += GetOperandEncodingLength(Operands[4]);
            break;
        case EXIT:
            n += 2;
            break;
        case NOP:
            n += 2;
            break;
        case INT:
            n += 2;
            // I
            n += 1;
            break;
        case SCMP:
        case GREP:
            n += 4;
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

I0Instruction::I0Instruction(SymbolScope* scope, std::string fmt, ...)
{
    AddrSizePrefix = EBM; // Eightbyte mode is the default one
    va_list args;

    va_start(args, fmt);
    VParse(scope, fmt, args);
    va_end(args);
}

void I0Instruction::Parse(SymbolScope *scope, std::string fmt)
{
    pANTLR3_INPUT_STREAM  input  = antlr3StringStreamNew((pANTLR3_UINT8)fmt.c_str(), ANTLR3_ENC_UTF8, fmt.size(), (pANTLR3_UINT8)"$$__inline_assembly");

    pI0Lexer  lxr  = I0LexerNew(input);      // CLexerNew is generated by ANTLR
    assert(lxr != NULL);

    pANTLR3_COMMON_TOKEN_STREAM  tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lxr));
    assert(tstream != NULL);

    pI0Parser psr = I0ParserNew(tstream);
    assert(psr != NULL);

    //TODO: multiple instructions
    std::vector<TargetInstruction *> *il = psr->translation_unit(psr, scope );
    I0Instruction *inst = dynamic_cast<I0Instruction *>(il->front());
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

void I0Instruction::VParse(SymbolScope *scope,std::string fmt, va_list args)
{
    //TODO: args
    std::string str = fmt;

    Parse(scope, str);
}



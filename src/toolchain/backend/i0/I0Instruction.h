#pragma once

#include <core/Target/TargetInstruction.h>
#include <string>
#include <stdlib.h>
#include <stdarg.h>

class SymbolScope;
class SymbolRef;
class Symbol;

struct I0Instruction: public TargetInstruction
{
public:
    enum I0AddrSizePrefix
    {
        EBM     = 0,
        SBM     = 1
    };

    enum I0Opcode
    {
        NOP     = 0x0,
        CONV    = 0x21,
        ADD     = 0x03,
        SUB     = 0x06,
        MUL     = 0x09,
        DIV     = 0x0C,
        AND     = 0x18,
        OR      = 0x1B,
        XOR     = 0x1E,
        B       = 0x15,
        SPAWN   = 0x25,
        SPAWNX  = 0x29,
        EXIT    = 0x86,
        INT     = 0x24,
        SHIFT   = 0x27,
        SCMP    = 0x2A,
        GREP    = 0x30
    };

    enum ConvOperandAttribute
    {
        ConvSigned8      = 0x0,
        ConvUnsigned8    = 0x4,
        ConvSigned32     = 0x3, // not used
        ConvUnsigned32   = 0x7, // not used
        ConvSigned64     = 0x1,
        ConvUnsigned64   = 0x5,
        ConvSigned128    = 0x2,
        ConvUnsigned128  = 0x6,
        ConvDouble       = 0x9,
        ConvSingle       = 0x8
    };

    enum OperandAttribute
    {
        Signed8      = ConvSigned8     ,
        Unsigned8    = ConvUnsigned8   ,
        Signed32     = ConvSigned32    ,
        Unsigned32   = ConvUnsigned32  ,
        Signed64     = ConvSigned64    ,
        Unsigned64   = ConvUnsigned64  ,
        Signed128    = ConvSigned128   ,
        Unsigned128  = ConvUnsigned128 ,
        Double       = ConvDouble      ,
        Single       = ConvSingle
    };

    enum OperandAddressingMode
    {
        Immediate = 0x0,
        Direct = 0x1,
        Indirect = 0x2,
        BaseDisplacement = 0x3
    };

    enum BJumpMode
    {
        J       = 0x0,
        L       = 0x2,
        LE      = 0x3,
        E       = 0x4,
        NE      = 0x5,
        Z       = 0x6,
        NZ      = 0x7,
        SL      = 0x8,
        JI      = 0xC,
    };

    enum ShiftType
    {
        ST_L      = 0,
        ST_R      = 1
    };

    enum JumpTarget
    {
        JT_A      = 0,
        JT_R      = 1
    };

    enum ExitActionMode
    {
        Commit = 0,
        Abort = 1,
        CommitDelete = 2,
        AbortDelete = 3
    };

public:
    struct I0Operand
    {
    public:
        I0Operand();
        I0Operand(OperandAddressingMode addrm, int64_t v, ConvOperandAttribute convattr);
        I0Operand(OperandAddressingMode addrm, uint64_t v, ConvOperandAttribute convattr);
        I0Operand(OperandAddressingMode addrm, float v);
        I0Operand(OperandAddressingMode addrm, double v);
        I0Operand(OperandAddressingMode addrm, int64_t base, int64_t displacement, ConvOperandAttribute convattr);
    public:
        bool Resolving;
        OperandAddressingMode AddressingMode;
        ConvOperandAttribute ConvAttribute;  // For CONV only
        union {
            int64_t IntValue;           // For Immediate
            uint64_t UIntValue;         // For Immediate
            double DoubleValue;         // For Immediate
            float SingleValue;          // For Immediate

            struct                      // For /Direct/Indirect/Base + Displacement
            {
                int64_t Address;
                int32_t Displacement;   // For Base + Displacement only
            };
        };
        SymbolRef *SymRef;
    };
public:
    static const int MAX_NUM_OPERANDS = 5;      // The SPAWN inctruction 4: The SCMP instruction 5: SPAWNX instruction 5.

public:
    I0AddrSizePrefix AddrSizePrefix;
    I0Opcode OpCode;
    OperandAttribute Attribute;
    BJumpMode JumpMode;
    bool RelativeJump;
    ExitActionMode ExitMode;
    I0Operand Operands[MAX_NUM_OPERANDS];

private:
    std::string GetOperand(I0Instruction::I0Operand op);
    std::string GetOperandAttr(I0Instruction::I0Operand op);
    std::string GetConvOperand(I0Instruction::I0Operand op);
    char * EncodeOperandData(char *buffer, I0Operand operand, I0AddrSizePrefix AddrSizePrefix);
    int64_t GetOperandEncodingLength(I0Operand operand);
    void VParse(SymbolScope *scope, std::string fmt, va_list args);
    void Parse(SymbolScope* scope, std::string fmt);
public:
    I0Instruction();
    I0Instruction(SymbolScope *scope, std::string fmt, ...);
    virtual ~I0Instruction();
public:
    void Encode(char* buffer);
    virtual int64_t GetLength();
    virtual std::string ToString();
};

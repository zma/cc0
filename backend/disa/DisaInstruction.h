#pragma once 

#include <core/Target/TargetInstruction.h>
#include <string>
#include <stdlib.h>
#include <stdarg.h>

class SymbolScope;
class SymbolRef;
class Symbol;

struct DisaInstruction: public TargetInstruction
{
public:
    enum DisaOpcode
    {
        NOP     = 0x0,
        MOV     = 0x0b,
        ADD     = 0x01,
        SUB     = 0x02,
        MUL     = 0x03,    
        DIV     = 0x04,
        AND     = 0x08,
        OR      = 0x09,
        XOR     = 0x0A,
        BR      = 0x07,
        NEWR    = 0x05,
        EXIT    = 0x06,
        INT     = 0x0C,
        STRCMP  = 0x0E
    };

    enum MovOperandAttribute
    {
        MovSigned64     = (0x0 << 2) | 0x0,
        MovUnsigned64   = (0x1 << 2) | 0x0,
        MovSigned32     = (0x0 << 2) | 0x1,
        MovUnsigned32   = (0x1 << 2) | 0x1,
        MovSigned8      = (0x0 << 2) | 0x2,
        MovUnsigned8    = (0x1 << 2) | 0x2,
        MovDouble       = (0x2 << 2) | 0x0,
        MovSingle       = (0x2 << 2) | 0x1
    }; 
    
    enum OperandAttribute
    {
        Signed64 = 0x0,
        Unsigned64 = 0x4,
        Double = 0x01,
        Single = 0x02
    };
    
    enum OperandAddressingMode
    {
        Immediate = 0x0,
        Direct = 0x1,
        Indirect = 0x2,
        BaseDisplacement = 0x3
    };  
    
    enum BrJumpMode
    {
        J       = 0x0,
        JI      = 0x1,
        LE      = 0x2,
        L       = 0x3,
        E       = 0x4,
        NE      = 0x5,
        Z       = 0x6,
        NZ      = 0x7,
        SL      = 0x8
    };    
    
    enum ExitActionMode
    {
        Commit = 0,
        Abort = 1,
        CommitDelete = 2,
        AbortDelete = 3
    };
    
public:
    struct DisaOperand
    {
    public:
        DisaOperand();
        DisaOperand(OperandAddressingMode addrm, int64_t v, MovOperandAttribute movattr);
        DisaOperand(OperandAddressingMode addrm, uint64_t v, MovOperandAttribute movattr);
        DisaOperand(OperandAddressingMode addrm, float v);
        DisaOperand(OperandAddressingMode addrm, double v);
        DisaOperand(OperandAddressingMode addrm, int64_t base, int64_t displacement, MovOperandAttribute movattr);
    public:
        bool Resolving;
        OperandAddressingMode AddressingMode;
        MovOperandAttribute MovAttribute;  // For MOV only
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
    static const int MAX_NUM_OPERANDS = 5;      // The NEWR inctruction 4: The STRCMP instruction 5.
    
public:
    DisaOpcode OpCode;
    OperandAttribute Attribute;
    BrJumpMode JumpMode;
    bool RelativeJump;
    ExitActionMode ExitMode;        
    DisaOperand Operands[MAX_NUM_OPERANDS];
    
private:
    std::string GetOperand(DisaInstruction::DisaOperand op);
    std::string GetMovOperand(DisaInstruction::DisaOperand op);
    char * EncodeOperandData(char *buffer, DisaOperand operand);
    int64_t GetOperandEncodingLength(DisaOperand operand);
    void VParse(SymbolScope *scope, std::string fmt, va_list args);
    void Parse(SymbolScope* scope, std::string fmt);
public:
    DisaInstruction();
    DisaInstruction(SymbolScope *scope, std::string fmt, ...);
    virtual ~DisaInstruction();
public:
    void Encode(char* buffer);
    virtual int64_t GetLength();
    virtual std::string ToString();
};


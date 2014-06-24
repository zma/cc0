#pragma once 

#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <typeinfo>
#include <core/Symbol/SymbolRef.h>
#include <boost/serialization/vector.hpp>

class Type;
class SymbolRef;


class ILVisitor;
class Symbol;
class ConstantValue;

class IL
{
private:
	IL();
	friend class ::boost::serialization::access;
	template<class A>
	void serialize(A& ar, const unsigned int)
	{
		ar & BOOST_SERIALIZATION_NVP(Opcode);
		ar & BOOST_SERIALIZATION_NVP(Operands);
		if(_tags.size())
		{
			::std::cerr<<"IL _tags is not empty!\n";
			exit(-1);
		}
	}
public:
    enum ILOpcode 
    {
        Nop = 0,
        Label,
        Add,            // add op1 = op2 + op3
        And,
        BrEq,           // beq <op1> <op2> <target>
        BrGt,
        BrGe,
        BrLt,
        BrLe,
        BrNe,
        Br,
        BrZ,
        BrNz,
        Call,           // call <target> <ret> <args>
        CallInd,
        CallVirt,
        Cast,
        CompEq,         // ceq op1 = (op2 == op3)
        CompGt,
        CompGe,
        CompLt,
        CompLe,
        CompNe,
        Conv,           // conv op1 = op2
        Cpblk,
        Cpobj,
        Div,
        Initblk,
        Initobj,
        Lda,            // lda op1 = &op2
        Ldelem,         // ldelem op1 = op2[op3]
        Ldelema,        // ldelema op1 = &op2[op3]
        Ldfld,          // ldfld op1 = op2.op3
        Ldflda,         // ldflda op1 = &op2.op3
        Ldind,          // ldi op1 = *op2
        Ldstr,
        Stelem,         // stelem op1[op2] = op3
        Stfld,          // stfld op1.op2 = op3
        Stind,          // sti *op1 = op2
        Stackalloc,
        Sub,
        Mul,
        Neg,
        Newarr,
        Newobj,
        Not,
        Or,
        Mod,
        Ret,
        Shl,
        Shr,
        Mov,            // mov op1  = op2
        Xor,
        Asm
    };
    
    enum ILOperandKind
    {
        Empty = 0,
        Constant,
        Variable,
        TypeToken,
        FieldToken,
    };
    
    enum ILOperandType
    {
        Void = 0,
        I,
        I1,
        I2,
        I4,
        I8,
        U,
        U1,
        U2,
        U4,
        U8,
        R4,
        R8,
        Str,
        Struct,
    };
    struct ILOperand
    {
    private:
    	friend class ::boost::serialization::access;
    	template<class A>
    	void serialize(A& ar, const unsigned int)
    	{
    		ar & BOOST_SERIALIZATION_NVP(OperandKind);
    		ar & BOOST_SERIALIZATION_NVP(OperandType);
    		ar & BOOST_SERIALIZATION_NVP(IValue);
    		ar & BOOST_SERIALIZATION_NVP(UValue);
    		ar & BOOST_SERIALIZATION_NVP(RValue);
    		ar & BOOST_SERIALIZATION_NVP(SValue);
    		ar & BOOST_SERIALIZATION_NVP(SymRef);
    		ar & BOOST_SERIALIZATION_NVP(ObjectType);
    	}
    public:
        ILOperand();
        ILOperand(ILOperandKind kind, ILOperandType type, intmax_t v);
        ILOperand(ILOperandKind kind, ILOperandType type, uintmax_t v);
        ILOperand(ILOperandKind kind, ILOperandType type, double v);
        ILOperand(IL::ILOperandKind kind, IL::ILOperandType type, std::string v);
        ILOperand(IL::ILOperandKind kind, IL::ILOperandType type, Symbol *v);
        ILOperand(IL::ILOperandKind kind, IL::ILOperandType type, SymbolRef *v);
        int Equal(ILOperand *operand);
    public:
        ILOperandKind OperandKind;
        ILOperandType OperandType;
        intmax_t IValue;
        uintmax_t UValue;
        double RValue;
        std::string SValue;
        SymbolRef *SymRef;
        Type *ObjectType;
    };
public:
    ILOpcode Opcode;
    std::vector<ILOperand> Operands;
public:
    IL(ILOpcode opcode);
    IL(ILOpcode opcode, const ILOperand &operand1);
    IL(ILOpcode opcode, const ILOperand &operand1, const ILOperand &operand2);
    IL(ILOpcode opcode, const ILOperand &operand1, const ILOperand &operand2, const ILOperand &operand3);
    IL(ILOpcode opcode, const ILOperand &operand1, const ILOperand &operand2, const ILOperand &operand3, const ILOperand &operand4);
    IL(ILOpcode opcode, const std::vector<ILOperand> &operands);
private:
    std::string GetOperandString(const ILOperand &operand);
    std::map<std::string, void *> _tags;
public:
    std::string ToString();
    
    template <typename T> T* GetTag()
    {
        std::map<std::string, void *>::iterator it = _tags.find(typeid(T).name());
        if (it == _tags.end())
        {
            return NULL;
        }
        else
        {
            void *data = it->second;
            return reinterpret_cast<T *>(data);
        }
    }    
    
    template <typename T> void SetTag(T* value)
    {
        void *data = reinterpret_cast<void *>(value);
        _tags[typeid(T).name()] = data;
    }
public:
    virtual void Accept(ILVisitor *visitor);
};



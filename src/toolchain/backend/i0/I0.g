grammar I0;

options
{
    language    = C;
    backtrack   = true;
    memoize     = true;
    k = 4;
}



@lexer::preincludes
{

}

@lexer::includes
{
    #include <core/Core.h>
    #include <assert.h>
}

@lexer::header
{

}

@lexer::members
{
}


@parser::preincludes
{

}

@parser::includes
{
    #include <core/Core.h>
    #include <backend/i0/I0Instruction.h>
    #include <vector>
    #include <assert.h>
}

@parser::header
{

}


@parser::members
{
    int64_t _i0currentCode;
    int64_t _i0currentData;
    int64_t _i0currentRodata;
    int64_t _i0currentBss;
    SymbolScope *_i0currentScope;
}

translation_unit [SymbolScope *scope] returns [std::vector<TargetInstruction *> *retval]
    @init
    {
        _i0currentScope = $scope;
        $retval = new std::vector<TargetInstruction *>();
    }
    :
    NEWLINE* inst1 = instruction
    {
        if ($inst1.retval != NULL)
            $retval->push_back($inst1.retval);
    }
    (NEWLINE+ inst2 = instruction
    {
        if ($inst2.retval != NULL)
            $retval->push_back($inst2.retval);
    })*
    NEWLINE*
    ;
instruction_or_data returns [I0Instruction *retval]
    : instruction
    {
        $retval = $instruction.retval;
    }
    | data_content
    {
        $retval = NULL;
    }
    ;

instruction returns [I0Instruction *retval]
    : exit_instruction { $retval = $exit_instruction.retval; }
    | mov_instruction  { $retval = $mov_instruction.retval; }
    | arithmatic_instruction  { $retval = $arithmatic_instruction.retval; }
    | br_instruction { $retval = $br_instruction.retval; }
    | newr_instruction { $retval = $newr_instruction.retval; }
    | spawnx_instruction { $retval = $spawnx_instruction.retval; }
    | nop_instruction { $retval = $nop_instruction.retval; }
    | softint_instruction { $retval = $softint_instruction.retval; }
    | strcmp_instruction { $retval = $strcmp_instruction.retval; }
    | grep_instruction { $retval = $grep_instruction.retval; }
    | exp_instruction { $retval = $exp_instruction.retval; }
    ;

//////////////////////////////////////////////////////
// Instructions

exit_instruction returns [I0Instruction *retval]
    : 'exit'
    {
        $retval = new I0Instruction();
        $retval->OpCode = I0Instruction::EXIT;
    } ':'(
        'c'
        {
            $retval->ExitMode = I0Instruction::Commit;
        }
        | 'a'
        {
            $retval->ExitMode = I0Instruction::Abort;
        }
        | 'cd'
        {
            $retval->ExitMode = I0Instruction::CommitDelete;
        }
        | 'ad'
        {
            $retval->ExitMode = I0Instruction::AbortDelete;
        }
    )
    ;

mov_instruction returns [I0Instruction *retval]
    : 'mov' src = mov_operand ',' dest = mov_operand
    {
        $retval = new I0Instruction();
        $retval->OpCode = I0Instruction::CONV;
        $retval->Operands[0] = $src.retval;
        $retval->Operands[1] = $dest.retval;
    }
    ;

arithmatic_instruction returns [I0Instruction *retval]
    : arithmatic_opcode ':' attributes op1=operand ',' op2=operand ',' op3=operand
    {
        $retval = new I0Instruction();
        $retval->OpCode = $arithmatic_opcode.retval;
        $retval->Attribute = $attributes.retval;
        $retval->Operands[0] = $op1.retval;
        $retval->Operands[1] = $op2.retval;
        $retval->Operands[2] = $op3.retval;
    }
    ;

br_instruction returns [I0Instruction *retval]
    : 'br' ':'
    {
        $retval = new I0Instruction();
        $retval->OpCode = I0Instruction::B;
    }
    (
        'j' r1 = br_relative imm1 = immediate_operand
        {
            $retval->JumpMode = I0Instruction::J;
            $retval->Operands[0] = $imm1.retval;
            $retval->RelativeJump = $r1.retval;
        }
        | 'ji' target=operand
        {
            $retval->JumpMode = I0Instruction::JI;
            $retval->Operands[0] = $target.retval;
        }
        | br_mode_compare ',' attr1 = attributes r2=br_relative op1 = operand ',' op2 = operand ',' op3 = immediate_operand
        {
            $retval->Attribute = $attr1.retval;
            $retval->JumpMode = $br_mode_compare.retval;
            $retval->Operands[0] = $op1.retval;
            $retval->Operands[1] = $op2.retval;
            $retval->Operands[2] = $op3.retval;
            $retval->RelativeJump = $r2.retval;
        }
        | br_mode_zero ',' attr2 = attributes r3=br_relative op4 = operand ',' op5 = immediate_operand
        {
            $retval->Attribute = $attr2.retval;
            $retval->JumpMode = $br_mode_zero.retval;
            $retval->Operands[0] = $op4.retval;
            $retval->Operands[1] = $op5.retval;
            $retval->RelativeJump = $r3.retval;
        }
    )
    ;
newr_instruction returns [I0Instruction *retval]
    : 'newr' op1 = operand ',' op2 = operand ',' op3 = operand ',' op4 = operand
    {
        $retval = new I0Instruction();
        $retval->OpCode = I0Instruction::SPAWN;
        $retval->Operands[0] = $op1.retval;
        $retval->Operands[1] = $op2.retval;
        $retval->Operands[2] = $op3.retval;
        $retval->Operands[3] = $op4.retval;
    }
    ;
spawnx_instruction returns [I0Instruction *retval]
    : 'spawnx' op1 = operand ',' op2 = operand ',' op3 = operand ',' op4 = operand ',' op5 = operand
    {
        $retval = new I0Instruction();
        $retval->OpCode = I0Instruction::SPAWNX;
        $retval->Operands[0] = $op1.retval;
        $retval->Operands[1] = $op2.retval;
        $retval->Operands[2] = $op3.retval;
        $retval->Operands[3] = $op4.retval;
        $retval->Operands[4] = $op5.retval;
    }
    ;

nop_instruction returns [I0Instruction *retval]
    : 'nop'
    {
        $retval = new I0Instruction();
        $retval->OpCode = I0Instruction::NOP;
    }
    ;

softint_instruction returns [I0Instruction *retval]
    : 'int' inum = integer_value
    {
        $retval = new I0Instruction();
        $retval->OpCode = I0Instruction::INT;
        $retval->Operands[0] = I0Instruction::I0Operand(I0Instruction::Immediate, (int64_t)inum, I0Instruction::ConvSigned64);
    }
    ;
strcmp_instruction returns [I0Instruction *retval]
    : 'strcmp' op1 = operand ',' op2 = operand ',' op3 = operand ',' op4 = operand ',' op5 = operand
    {
        $retval = new I0Instruction();
        $retval->OpCode = I0Instruction::SCMP;
        $retval->Operands[0] = $op1.retval;
        $retval->Operands[1] = $op2.retval;
        $retval->Operands[2] = $op3.retval;
        $retval->Operands[3] = $op4.retval;
        $retval->Operands[4] = $op5.retval;
    }
    ;
grep_instruction returns [I0Instruction *retval]
    : 'grep' op1 = operand ',' op2 = operand ',' op3 = operand ',' op4 = operand ',' op5 = operand
    {
        $retval = new I0Instruction();
        $retval->OpCode = I0Instruction::GREP;
        $retval->Operands[0] = $op1.retval;
        $retval->Operands[1] = $op2.retval;
        $retval->Operands[2] = $op3.retval;
        $retval->Operands[3] = $op4.retval;
        $retval->Operands[4] = $op5.retval;
    }
    ;
exp_instruction returns [I0Instruction *retval]
    : 'exp' ':'
    {
        $retval = new I0Instruction();
        $retval->OpCode = I0Instruction::EXP;
    }
    (
        'e' ',' attr = attributes op1 = operand ',' op2 = operand
         {
                $retval->ExpBase = I0Instruction::BaseE;
                $retval->Attribute = $attributes.retval;
                $retval->Operands[0] = $op1.retval;
                $retval->Operands[1] = $op2.retval;
         }
    )
    ;
br_mode_compare returns [I0Instruction::BJumpMode retval]
    : 'l' { $retval = I0Instruction::L; }
    | 'le' { $retval = I0Instruction::LE; }
    | 'e' { $retval = I0Instruction::E; }
    | 'ne' { $retval = I0Instruction::NE; }
    | 'sl' { $retval = I0Instruction::SL; }
    ;

br_mode_zero returns [I0Instruction::BJumpMode retval]
    : 'z' { $retval = I0Instruction::Z; }
    | 'nz' { $retval = I0Instruction::NZ; }
    ;

br_relative returns [bool retval]
    : 'a'
    {
        return false;
    }
    | 'r'
    {
        return true;
    }
    ;

arithmatic_opcode returns [I0Instruction::I0Opcode retval]
    : 'add'
    {
        $retval = I0Instruction::ADD;
    }
    | 'sub'
    {
        $retval = I0Instruction::SUB;
    }
    | 'mul'
    {
        $retval = I0Instruction::MUL;
    }
    | 'div'
    {
        $retval = I0Instruction::DIV;
    }
    | 'and'
    {
        $retval = I0Instruction::AND;
    }
    | 'or'
    {
        $retval = I0Instruction::OR;
    }
    | 'xor'
    {
        $retval = I0Instruction::XOR;
    }
    ;

// uq and sq are deprecated
attributes returns [I0Instruction::OperandAttribute retval]
    : 'uq'
    {
        $retval = I0Instruction::Unsigned64;
    }
    | 'sq'
    {
        $retval = I0Instruction::Unsigned64;
    }
    | 'ue'
    {
        $retval = I0Instruction::Unsigned64;
    }
    | 'se'
    {
        $retval = I0Instruction::Unsigned64;
    }
    | 'fd'
    {
        $retval = I0Instruction::Double;
    }
    | 'fs'
    {
        $retval = I0Instruction::Single;
    }
    // TODO: Finish it
    ;

mov_operand returns [I0Instruction::I0Operand retval]
    : op = operand ':' ma = mov_attributes
    {
        $retval = $op.retval;
        $retval.ConvAttribute = $ma.retval;
    }
    ;

// muq and msq are deprecated
mov_attributes returns [I0Instruction::ConvOperandAttribute retval]
    : 'muq'
    {
        return I0Instruction::ConvUnsigned64;
    }
    | 'msq'
    {
        return I0Instruction::ConvSigned64;
    }
    | 'ue'
    {
        return I0Instruction::ConvUnsigned64;
    }
    | 'se'
    {
        return I0Instruction::ConvSigned64;
    }

    // TODO: Finish it
    ;

operand returns [I0Instruction::I0Operand retval]
    : op1 = immediate_operand
    {
        $retval = $op1.retval;
    }
    | op2 = direct_operand
    {
        $retval = $op2.retval;
    }
    | op3 = indirect_operand
    {
        $retval = $op3.retval;
    }
    | op4 = base_disp_operand
    {
        $retval = $op4.retval;
    }
    ;

immediate_operand returns [I0Instruction::I0Operand retval]
    : '$' integer_value
    {
        $retval = I0Instruction::I0Operand(I0Instruction::Immediate, $integer_value.retval, I0Instruction::ConvSigned64);
    }
    | '$' fp_value
    {
        $retval = I0Instruction::I0Operand(I0Instruction::Immediate, $fp_value.retval);
    }
    | '$' symbol_ref
    {
        $retval = I0Instruction::I0Operand(I0Instruction::Immediate, (int64_t)0, I0Instruction::ConvSigned64);
        $retval.SymRef = $symbol_ref.retval;
    }
    ;

direct_operand returns [I0Instruction::I0Operand retval]
    : integer_value
    {
        $retval = I0Instruction::I0Operand(I0Instruction::Direct, $integer_value.retval, I0Instruction::ConvSigned64);
    }
    | symbol_ref
    {
        $retval = I0Instruction::I0Operand(I0Instruction::Direct, (int64_t)0, I0Instruction::ConvSigned64);
        $retval.SymRef = $symbol_ref.retval;
    }
    ;

indirect_operand returns [I0Instruction::I0Operand retval]
    : '(' integer_value ')'
    {
        $retval = I0Instruction::I0Operand(I0Instruction::Indirect, $integer_value.retval, I0Instruction::ConvSigned64);
    }
    | '(' symbol_ref ')'
    {
        $retval = I0Instruction::I0Operand(I0Instruction::Indirect, (int64_t)0, I0Instruction::ConvSigned64);
        $retval.SymRef = $symbol_ref.retval;
    }
    ;

base_disp_operand returns [I0Instruction::I0Operand retval]
    : disp1 = integer_value '(' base1 = integer_value ')'
    {
        $retval = I0Instruction::I0Operand(I0Instruction::BaseDisplacement, $base1.retval, $disp1.retval, I0Instruction::ConvSigned64);
    }
    | disp2 = integer_value '(' base2 = symbol_ref ')'
    {
        $retval = I0Instruction::I0Operand(I0Instruction::BaseDisplacement, 0, $disp2.retval, I0Instruction::ConvSigned64);
        $retval.SymRef = $base2.retval;
    }
    ;

symbol_ref returns [SymbolRef* retval]
    :id = IDENTIFIER
    {
        std::string name = (const char *)($id.text->chars);

        $retval = new SymbolRef(_i0currentScope, name);
    }
    ;
integer_value returns [int64_t retval]
    : DECIMAL_LITERAL
    {
        $retval = atol((const char *)($DECIMAL_LITERAL.text->chars));
    }
    | HEX_LITERAL
    {
        long long v;
        sscanf((const char *)(&($HEX_LITERAL.text->chars[2])), "\%llx", &v);
        $retval = (int64_t)v;
    }
    | OCTAL_LITERAL
    | '-' i1 =integer_value
    {
        $retval = -($i1.retval);
    }
    | '+' i2 = integer_value
    {
        $retval = +($i2.retval);
    }
    ;



fp_value returns [double retval]
    : FLOATING_POINT_LITERAL
    {
        $retval = atof((const char *)($FLOATING_POINT_LITERAL.text->chars));
    }
    | '-' f1 = fp_value
    {
        $retval = -($f1.retval);
    }
    | '+' f2 = fp_value
    {
        $retval = +($f2.retval);
    }
    ;
string_value returns [std::string str]
    : STRING_LITERAL
    ;

data_content returns [I0Instruction *retval]
    : '.byte' (integer_value)+
    | '.short' (integer_value)+
    | '.int' (integer_value)+
    | '.long' (integer_value)+
    ;

IDENTIFIER
    :   LETTER (LETTER|'0'..'9')*
    ;



fragment
LETTER
    : 'A'..'Z'
    |   'a'..'z'
    |   '_'
    ;

CHARACTER_LITERAL
    :   '\'' ( EscapeSequence | ~('\''|'\\') ) '\''
    ;

STRING_LITERAL
    :  '"' STRING_GUTS '"'
    ;

fragment
STRING_GUTS :   ( EscapeSequence | ~('\\'|'"') )* ;

HEX_LITERAL returns [int64_t retval]
    : '0' ('x'|'X') (HexDigit)+ IntegerTypeSuffix? ;

DECIMAL_LITERAL returns [int64_t retval]
    : '0' | ('1'..'9' '0'..'9'*) IntegerTypeSuffix?
    ;

OCTAL_LITERAL : '0' ('0'..'7')+ IntegerTypeSuffix? ;

fragment
HexDigit
    : ('0'..'9'|'a'..'f'|'A'..'F')
    ;

fragment
IntegerTypeSuffix
    :   ('l'|'L')
    |   ('u'|'U')  ('l'|'L')?
    ;

FLOATING_POINT_LITERAL
    :   ('0'..'9')+ '.' ('0'..'9')* Exponent? FloatTypeSuffix?
    |   '.' ('0'..'9')+ Exponent? FloatTypeSuffix?
    |   ('0'..'9')+ ( Exponent FloatTypeSuffix? | FloatTypeSuffix)
    ;

fragment
Exponent : ('e'|'E') ('+'|'-')? ('0'..'9')+ ;

fragment
FloatTypeSuffix : ('f'|'F'|'d'|'D') ;

fragment
EscapeSequence
    :   '\\' ('b'|'t'|'n'|'f'|'r'|'\"'|'\''|'\\')
    |   OctalEscape
    ;

fragment
OctalEscape
    :   '\\' ('0'..'3') ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7')
    ;

fragment
UnicodeEscape
    :   '\\' 'u' HexDigit HexDigit HexDigit HexDigit
    ;

NEWLINE
    : ('\r' | '\n' | '\r\n')
;

WS  :  (' '|'\t'|'\u000C') {$channel=HIDDEN;}
    ;


LINE_COMMENT
    : '#' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
    ;

grammar Disa;

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
    #include <backend/disa/DisaInstruction.h>
    #include <vector>
    #include <assert.h>
}

@parser::header
{

}


@parser::members
{
    int64_t _currentCode;
    int64_t _currentData;
    int64_t _currentRodata;
    int64_t _currentBss;
    SymbolScope *_currentScope;
}

translation_unit [SymbolScope *scope] returns [std::vector<TargetInstruction *> *retval]
    @init
    {
        _currentScope = $scope;
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
instruction_or_data returns [DisaInstruction *retval]
    : instruction
    {
        $retval = $instruction.retval;
    }
    | data_content
    {
        $retval = NULL;
    }
    ;

instruction returns [DisaInstruction *retval]
    : exit_instruction { $retval = $exit_instruction.retval; }
    | mov_instruction  { $retval = $mov_instruction.retval; }
    | arithmatic_instruction  { $retval = $arithmatic_instruction.retval; }
    | br_instruction { $retval = $br_instruction.retval; }
    | newr_instruction { $retval = $newr_instruction.retval; }
    | nop_instruction { $retval = $nop_instruction.retval; }
    | softint_instruction { $retval = $softint_instruction.retval; }
    | strcmp_instruction { $retval = $strcmp_instruction.retval; }
    ;

//////////////////////////////////////////////////////
// Instructions

exit_instruction returns [DisaInstruction *retval]
    : 'exit'
    {
        $retval = new DisaInstruction();
        $retval->OpCode = DisaInstruction::EXIT;
    } ':'( 
        'c'
        {
            $retval->ExitMode = DisaInstruction::Commit;
        }
        | 'a'
        {
            $retval->ExitMode = DisaInstruction::Abort;
        }
        | 'cd'
        {
            $retval->ExitMode = DisaInstruction::CommitDelete;
        }
        | 'ad'
        {
            $retval->ExitMode = DisaInstruction::AbortDelete;
        }
    )        
    ;
    
mov_instruction returns [DisaInstruction *retval]
    : 'mov' src = mov_operand ',' dest = mov_operand
    {
        $retval = new DisaInstruction();
        $retval->OpCode = DisaInstruction::MOV;
        $retval->Operands[0] = $src.retval;
        $retval->Operands[1] = $dest.retval;
    }
    ;

arithmatic_instruction returns [DisaInstruction *retval]
    : arithmatic_opcode ':' attributes op1=operand ',' op2=operand ',' op3=operand
    {
        $retval = new DisaInstruction();
        $retval->OpCode = $arithmatic_opcode.retval;
        $retval->Attribute = $attributes.retval;
        $retval->Operands[0] = $op1.retval;
        $retval->Operands[1] = $op2.retval;
        $retval->Operands[2] = $op3.retval;
    }
    ;

br_instruction returns [DisaInstruction *retval]
    : 'br' ':' 
    {
        $retval = new DisaInstruction();
        $retval->OpCode = DisaInstruction::BR;
    }
    ( 
        'j' r1 = br_relative imm1 = immediate_operand
        {
            $retval->JumpMode = DisaInstruction::J;
            $retval->Operands[0] = $imm1.retval;
            $retval->RelativeJump = $r1.retval;
        }
        | 'ji' target=operand
        {
            $retval->JumpMode = DisaInstruction::JI;
            $retval->Operands[0] = $target.retval;
        }
        | br_mode_compare ':' attr1 = attributes r2=br_relative op1 = operand ',' op2 = operand ',' op3 = immediate_operand
        {
            $retval->Attribute = $attr1.retval;
            $retval->JumpMode = $br_mode_compare.retval;
            $retval->Operands[0] = $op1.retval;
            $retval->Operands[1] = $op2.retval;
            $retval->Operands[2] = $op3.retval;
            $retval->RelativeJump = $r2.retval;
        }
        | br_mode_zero ':' attr2 = attributes r3=br_relative op4 = operand ',' op5 = immediate_operand
        {
            $retval->Attribute = $attr2.retval;
            $retval->JumpMode = $br_mode_zero.retval;
            $retval->Operands[0] = $op4.retval;
            $retval->Operands[1] = $op5.retval;
            $retval->RelativeJump = $r3.retval;
        }
    )
    ;
newr_instruction returns [DisaInstruction *retval]
    : 'newr' op1 = operand ',' op2 = operand ',' op3 = operand ',' op4 = operand
    {
        $retval = new DisaInstruction();
        $retval->OpCode = DisaInstruction::NEWR;
        $retval->Operands[0] = $op1.retval;
        $retval->Operands[1] = $op2.retval;
        $retval->Operands[2] = $op3.retval;
        $retval->Operands[3] = $op4.retval;
    }
    ;

nop_instruction returns [DisaInstruction *retval]
    : 'nop'
    {
        $retval = new DisaInstruction();
        $retval->OpCode = DisaInstruction::NOP;
    }
    ;
    
softint_instruction returns [DisaInstruction *retval]
    : 'int' inum = integer_value
    {
        $retval = new DisaInstruction();
        $retval->OpCode = DisaInstruction::INT;
        $retval->Operands[0] = DisaInstruction::DisaOperand(DisaInstruction::Immediate, (int64_t)inum, DisaInstruction::MovSigned64);
    }
    ;
strcmp_instruction returns [DisaInstruction *retval]
    : 'strcmp' op1 = operand ',' op2 = operand ',' op3 = operand ',' op4 = operand ',' op5 = operand
    {
        $retval = new DisaInstruction();
        $retval->OpCode = DisaInstruction::STRCMP;
        $retval->Operands[0] = $op1.retval;
        $retval->Operands[1] = $op2.retval;
        $retval->Operands[2] = $op3.retval;
        $retval->Operands[3] = $op4.retval;
        $retval->Operands[4] = $op5.retval;
    }
    ;

br_mode_compare returns [DisaInstruction::BrJumpMode retval]
    : 'l' { $retval = DisaInstruction::L; }
    | 'le' { $retval = DisaInstruction::LE; }
    | 'e' { $retval = DisaInstruction::E; }
    | 'ne' { $retval = DisaInstruction::NE; }
    | 'sl' { $retval = DisaInstruction::SL; }
    ;
    
br_mode_zero returns [DisaInstruction::BrJumpMode retval]
    : 'z' { $retval = DisaInstruction::Z; }
    | 'nz' { $retval = DisaInstruction::NZ; }
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
    
arithmatic_opcode returns [DisaInstruction::DisaOpcode retval]
    : 'add'
    {
        $retval = DisaInstruction::ADD;
    }
    | 'sub'
    {
        $retval = DisaInstruction::SUB;
    }
    | 'mul'
    {
        $retval = DisaInstruction::MUL;
    }
    | 'div'
    {
        $retval = DisaInstruction::DIV;
    }
    | 'and'
    {
        $retval = DisaInstruction::AND;
    }
    | 'or'
    {
        $retval = DisaInstruction::OR;
    }
    | 'xor'
    {
        $retval = DisaInstruction::XOR;
    }
    ;
    
attributes returns [DisaInstruction::OperandAttribute retval]
    : 'uq'
    {
        $retval = DisaInstruction::Unsigned64;
    }
    | 'sq'
    {
        $retval = DisaInstruction::Unsigned64;
    }
    // TODO: Finish it
    ;
    
mov_operand returns [DisaInstruction::DisaOperand retval]
    : op = operand ':' ma = mov_attributes
    {
        $retval = $op.retval;
        $retval.MovAttribute = $ma.retval;
    }
    ;
    
mov_attributes returns [DisaInstruction::MovOperandAttribute retval]
    : 'muq'
    {
        return DisaInstruction::MovUnsigned64;
    }
    | 'msq'
    {
        return DisaInstruction::MovSigned64;
    }
    // TODO: Finish it
    ;
    
operand returns [DisaInstruction::DisaOperand retval]
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
    
immediate_operand returns [DisaInstruction::DisaOperand retval]
    : '$' integer_value
    {        
        $retval = DisaInstruction::DisaOperand(DisaInstruction::Immediate, $integer_value.retval, DisaInstruction::MovSigned64);
    }
    | '$' fp_value
    {
        $retval = DisaInstruction::DisaOperand(DisaInstruction::Immediate, $fp_value.retval);
    }
    | '$' symbol_ref
    {
        $retval = DisaInstruction::DisaOperand(DisaInstruction::Immediate, (int64_t)0, DisaInstruction::MovSigned64);
        $retval.SymRef = $symbol_ref.retval;
    }
    ;

direct_operand returns [DisaInstruction::DisaOperand retval]
    : integer_value
    {
        $retval = DisaInstruction::DisaOperand(DisaInstruction::Direct, $integer_value.retval, DisaInstruction::MovSigned64);
    }
    | symbol_ref
    {
        $retval = DisaInstruction::DisaOperand(DisaInstruction::Direct, (int64_t)0, DisaInstruction::MovSigned64);
        $retval.SymRef = $symbol_ref.retval;
    }
    ;

indirect_operand returns [DisaInstruction::DisaOperand retval]
    : '(' integer_value ')'
    {
        $retval = DisaInstruction::DisaOperand(DisaInstruction::Indirect, $integer_value.retval, DisaInstruction::MovSigned64);
    }
    | '(' symbol_ref ')'
    {
        $retval = DisaInstruction::DisaOperand(DisaInstruction::Indirect, (int64_t)0, DisaInstruction::MovSigned64);
        $retval.SymRef = $symbol_ref.retval;
    }
    ;

base_disp_operand returns [DisaInstruction::DisaOperand retval]
    : disp1 = integer_value '(' base1 = integer_value ')'
    {
        $retval = DisaInstruction::DisaOperand(DisaInstruction::BaseDisplacement, $base1.retval, $disp1.retval, DisaInstruction::MovSigned64);
    }    
    | disp2 = integer_value '(' base2 = symbol_ref ')'
    {
        $retval = DisaInstruction::DisaOperand(DisaInstruction::BaseDisplacement, 0, $disp2.retval, DisaInstruction::MovSigned64);
        $retval.SymRef = $base2.retval;
    }
    ;

symbol_ref returns [SymbolRef* retval]
    :id = IDENTIFIER
    {
        std::string name = (const char *)($id.text->chars);

        $retval = new SymbolRef(_currentScope, name);
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

data_content returns [DisaInstruction *retval]
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


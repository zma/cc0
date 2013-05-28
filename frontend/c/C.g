grammar C;

options 
{
    language    = C;
    backtrack	= true;
    memoize	= true;
}

@lexer::preincludes
{

}

@lexer::includes
{
#include <stdlib.h>
#include <core/Core.h>
#ifdef __cplusplus
extern "C" int numErrors;
extern "C" int numWarnings;
#else
extern int numErrors;
extern int numWarnings;
#endif

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
#include <frontend/c/DeclarationTemplate.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" int numErrors;
extern "C" int numWarnings;
#else
extern int numErrors;
extern int numWarnings;
#endif

}

@parser::header
{

}


@parser::members
{
#include <stack>
#include <stdio.h>

// TODO: Make these variables to be the member of CTX, for thread-safety
int numErrors = 0;
int numWarnings = 0;

int DisplayError(pCParser ctx, const char * msg, ...)
{
    va_list arg;
    va_start(arg, msg);

    char buffer[1024];
    vsprintf(buffer, msg, arg);
    fprintf(stderr, "Error at line \%u: \%s\n", LT(0)->getLine(LT(0)), buffer);
    va_end(arg);
    numErrors++;
    exit(1);
}

int DisplayWarning(pCParser ctx, const char * msg, ...)
{
    va_list arg;
    va_start(arg, msg);

    char buffer[1024];
    vsprintf(buffer, msg, arg);
    fprintf(stderr, "Warning at line \%u: \%s\n", LT(0)->getLine(LT(0)), buffer);
    va_end(arg);
    numWarnings++;
}


SymbolScope *currentScope = NULL;

SymbolScope * GetCurrentScope()
{
    return currentScope;
}

void EnterScope(SymbolScope::ScopeKind kind, Expression *expr)
{
    currentScope = new SymbolScope(currentScope, kind, expr);
}

int ExitScope()
{
    currentScope = currentScope->GetParentScope();
}
bool isTypeName(pCParser ctx, const char *name)
{
    //TODO: Check the scope symbol table and determine whether it's an typedef name.
    return false;
}

}

translation_unit returns [ProgramExpression * retval]
    scope
    {
        ProgramExpression *program;
    }
    @init
    {
        $translation_unit::program = new ProgramExpression();
        EnterScope(SymbolScope::Global, $translation_unit::program);
    }
    @after
    {
        $retval = $translation_unit::program;    
        ExitScope();
    }
    : (
//    	( ds = declaration_specifiers declarator[new DeclarationTemplate($ds.retval)] declaration* '{' )=> (func = function_definition)
        func = function_definition
    	{
            FunctionExpression *funcExpr = $func.retval;
            $translation_unit::program->GetFunctions()->push_back(funcExpr);    
    	}
    	| d = declaration
    	{
            std::vector<Declaration *> *decls = $d.retval;
            for (int i = 0; i < decls->size(); i++)
            {
                Declaration *paramDecl = (*decls)[i];
                Type *type = paramDecl->GetType();
                type->SetSpecifiers((Type::TypeSpecifier)(type->GetSpecifiers() | Type::ForwardDecl));
                if (paramDecl->GetName() != "")
                {
                    Symbol *sym = new Symbol(paramDecl->GetName(), type, Symbol::ObjectName);
                    GetCurrentScope()->Add(sym);
                }
            }     	

    	}
    )*
    ;

function_definition returns [FunctionExpression* retval]
    scope
    {
        Type *baseType;
        FunctionExpression *func;        
    }
    : ds = declaration_specifiers
    {
        $function_definition::baseType = $ds.retval;
    }
    decl = declarator[new DeclarationTemplate($function_definition::baseType)]
    {
        std::string name = $decl.retval->GetName();
        FunctionType *type = dynamic_cast<FunctionType *>($decl.retval->GetType());

        Symbol *sym = new Symbol(name, type, Symbol::ObjectName);
        GetCurrentScope()->Add(sym);

        EnterScope(SymbolScope::Function, NULL);

        $function_definition::func = new FunctionExpression(sym, GetCurrentScope());
        
        GetCurrentScope()->SetAssociatedExpression($function_definition::func);

        for (std::vector<Declaration *>::iterator it = type->GetParameters()->begin();
                it != type->GetParameters()->end();
                ++it
        )
        {
            std::string paramName = (*it)->GetName();
            Type *paramType = (*it)->GetType();
            if (paramName != "")
            {
                Symbol *symbol = new Symbol(paramName, paramType, Symbol::ObjectName);
                if (!GetCurrentScope()->Add(symbol))
                {
                    char buffer[512];
                    snprintf(buffer, 512, "\'\%s\' is already declared in this scope", paramName.c_str());
                    DisplayError(CTX, buffer);
                }
            }
        }
    }
    stmt = compound_statement
    {
        BlockExpression *body = $stmt.retval;
        $function_definition::func->SetBody(body);
        $retval = $function_definition::func;
        ExitScope();        
    }
    ;

declaration returns [std::vector<Declaration *> *retval]
    scope
    {
        Type *baseType;
    }
    : 
    'typedef' ds = declaration_specifiers? init_declarator_list[$ds.retval] ';'
    {
        DisplayError(CTX, "Unsupported feature: typedef.");
    }
    | ds = declaration_specifiers
    {
    	$declaration::baseType = $ds.retval;
        if ($declaration::baseType == NULL) 
        {
            DisplayError(CTX, "Missing type name.");
        }
    }
    (   idl = init_declarator_list[$declaration::baseType] ';'
        {
            $retval = $idl.retval;
        }
        | ';'
        {
            $retval = new std::vector<Declaration *>();
        }
    )
    ;

declaration_specifiers returns [Type * retval]
    scope
    {
        Type::TypeSpecifier specifiers;
    }
    @init
    {
        $retval = NULL;
        $declaration_specifiers::specifiers = Type::None;
    }
    @after
    {
        if ($retval != NULL)
        {
            $retval->SetSpecifiers($declaration_specifiers::specifiers);
        }
    }
    :   
    (
        scs = storage_class_specifier
        {
            $declaration_specifiers::specifiers = (Type::TypeSpecifier)($declaration_specifiers::specifiers | $scs.retval);
        }
        | ts = type_specifier
        {
            if ($retval != NULL)
            {
                DisplayError(CTX, "More than one type specifiers appears.");
            }
            $retval = $ts.retval;
        }
        | tq = type_qualifier
        {
            $declaration_specifiers::specifiers = (Type::TypeSpecifier)($declaration_specifiers::specifiers| $tq.retval);
        }
    )+
    ;

init_declarator_list [Type *baseType] returns [std::vector<Declaration *> *retval]
    scope
    {
        Type *type;
    }
    @init
    {
        $init_declarator_list::type = $baseType;        
        $retval = new std::vector<Declaration *>();
    }
    : idecl1 = init_declarator[$init_declarator_list::type]
    {
        $retval->push_back($idecl1.retval);
    }
    (
    	',' idecl2 = init_declarator[$init_declarator_list::type]
    	{
            $retval->push_back($idecl2.retval);
    	}
    )*
    ;

init_declarator [Type *baseType] returns [Declaration *retval]
    scope
    {
        Type *type;
    }
    @init
    {
       $init_declarator::type= $baseType;
    }
    : 
    declarator[new DeclarationTemplate($init_declarator::type)] '=' initializer 
    { 
        //TODO: Initializer
    }
    | decl = declarator[new DeclarationTemplate($init_declarator::type)]
    {
        $retval = new Declaration($decl.retval->GetName(), $decl.retval->GetType());
    }
    ;

storage_class_specifier returns [Type::TypeSpecifier retval]
    : 'extern'
    {
        $retval = Type::Extern;
    }
    | 'static'
    {
        $retval = Type::Static;
    }
    | 'auto' 
    {
        $retval = Type::None;
    }
    | 'register'
    {
        $retval = Type::Register;
    }
    | 'standalone'
    {
        $retval = Type::Standalone;
    }
    ;

type_specifier returns [Type* retval]
    : 'void'
    {
        $retval = new VoidType();
    }
    | 'bool' 
    {
        $retval = new BooleanType();
    }
    | 'char'
    {
        $retval = new IntegerType(1, true);
    }
    | 'float' 
    {
        $retval = new FloatingPointType(4);
    }
    | 'double'
    {
        $retval = new FloatingPointType(8);
    }
    | 'signed' 'short'
    {
        $retval = new IntegerType(2, true);
    }
    | 'unsigned' 'short'
    {
        $retval = new IntegerType(2, false);
    }
    | 'signed' 'int'
    {
        $retval = new IntegerType(4, true);
    }
    | 'unsigned' 'int'
    {
        $retval = new IntegerType(4, false);
    }
    | 'signed' 'long'
    {
        $retval = new IntegerType(8, true);
    }
    | 'unsigned' 'long'
    {
        $retval = new IntegerType(8, false);
    } 
    | 'short'
    {
        $retval = new IntegerType(2, true);
    }
    | 'int' 
    {
        $retval = new IntegerType(4, true);
    }
    | 'long' 
    {
        $retval = new IntegerType(8, true);
    }
    | ss = struct_or_union_specifier
    {
        $retval  = $ss.retval;
    }
    | enum_specifier
    {
        DisplayError(CTX, "Not implement: enum");
    }
    | type_id
    {
        DisplayError(CTX, "Not implement: type_id as type_specifier");
    }
    ;

type_id returns [Type *retval]
    :   {isTypeName(ctx, (const char *)(LT(1)->getText(LT(1))->chars)) }? IDENTIFIER
    ;

struct_or_union_specifier returns [Type * retval]
    scope
    {
        StructType::LayoutMode layout;
        StructType *type;
        int64_t offset;
    }
    @init
    {
        $struct_or_union_specifier::type = NULL;
        $struct_or_union_specifier::offset = 0;
    }
    : 
    lt = struct_or_union 
    {
        $struct_or_union_specifier::layout = $lt.retval;
    }
    (
        (
            id1 = IDENTIFIER
            {
                $struct_or_union_specifier::type = new StructType($struct_or_union_specifier::layout, (const char *)($id1.text->chars));
            }
        )?
        {
            if ($struct_or_union_specifier::type == NULL)
            {
                $struct_or_union_specifier::type = new StructType($struct_or_union_specifier::layout);
            }
        }        
        '{'
        {
            // If it is a named struct, define a type symbol;
            if ($struct_or_union_specifier::type->GetName().size() != 0)
            {
                //FIXME: differenciate type name and variable name.
                Symbol *sym = new Symbol($struct_or_union_specifier::type->GetName(), $struct_or_union_specifier::type, Symbol::TypeName);
                GetCurrentScope()->Add(sym);
            }
        }
        (
            sd = struct_declaration
            {
                std::vector<Declaration *> *declList = $sd.retval;
                for (std::vector<Declaration *>::iterator it = declList->begin(); it != declList->end(); ++it)
                {
                    Declaration *decl = *it;
                    decl->SetOffset($struct_or_union_specifier::offset);
                    switch ($struct_or_union_specifier::layout)
                    {
                        case StructType::Sequential:
                            $struct_or_union_specifier::offset += decl->GetType()->GetSize();
                            $struct_or_union_specifier::offset = ROUND_UP($struct_or_union_specifier::offset, MAX(decl->GetType()->GetSize(), PointerType::PointerSize));
                            break;
                        case StructType::Union:
                            $struct_or_union_specifier::offset = 0;
                            break;
                        default:
                            abort();
                    }
                    $struct_or_union_specifier::type->GetFieldList()->push_back(*it);
                }
            }
        )+ 
        '}'
        | id2 = IDENTIFIER
        {
            // Referencing existing struct type
            Symbol *sym = GetCurrentScope()->Lookup((const char *)($id2.text->chars));
            $struct_or_union_specifier::type = dynamic_cast<StructType *>(sym->DeclType);            
        }
    )
    {
        $retval = $struct_or_union_specifier::type;
    }
    ;

struct_or_union returns [StructType::LayoutMode retval]
    : 'struct' { $retval = StructType::Sequential; }
    | 'union' { $retval = StructType::Union; }
    ;

struct_declaration returns [std::vector<Declaration *> *retval]
    scope
    {
        Type *baseType;
    }
    : ql = specifier_qualifier_list 
    {
        $struct_declaration::baseType = $ql.retval;
    }
    dl = struct_declarator_list[$struct_declaration::baseType]
    {
        $retval = $dl.retval;
    }
    ';'
    ;

specifier_qualifier_list returns [Type *retval]
    @init
    {
        $retval = NULL;
    }
    :   
    (
        ts = type_specifier
        {
            if ($retval != NULL)
            {
                DisplayError(CTX, "More than one field type specifier appears.");
            }
            $retval = $ts.retval;
        }
        | type_qualifier
        {
        }
    )+
    ;

struct_declarator_list [Type *baseType] returns [std::vector<Declaration*> *retval]
    scope
    {
        Type *type;
    }
    @init
    {
        $struct_declarator_list::type = $baseType;
    }
    : d1 = struct_declarator[$struct_declarator_list::type]
    {
        $retval = new std::vector<Declaration *>();
        $retval->push_back($d1.retval);
    }
    (
        ',' d2 = struct_declarator[$struct_declarator_list::type]
        {
            $retval->push_back($d2.retval);
        }
    )*
    ;

struct_declarator [Type *baseType] returns [Declaration* retval]
    scope
    {
        Type *type;
    }
    @init
    {
        $struct_declarator::type = $baseType;
    }
    : d = declarator[new DeclarationTemplate($struct_declarator::type)] 
    {
        $retval = new Declaration($d.retval->GetName(), $d.retval->GetType());
    }
    |  declarator[new DeclarationTemplate($struct_declarator::type)] ':' constant_expression 
    {
        DisplayError(CTX, "Not supported feature: bit-field");
    }
    ;

enum_specifier
    options { k=3; }
    : 'enum' '{' enumerator_list '}'
    | 'enum' IDENTIFIER '{' enumerator_list '}'
    | 'enum' IDENTIFIER
    ;

enumerator_list
    : enumerator (',' enumerator)*
    ;

enumerator
    : IDENTIFIER ('=' constant_expression)?
    ;

type_qualifier returns [Type::TypeSpecifier retval]
    : ('const'
    {
        $retval = Type::Const;
    }

    | 'volatile')
    {
        $retval = Type::Volatile;
    }
    ;

declarator [DeclarationTemplate *baseTemplate] returns [DeclarationTemplate* retval]
    scope
    {
        DeclarationTemplate *declTemplate;
    }
    @init
    {
        $declarator::declTemplate = $baseTemplate;
    }
    : pt1 = pointer[$declarator::declTemplate] 
    {
        $declarator::declTemplate = $pt1.retval;
    }
    dd1 = direct_declarator[$declarator::declTemplate]
    {
        $declarator::declTemplate = $dd1.retval;
        $retval = $declarator::declTemplate;
    }
    | dd2 = direct_declarator[$declarator::declTemplate]
    {
        $retval = $dd2.retval;
    }
    | pt3 = pointer[$declarator::declTemplate]
    {
    	DisplayError(CTX, "What's this? declarator->pointer.");
    }
    ;

direct_declarator [DeclarationTemplate *baseTemplate] returns [DeclarationTemplate* retval]
    : 
    (
        id = IDENTIFIER
        {
            $retval = $baseTemplate;
            $retval->SetName(std::string((const char *)($id.text->chars)));
        }
        | '(' d = declarator[$baseTemplate] ')'
        {
            $retval = new DeclarationTemplate($baseTemplate);
        }
    )
    (suffix = declarator_suffix
    {
        DeclarationTemplate::DeclarationSpecifier spec = $suffix.retval;
        $retval->GetSuffixList()->push_back(spec);
    })*
    ;

declarator_suffix returns [DeclarationTemplate::DeclarationSpecifier retval]
    :   '[' ce = constant_expression ']'     // Array
    {
    	$retval.Kind = DeclarationTemplate::Array;
        $retval.ArraySize = $ce.retval;
    }
    |   '[' ']'                         // Array without length
    {
        $retval.Kind = DeclarationTemplate::Array;
        $retval.ArraySize = NULL;
    }
    |   '(' pl = parameter_type_list ')'     // Function parameters
    {
        $retval.Kind = DeclarationTemplate::Function;
        $retval.Parameters = $pl.retval;
    }
    |   '(' identifier_list ')'         // TODO: what's this?
    {
    	DisplayError(CTX, "[FIXME} What's this??");
    }
    |   '(' ')'                         // Function without parameters
    {
        $retval.Kind = DeclarationTemplate::Function;
        $retval.Parameters = new std::vector<Declaration *>();
    }
    ;

pointer [DeclarationTemplate* baseTemplate] returns [DeclarationTemplate* retval]
    scope
    {
        DeclarationTemplate *declTemplate;
    }
    @init
    {
        $pointer::declTemplate = $baseTemplate;
    }
    : (
        tq = type_qualifier
        {
            //TODO
        }
    )* 
    '*'
    {
        $retval = $pointer::declTemplate;
    }
    (
        p = pointer[$pointer::declTemplate]
        { 
            $retval = $p.retval;
        }
    )?
    {      
        DeclarationTemplate::DeclarationSpecifier spec;
        spec.Kind = DeclarationTemplate::Pointer;
        $retval->GetPointerList()->push_back(spec);
    }
    ;

parameter_type_list returns [std::vector<Declaration *> *retval]
    : pl = parameter_list 
    {
    	$retval = $pl.retval;
    }
    | parameter_list ',' '...'
    {
    	DisplayError(CTX, "Not supported feature: varadic function.");
    }       
    ;

parameter_list returns [std::vector<Declaration *> *retval]
    : pd1 = parameter_declaration
    {
        $retval = new std::vector<Declaration *>();
    	$retval->push_back($pd1.retval);
    }
    (
    	',' pd2 = parameter_declaration
    	{
            $retval->push_back($pd2.retval);    
    	}
    )*
    ;

parameter_declaration returns [Declaration *retval]
    scope 
    {
        Type *type;
    }
    : ds = declaration_specifiers 
    {
        $parameter_declaration::type = $ds.retval;
    }
    (
    	d = declarator[new DeclarationTemplate($parameter_declaration::type)]
    	{
            $retval = new Declaration($d.retval->GetName(), $d.retval->GetType());
    	}
        | ad = abstract_declarator[new DeclarationTemplate($parameter_declaration::type)]
    	{
            $retval = new Declaration("", $ad.retval->GetType());
    	}
    )
    ;

identifier_list returns [std::vector<std::string> *retval]
    : id1 = IDENTIFIER 
    {
        $retval = new std::vector<std::string>();
    	$retval->push_back((const char *)($id1.text->chars));
    }
    (',' id2 =IDENTIFIER
    	{
    		$retval->push_back((const char *)($id2.text->chars));
	}
    )*
    ;

type_name returns [Type *retval]
    scope
    {
        Type *type;
    }
    : sql = specifier_qualifier_list 
    {
    	$type_name::type = $sql.retval;
        $retval = $type_name::type;
    }
    (
    	ad = abstract_declarator[new DeclarationTemplate($type_name::type)]
    	{
            $type_name::type = $ad.retval->GetType();
            $retval = $type_name::type;
    	}
    )?
    ;

abstract_declarator [DeclarationTemplate *baseTemplate] returns [DeclarationTemplate *retval]
    scope
    {
        DeclarationTemplate *declTemplate;
    }
    @init
    {
        $abstract_declarator::declTemplate = $baseTemplate;
    }
    : pt1 = pointer[$abstract_declarator::declTemplate] 
    {
        $abstract_declarator::declTemplate = $pt1.retval;
    }
    dd1 = direct_abstract_declarator[$abstract_declarator::declTemplate]
    {
        $retval = $dd1.retval;
    }
    | pt2 = pointer[$abstract_declarator::declTemplate]
    {
        $retval = $pt2.retval;
    }
    | dd3 = direct_abstract_declarator[$abstract_declarator::declTemplate]
    {
        $retval = $dd3.retval;
    }
    ;

direct_abstract_declarator [DeclarationTemplate *baseTemplate] returns [DeclarationTemplate *retval]
    scope
    {
        DeclarationTemplate *declTemplate;
        Expression *parent;
    }
    @init
    {
        $direct_abstract_declarator::declTemplate = $baseTemplate;
        $retval = $direct_abstract_declarator::declTemplate;
    }
    : 
    ( 
    	'(' abstract_declarator[$direct_abstract_declarator::declTemplate] ')' 
        {
            $retval = new DeclarationTemplate($direct_abstract_declarator::declTemplate);
        }
        | suffix1 = abstract_declarator_suffix
        {
            DeclarationTemplate::DeclarationSpecifier spec = $suffix1.retval;
            $direct_abstract_declarator::declTemplate->GetSuffixList()->push_back(spec);

            $retval = $direct_abstract_declarator::declTemplate;
        }
    )
    (suffix2 = abstract_declarator_suffix
    {
        DeclarationTemplate::DeclarationSpecifier spec = $suffix2.retval;
        $direct_abstract_declarator::declTemplate->GetSuffixList()->push_back(spec);
    })*
    ;

abstract_declarator_suffix returns [DeclarationTemplate::DeclarationSpecifier retval]
    :	'[' ']'
    {
        $retval.Kind = DeclarationTemplate::Array;
        $retval.ArraySize = NULL;
    }
    |	'[' ce = constant_expression ']'
    {
        $retval.Kind = DeclarationTemplate::Array;
        $retval.ArraySize = $ce.retval;
    }
    |	'(' ')'
    {
        $retval.Kind = DeclarationTemplate::Function;
        $retval.Parameters = new std::vector<Declaration *>();
    }
    |	'(' pl = parameter_type_list ')'
    {
        $retval.Kind = DeclarationTemplate::Function;
        $retval.Parameters = $pl.retval;
    }
    ;
    
initializer
    @init
    {
        DisplayError(CTX, "Unsupported feature: initialzier.");        
    }
    : assignment_expression
    | '{' initializer_list ','? '}'
    ;

initializer_list
    : initializer (',' initializer)*
    ;

// Expressions

argument_expression_list returns [std::vector<Expression *> *retval]
    @init
    {
        $retval = new std::vector<Expression *>();
    }
    : e1 = assignment_expression
    {
        $retval->push_back($e1.retval);
    }
    (
        ',' e2 = assignment_expression
        {
            $retval->push_back($e2.retval);
        }
    )*
    ;

additive_expression returns [Expression *retval]
    : (e1 = multiplicative_expression)
    {
        $retval = $e1.retval;
    }
    (
        '+' e2=multiplicative_expression 
        {
            $retval = new BinaryExpression(Expression::Add, $retval, $e2.retval);
        }
        | '-' e3=multiplicative_expression
        {
            $retval = new BinaryExpression(Expression::Subtract, $retval, $e3.retval);
        }
    )*
    ;

multiplicative_expression returns [Expression *retval]
    : (e1 = cast_expression) 
    {
        $retval = $e1.retval;
    }
    (
        '*' e2 = cast_expression 
        {
            $retval = new BinaryExpression(Expression::Multiply, $retval, $e2.retval);
        }
        | '/' e3 = cast_expression 
        {
            $retval = new BinaryExpression(Expression::Divide, $retval, $e3.retval);
        }
        | '%' e4 = cast_expression
        {
            $retval = new BinaryExpression(Expression::Modulo, $retval, $e4.retval);
        }
    )*
    ;

cast_expression returns [Expression *retval]
    : '(' t1 = type_name ')' e1 = cast_expression
    {
        $retval = new ConvertExpression($t1.retval, $e1.retval);
    }
    | e2 = unary_expression
    {
        $retval = $e2.retval;
    }
    ;

unary_expression returns [Expression *retval]
    : e1 = postfix_expression
    {
        $retval = $e1.retval;
    }
//    | '++' e2 = unary_expression
//    {
//       $retval = new UnaryExpression(Expression::PrefixIncrement, $e2.retval);           
//    }
//    | '--' e3 = unary_expression
//    {
//       $retval = new UnaryExpression(Expression::PrefixDecrement, $e3.retval);
//    }
    | o4 = unary_operator e4 = cast_expression
    {
       $retval = new UnaryExpression($o4.retval, $e4.retval);
    }
    | 'sizeof' e5 = unary_expression
    {
        DisplayError(CTX, "Not implemented: sizeof");
    }
    | 'sizeof' '(' t6 = type_name ')'
    {
        DisplayError(CTX, "Not implemented: sizeof");
    }
    ;

postfix_expression returns [Expression *retval]
    : pe = primary_expression
    {
        $retval = $pe.retval;
    }
    (        
    '[' e1 = expression ']'
    {
        $retval = new IndexExpression($retval, $e1.retval);
    }
    | '[' e2l = expression ',' ',' e2u = expression ']'
    {
        $retval = new ArraySegmentExpression($retval, $e2l.retval, $e2u.retval);
    }
    | '(' ')'
    {
        $retval = new CallExpression($retval);        
    }
    | '(' e4 = argument_expression_list ')'
    {
        $retval = new CallExpression($retval, $e4.retval);        
    }
    | '.' id5 = IDENTIFIER
    {
        $retval = new MemberExpression($retval, (const char *)($id5.text->chars));
    }
//    | '*' id6 = IDENTIFIER
//    {
//        DisplayError(CTX, "Not implemented: dereference");
//    }
    | '->' id7 =IDENTIFIER
    {
        $retval = new MemberExpression(new UnaryExpression(Expression::Dereference, $retval), (const char *)($id7.text->chars));
    }
//    | '++'
//    {
//    $retval = new UnaryExpression(Expression::PostfixIncrement, $retval);
//    }
//    | '--'           
//    {
//        $retval = new UnaryExpression(Expression::PostfixDecrement, $retval);
//    }
    )*
    ;

unary_operator returns [Expression::ExpressionAction retval]
    : '&'
    {
        $retval = Expression::Reference;
    }
    | '*'
    {
        $retval = Expression::Dereference;
    }
    | '+'
    {
        $retval = Expression::Plus;
    }
    | '-'
    {
        $retval = Expression::Minus;
    }
    | '~'
    {
        $retval = Expression::Not;
    }
    | '!'
    {
        $retval = Expression::Not;
    }
    ;

primary_expression returns [Expression *retval]
    : id1 = IDENTIFIER
    {
        std::string name = std::string((const char *)($id1.text->chars));
        SymbolScope *scope = GetCurrentScope();
        Symbol *symbol = scope->Lookup(name);
        if (symbol == NULL)
        {
            DisplayError(CTX, "\%s is not declared in this scope.", name.c_str()); 
        }
        else
        {
            $retval = new VariableExpression(symbol);        
        }
    }
    | e2 = constant
    {
        $retval = new ConstantExpression($e2.retval);
    }
    | '(' e3 = expression ')'
    {
        $retval = $e3.retval;
    }
    ;

constant returns [ConstantValue *retval]
    :   HEX_LITERAL
    {
        long long v;
        sscanf((const char *)(&($HEX_LITERAL.text->chars[2])), "\%llx", &v);
        $retval = new ConstantValue((int64_t)v);
    }
    |   OCTAL_LITERAL
    {
    }
    |   DECIMAL_LITERAL
    {
        int64_t v = (int64_t)(atoll((const char *)($DECIMAL_LITERAL.text->chars)));
        $retval = new ConstantValue(v);
    }
    |   CHARACTER_LITERAL
    {
        char v = (char)($CHARACTER_LITERAL.text->chars[1]);
        $retval = new ConstantValue(v);
    }
    |   STRING_LITERAL
    {
    }
    |   FLOATING_POINT_LITERAL
    {
        double v = (double)(atof((const char *)($FLOATING_POINT_LITERAL.text->chars)));
        $retval = new ConstantValue(v);
    }
    |   'true'
    {
        $retval = new ConstantValue((bool)true);;
    }
    |   'false'
    {
        $retval = new ConstantValue((bool)false);;
    }
    ;

expression returns [Expression *retval]
    scope
    {
        BlockExpression *block;
    }
    @init
    {
        $expression::block = NULL;
    }
    : e1 = assignment_expression 
    {
        $retval = $e1.retval;
    }
    (
        ',' e2 = assignment_expression
        {
            if ($expression::block == NULL)
            {
                $expression::block = new BlockExpression();
                $expression::block->GetExpressionList()->push_back($retval);
            }
            
            $expression::block->GetExpressionList()->push_back($e2.retval);
            $retval = $expression::block;
        }
    )*
    ;

constant_expression returns [Expression *retval]
    : e1= conditional_expression
    {
        $retval = $e1.retval;
    }
    ;

assignment_expression returns [Expression *retval]
    : e1l = lvalue o1 = assignment_operator e1r = assignment_expression
    {
        $retval = new AssignExpression($e1l.retval, $e1r.retval, $o1.retval);
    }
    | e2 = conditional_expression
    {
        $retval = $e2.retval;
    }
    ;
	
lvalue returns [Expression *retval]
    :	e1 = unary_expression
    {
        $retval = $e1.retval;
    }
    ;

assignment_operator returns [Expression::ExpressionAction retval]
    : '='
    {
        $retval = Expression::Nop;
    }
    | '*='
    {
        $retval = Expression::Multiply;
    }
    | '/='
    {
        $retval = Expression::Divide;
    }
    | '%='
    {
        $retval = Expression::Modulo;
    }
    | '+='
    {
        $retval = Expression::Add;
    }
    | '-='
    {
        $retval = Expression::Subtract;
    }
    | '<<='
    {
        $retval = Expression::LeftShift;
    }
    | '>>='
    {
        $retval = Expression::RightShift;
    }
    | '&='
    {
        $retval = Expression::And;
    }
    | '^='
    {
        $retval = Expression::ExclusiveOr;
    }
    | '|='
    {
        $retval = Expression::Or;
    }
    ;

conditional_expression returns [Expression *retval]
    : e = logical_or_expression
    {
        $retval = $e.retval;
    }
    (
        '?' et = expression ':' ef = conditional_expression
        {
            DisplayError(CTX, "Not supported: ?:");
            /* $retval = new ConditionalExpression($retval, $et.retval, $ef.retval); */
        }
    )?
    ;

logical_or_expression returns [Expression *retval]
    : e1 = logical_and_expression 
    {
        $retval = $e1.retval;
    }
    (
        '||' e2 = logical_and_expression
        {
            $retval = new BinaryExpression(Expression::OrElse, $retval, $e2.retval);
        }
    )*
    ;

logical_and_expression returns [Expression *retval]
    : e1 = inclusive_or_expression 
    {
        $retval = $e1.retval;
    }
    (
        '&&' e2 = inclusive_or_expression
        {
            $retval = new BinaryExpression(Expression::AndAlso, $retval, $e2.retval);
        }
    )*
    ;

inclusive_or_expression returns [Expression *retval]
    : e1 = exclusive_or_expression 
    {
        $retval = $e1.retval;
    }
    (
        '|' e2 = exclusive_or_expression
        {
            $retval = new BinaryExpression(Expression::Or, $retval, $e2.retval);
        }
    )*
    ;

exclusive_or_expression returns [Expression *retval]
    : e1 = and_expression 
    {
        $retval = $e1.retval;
    }
    (
        '^' e2 = and_expression
        {
            $retval = new BinaryExpression(Expression::ExclusiveOr, $retval, $e2.retval);
        }
    )*
    ;

and_expression returns [Expression *retval]
    : e1 = equality_expression 
    {
        $retval = $e1.retval;
    }
    (
        '&' e2 = equality_expression
        {
            $retval = new BinaryExpression(Expression::And, $retval, $e2.retval);
        }
    )*
    ;

equality_expression returns [Expression *retval]
    : e1 = relational_expression 
    {
        $retval = $e1.retval;
    }
    (
        '==' e2 = relational_expression
        {
            $retval = new BinaryExpression(Expression::Equal, $retval, $e2.retval);
        }
        | '!=' e3 = relational_expression
        {
            $retval = new BinaryExpression(Expression::NotEqual, $retval, $e3.retval);
        }
    )*
    ;

relational_expression returns [Expression *retval]
    : e1 = shift_expression 
    {
        $retval = $e1.retval;
    }
    (
        '<' e2 = shift_expression
        {
            $retval = new BinaryExpression(Expression::LessThan, $retval, $e2.retval);
        }
        | '>' e3 = shift_expression
        {
            $retval = new BinaryExpression(Expression::GreaterThan, $retval, $e3.retval);
        }
        | '<=' e4 = shift_expression
        {
            $retval = new BinaryExpression(Expression::LessThanOrEqual, $retval, $e4.retval);
        }
        | '>=' e5 = shift_expression
        {
            $retval = new BinaryExpression(Expression::GreaterThanOrEqual, $retval, $e5.retval);
        }
    )*
    ;

shift_expression returns [Expression *retval]
    : e1 = additive_expression
    {
        $retval = $e1.retval;
    }    
    (
        '<<' e2 = additive_expression
        {
            $retval = new BinaryExpression(Expression::LeftShift, $retval, $e2.retval);
        }
        | '>>' e3 = additive_expression
        {
            $retval = new BinaryExpression(Expression::RightShift, $retval, $e3.retval);
        }
    )*
    ;

// S t a t e m e n t s

statement returns [Expression *retval]
    : s1 = labeled_statement
    {
        $retval = $s1.retval;
    }
    | s2 = compound_statement
    {
        $retval = $s2.retval;
    }
    | s3 = expression_statement
    {
        $retval = $s3.retval;
    }
    | s4 = selection_statement
    {
        $retval = $s4.retval;
    }
    | s5 = iteration_statement
    {
        $retval = $s5.retval;
    }
    | s6 = jump_statement
    {
        $retval = $s6.retval;
    }
    | s7 = runner_statement
    {
        $retval = $s7.retval;
    }
    | s8 = inline_asm_statement
    {
        $retval = $s8.retval;
    }
    | s9 = empty_statement
    {
        $retval = $s9.retval;
    }
    ;

empty_statement returns [Expression *retval]:
    ';'
    {
        BlockExpression *expr = new BlockExpression();
        expr->SetExpressionList(new std::vector<Expression *>());
        $retval = expr;
    }
    ;
    

inline_asm_statement returns [Expression *retval]:
    'asm' '(' STRING_LITERAL  ')' ';'
    {
        std::string str = std::string((const char *)($STRING_LITERAL.text->chars));
        str = str.substr(1, str.size() - 2);
        InlineAssemblyExpression *expr = new InlineAssemblyExpression(str);
        $retval = expr;
    }
    ;
runner_statement returns [Expression *retval]
    scope
    {
        std::vector< Expression* > *argList;
        std::vector< Expression* > *watchingList;
        std::vector< Expression* > *usingList;
        std::vector< Expression* > *space;
    }
    : 'runner' 
    {
        $runner_statement::argList = new std::vector< Expression* >();
        $runner_statement::watchingList = new std::vector< Expression* >();
        $runner_statement::usingList = new std::vector< Expression* >();        
        $runner_statement::space = new std::vector< Expression* >();        
    }
    primary_expression 
    '(' (arglist1 = argument_expression_list { $runner_statement::argList = $arglist1.retval; })? ')'
    (
        'watching' arglist2 = argument_expression_list
        {
            $runner_statement::watchingList = $arglist2.retval;
        }
        | 'using' arglist3 = argument_expression_list
        {
            $runner_statement::usingList = $arglist3.retval;
        }
        | 'in' arglist4 = argument_expression_list
        {
            $runner_statement::space = $arglist4.retval;
        }

    )* ';'
    {
        $retval = new NewRunnerExpression($primary_expression.retval, $runner_statement::argList, $runner_statement::watchingList, $runner_statement::usingList, $runner_statement::space);
    }
    ;

labeled_statement returns [Expression *retval]
    : id1 = IDENTIFIER ':' 
    {
        Symbol *sym = new Symbol((const char *)$id1.text->chars, new LabelType(), Symbol::ObjectName);
        SymbolScope::GetRootScope()->Add(sym);
        $retval  = new LabelExpression(sym);        
    }
    ;

compound_statement returns [BlockExpression *retval]
    : '{'
    {
        $retval = new BlockExpression();
        EnterScope(SymbolScope::Block, $retval);
        $retval->SetBlockScope(GetCurrentScope());
    }
    (   
        d = declaration
        {
            std::vector<Declaration *> *decl = $d.retval;
            for (int i = 0; i < decl->size(); i++)
            {
                if ((*decl)[i]->GetName() != "")
                {
                    Symbol *sym = new Symbol((*decl)[i]->GetName(), (*decl)[i]->GetType(), Symbol::ObjectName);
                    if (!GetCurrentScope()->Add(sym))
                    {
                        DisplayError(CTX, "\%s is already declared in this scope", sym->Name.c_str());
                    }
                }
            }                       
        }
    )* 
    (
        sl = statement_list
        {
            $retval->SetExpressionList($sl.retval);
        }
    )
    '}'
    {
        ExitScope();
    }
    ;

statement_list returns [std::vector<Expression *> *retval]
	@init
	{
		$retval = new std::vector<Expression *>();
	}
	: (
            stmt = statement
            {
                $retval->push_back($stmt.retval);
            }
	)
	*
    ;

expression_statement returns [Expression *retval]
    : ';'
    {
        $retval = new NopExpression();
    }
    | e = expression ';'
    {
        $retval = $e.retval;
    }
    ;

selection_statement returns [ConditionalExpression *retval]
    : 'if' '(' cs1 = expression ')' ts1 = statement ('else')=> 'else' es1 = statement
    {
        $retval = new ConditionalExpression();  
        $retval->GetConditionalBranches()->insert(std::pair<Expression *, Expression *>($cs1.retval, $ts1.retval));
        $retval->SetDefaultBranch($es1.retval);
    }
    | 'if' '(' cs2 = expression ')' ts2 = statement
    {
        $retval = new ConditionalExpression();  
        $retval->GetConditionalBranches()->insert(std::pair<Expression *, Expression *>($cs2.retval, $ts2.retval));
        $retval->SetDefaultBranch(new BlockExpression());
    }
    | 'switch' '(' e3 = expression ')' {$retval = new ConditionalExpression(); } switch_body[$retval, $e3.retval]
    {
        $retval->SetGoThrough(true);
    }
    ;

switch_body [ConditionalExpression *expr, Expression *cond]
    : '{'
    (
    'case' e1 = expression ':' s1 = statement_list
    {
        Expression *condExpr = new BinaryExpression(Expression::Equal, cond, $e1.retval);
        BlockExpression *bodyExpr = new BlockExpression();
        bodyExpr->SetExpressionList($s1.retval);
        $expr->GetConditionalBranches()->insert($expr->GetConditionalBranches()->end(), std::pair<Expression*, Expression*>(condExpr, bodyExpr));
    }
    | 'default' ':' s2 = statement_list
    {
        BlockExpression *bodyExpr = new BlockExpression();
        bodyExpr->SetExpressionList($s2.retval);
        $expr->SetDefaultBranch(bodyExpr);    
    }
    )* '}'
    ;
    

iteration_statement returns [Expression *retval]
    : 'while' '(' e1 = expression ')' s1 = statement
    {
        $retval = new LoopExpression(NULL, $e1.retval, NULL, $s1.retval, false);
    }
    | 'do' s2 = statement 'while' '(' e3 = expression ')' ';'
    {
        $retval = new LoopExpression(NULL, $e3.retval, NULL, $s2.retval, true);
    }
    | 'for' '(' s4 = expression_statement s5 = expression_statement e6 = expression ')' s7 = statement
    {
        $retval = new LoopExpression($s4.retval, $s5.retval, $e6.retval, $s7.retval, false);
    }
    ;

jump_statement returns [Expression *retval]
    : 'goto' id1 = IDENTIFIER ';'
    {
        $retval = new GotoExpression((const char *)($id1.text->chars));
    }
    | 'continue' ';'
    {
        $retval = new GotoExpression(GotoExpression::Continue);
    }
    | 'break' ';'
    {
        $retval = new GotoExpression(GotoExpression::Break);
    }
    | 'return' ';'
    {
        $retval = new ReturnExpression();       
    }
    | 'return' re = expression ';'
    {
        $retval = new ReturnExpression($re.retval);       
    }
    | 'commit' ';'
    {
        $retval = new CommitExpression(false);
    }
    | 'abort' ';'
    {
        $retval = new AbortExpression(false);
    }
    | 'commitd' ';'
    {
        $retval = new CommitExpression(true);
    }
    | 'abortd' ';'
    {
        $retval = new AbortExpression(true);
    }    ;


IDENTIFIER
    :   LETTER (LETTER|'0'..'9')*
    ;
	
fragment
LETTER
    :	'$'
    |	'A'..'Z'
    |	'a'..'z'
    |	'_'
    ;

CHARACTER_LITERAL
    :   '\'' ( EscapeSequence | ~('\''|'\\') ) '\''
    ;

STRING_LITERAL
    :  '"' STRING_GUTS '"'
    ;

fragment
STRING_GUTS :	( EscapeSequence | ~('\\'|'"') )* ;

HEX_LITERAL : '0' ('x'|'X') HexDigit+ IntegerTypeSuffix? ;

DECIMAL_LITERAL : ('0' | '1'..'9' '0'..'9'*) IntegerTypeSuffix? ;

OCTAL_LITERAL : '0' ('0'..'7')+ IntegerTypeSuffix? ;


fragment
HexDigit : ('0'..'9'|'a'..'f'|'A'..'F') ;

fragment
IntegerTypeSuffix
    :	('l'|'L')
    |	('u'|'U')  ('l'|'L')?
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

WS  :  (' '|'\r'|'\t'|'\u000C' | '\n') {$channel=HIDDEN;}
    ;


COMMENT
    :   '/*' ( options {greedy=false;} : . )* '*/' {$channel=HIDDEN;}
    ;

LINE_COMMENT
    : '//' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
    ;

// ignore #line info for now
LINE_COMMAND 
    : '#' (' ' | '\t')*
    (
        'include' (' ' | '\t')+ '"' file = STRING_GUTS '"' (' ' | '\t')* '\r'? '\n'
        {
            pANTLR3_STRING fName;
            pANTLR3_INPUT_STREAM    in;
            fName = $file.text;  
            
            // Create a new input stream and take advantage of built in stream stacking in C target runtime.
            in = antlr3FileStreamNew(fName->chars, ANTLR3_ENC_UTF8);
            if (in == NULL)
            {
                // TODO: Better error - location, file name
                fprintf(stderr, "Erros: \%u: Cannot find include file \'\%s\'\n", GETLINE(), (const char *)(fName->chars));
                numErrors++;
                exit(1);
            }

            PUSHSTREAM(in);
        }
        | (('0'..'9')=>('0'..'9'))+ ~('\n'|'\r')* '\r'? '\n'
    )
    {$channel=HIDDEN;}
    ;

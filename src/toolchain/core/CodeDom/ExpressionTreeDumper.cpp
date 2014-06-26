#include <core/Core.h>
#include "ExpressionTreeDumper.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include "../Pass/ILGenerator.h"

ExpressionTreeDumper::ExpressionTreeDumper()
{
    this->_indentLevel = 0;
}

ExpressionTreeDumper::~ExpressionTreeDumper()
{

}

void ExpressionTreeDumper::MakeIndent()
{
    for (int i = 0; i < _indentLevel; i++)
    {
        if(CompilationContext::GetInstance()->Debug)
            printf("    ");
    }
}

void ExpressionTreeDumper::Visit(ProgramExpression* node)
{
    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("*************************************\n");

    std::vector<Expression *> *functions = node->GetFunctions();
    for (std::vector<Expression *>::iterator it = functions->begin();
         it != functions->end();
         ++it)
    {
        Expression *expr = (*it);
        expr->Accept(this);
    }

    if(CompilationContext::GetInstance()->Debug)
        printf("*************************************\n");
}

void ExpressionTreeDumper::Visit(FunctionExpression* node)
{
    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("FUNCTION: %s\n", node->GetName().c_str());
    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("RETURN: %s\n", node->GetReturnType()->ToString().c_str());
    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("PARAMETERS:\n");

    _indentLevel++;
    for (std::vector<Declaration *>::iterator it = node->GetParameters()->begin();
         it != node->GetParameters()->end();
         it++)
    {
        MakeIndent();
        if(CompilationContext::GetInstance()->Debug)
            printf("NAME: %s, TYPE: %s\n", (*it)->GetName().c_str(), (*it)->GetType()->ToString().c_str());
    }
    _indentLevel--;

    BlockExpression *body = node->GetBody();
    if (body != NULL)
    {
        MakeIndent();
        if(CompilationContext::GetInstance()->Debug)
            printf("BODY:\n");

        body->Accept(this);
    }
}

void ExpressionTreeDumper::Visit(ReturnExpression* node)
{
    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("return");
    Expression *retValue = node->GetValue();
    if (retValue != NULL)
    {
        if(CompilationContext::GetInstance()->Debug)
            printf(" ");
        retValue->Accept(this);
    }
    if(CompilationContext::GetInstance()->Debug)
        printf("\n");
}

void ExpressionTreeDumper::Visit(ConvertExpression* node)
{
    MakeIndent();
    Type *targetType = node->GetTargetType();
    if(CompilationContext::GetInstance()->Debug)
        printf("(%s)", targetType->ToString().c_str());
    node->GetSourceExpression()->Accept(this);
}

void ExpressionTreeDumper::Visit(UnaryExpression* node)
{
    if(CompilationContext::GetInstance()->Debug)
        printf("(%s", this->GetOpeartorString(node->GetAction()).c_str());
    node->GetOperand()->Accept(this);
    if(CompilationContext::GetInstance()->Debug)
        printf(")");
}

void ExpressionTreeDumper::Visit(VariableExpression* node)
{
    if(CompilationContext::GetInstance()->Debug)
        printf("%s", node->GetSymbol()->Name.c_str());
}

void ExpressionTreeDumper::Visit(MemberExpression* node)
{
    //TODO: Member expression
    node->GetContainer()->Accept(this);
    if(CompilationContext::GetInstance()->Debug)
        printf(".%s", node->GetFieldName().c_str());
}

void ExpressionTreeDumper::Visit(LoopExpression* node)
{
    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("\nLOOP\n");
    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("INIT\n");
    if (node->GetInitializingExpression() != NULL)
    {
        node->GetInitializingExpression()->Accept(this);
    }
    if(CompilationContext::GetInstance()->Debug)
        printf("\n");
    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("COND\n");
    if (node->GetConditionExpression() != NULL)
    {
        node->GetConditionExpression()->Accept(this);
    }
    if(CompilationContext::GetInstance()->Debug)
        printf("\n");
    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("INCR\n");
    if (node->GetIncrementExpression() != NULL)
    {
        node->GetIncrementExpression()->Accept(this);
    }
    if(CompilationContext::GetInstance()->Debug)
        printf("\n");
    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("BODY\n");
    if (node->GetBodyExpression() != NULL)
    {
        node->GetBodyExpression()->Accept(this);
    }
    if(CompilationContext::GetInstance()->Debug)
        printf("\n");
}

void ExpressionTreeDumper::Visit(LabelExpression* node)
{
    _indentLevel--;
    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("%s:", node->GetName().c_str());
    _indentLevel++;
}

void ExpressionTreeDumper::Visit(IndexExpression* node)
{
    node->GetContainer()->Accept(this);
    if(CompilationContext::GetInstance()->Debug)
        printf("[");
    node->GetIndex()->Accept(this);
    if(CompilationContext::GetInstance()->Debug)
        printf("]");
}

void ExpressionTreeDumper::Visit(GotoExpression* node)
{
    MakeIndent();
    switch (node->GetTargetType())
    {
    case GotoExpression::NamedLabel:
        if(CompilationContext::GetInstance()->Debug)
            printf("goto %s\n", node->GetLabelName().c_str());
        break;
    case GotoExpression::Continue:
        if(CompilationContext::GetInstance()->Debug)
            printf("continue\n");
        break;
    case GotoExpression::Break:
        if(CompilationContext::GetInstance()->Debug)
            printf("break\n");
        break;
    default:
        abort();
    }
}

void ExpressionTreeDumper::Visit(CallExpression* node)
{
    Expression *target = node->GetTarget();
    std::vector<Expression *> *arguments = node->GetArgumentList();
    target->Accept(this);
    if(CompilationContext::GetInstance()->Debug)
        printf("(");
    for (size_t i = 0; i < arguments->size(); i++)
    {
        Expression *arg = (*arguments)[i];
        arg->Accept(this);
        if (i != arguments->size() - 1)
        {
            if(CompilationContext::GetInstance()->Debug)
                printf(", ");
        }
    }
    if(CompilationContext::GetInstance()->Debug)
        printf(")");
}

void ExpressionTreeDumper::Visit(ConstantExpression* node)
{
    //TODO: More details
    if(CompilationContext::GetInstance()->Debug)
        printf("%s", node->GetValue()->ToString().c_str());
}

void ExpressionTreeDumper::Visit(ConditionalExpression* node)
{
    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("branch\n");

    _indentLevel++;

    std::map<Expression *, Expression *> *condBranches = node->GetConditionalBranches();
    for (std::map<Expression *, Expression *>::iterator it = condBranches->begin(); it != condBranches->end(); ++it)
    {
        Expression *condtion = it->first;
        Expression *expr = it->second;
        MakeIndent();
        condtion->Accept(this);
        if(CompilationContext::GetInstance()->Debug)
            printf(":\n");
        _indentLevel++;
        MakeIndent();
        expr->Accept(this);
        _indentLevel--;
    }

    Expression *defExpr = node->GetDefaultBranch();
    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("default:\n");
    _indentLevel++;
    MakeIndent();
    defExpr->Accept(this);
    _indentLevel--;

    _indentLevel--;
}

void ExpressionTreeDumper::Visit(BlockExpression* node)
{
    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("{\n");

    _indentLevel++;

    std::vector<Expression *> *expressions = node->GetExpressionList();
    if (expressions != NULL)
    {
        for (std::vector<Expression *>::iterator it = expressions->begin();
             it != expressions->end();
             ++it
            )
        {
            Expression *expr = *it;
            BlockExpression *block = dynamic_cast<BlockExpression *>(expr);
            if (block != NULL)
            {
                block->Accept(this);
            }
            else
            {
                MakeIndent();
                expr->Accept(this);
                if(CompilationContext::GetInstance()->Debug)
                    printf("\n");
            }
        }
    }

    _indentLevel--;

    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("}\n");
}

void ExpressionTreeDumper::Visit(NopExpression* node)
{
    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("\n");

}

void ExpressionTreeDumper::Visit(AssignExpression* node)
{
    if(CompilationContext::GetInstance()->Debug)
        printf("(");
    node->GetTarget()->Accept(this);
    if(CompilationContext::GetInstance()->Debug)
        printf(" %s= ", this->GetOpeartorString(node->GetAdditionalOperator()).c_str());
    node->GetSource()->Accept(this);
    if(CompilationContext::GetInstance()->Debug)
        printf(")");

}


void ExpressionTreeDumper::Visit(BinaryExpression* node)
{
    if(CompilationContext::GetInstance()->Debug)
        printf("(");
    node->GetLeftOperand()->Accept(this);
    if(CompilationContext::GetInstance()->Debug)
        printf(" %s ", this->GetOpeartorString(node->GetAction()).c_str());
    node->GetRightOperand()->Accept(this);
    if(CompilationContext::GetInstance()->Debug)
        printf("):");
    Type *type = node->GetTag<Type>("Type");

    ILGenerator igen;
    if(CompilationContext::GetInstance()->Debug)
        printf("%d", igen.GetOperandType(type));
}



std::string ExpressionTreeDumper::GetOpeartorString(Expression::ExpressionAction type)
{
    switch (type)
    {
    case Expression::Program:
        return "PROGRAM";
        break;
    case Expression::Add:
        return "+";
        break;
    case Expression::And:
        return "&";
        break;
    case Expression::AndAlso:
        return "&&";
        break;
    case Expression::ArrayIndex:
        return "[]";
        break;
    case Expression::Call:
        return "CALL";
        break;
    case Expression::Conditional:
        return "?:";
        break;
    case Expression::Constant:
        return "CONSTANT";
        break;
    case Expression::Convert:
        return "(T)";
        break;
    case Expression::Divide:
        return "/";
        break;
    case Expression::Equal:
        return "==";
        break;
    case Expression::ExclusiveOr:
        return "^";
        break;
    case Expression::Function:
        return "FUNCTION";
        break;
    case Expression::GreaterThan:
        return ">";
        break;
    case Expression::GreaterThanOrEqual:
        return ">=";
        break;
    case Expression::LeftShift:
        return "<<";
        break;
    case Expression::LessThan:
        return "<";
        break;
    case Expression::LessThanOrEqual:
        return "<=";
        break;
    case Expression::Modulo:
        return "%";
        break;
    case Expression::Multiply:
        return "*";
        break;
    case Expression::Minus:
        return "~";
        break;
    case Expression::Plus:
        return "+";
        break;
    case Expression::Not:
        return "!";
        break;
    case Expression::NotEqual:
        return "!=";
        break;
    case Expression::Or:
        return "|";
        break;
    case Expression::OrElse:
        return "||";
        break;
    case Expression::RightShift:
        return ">>";
        break;
    case Expression::Subtract:
        return "-";
        break;
    case Expression::Assign:
        return "=";
        break;
    case Expression::Block:
        return "BLOCK";
        break;
    case Expression::Return:
        return "return";
        break;
    case Expression::Goto:
        return "goto";
        break;
    case Expression::Label:
        return ":";
        break;
    case Expression::Loop:
        return "LOOP";
        break;
    case Expression::Switch:
        return "SWITCH";
        break;
    case Expression::Member:
        return ".";
        break;
    case Expression::Variable:
        return "VAR";
        break;
    case Expression::Reference:
        return "&";
        break;
    case Expression::Dereference:
        return "*";
        break;
    case Expression::Nop:
        return "";
        break;
    default:
        abort();
    }
}

void ExpressionTreeDumper::Visit(ArraySegmentExpression* node)
{
    node->GetArray()->Accept(this);
    if(CompilationContext::GetInstance()->Debug)
        printf("[");
    node->GetStartIndex()->Accept(this);
    if(CompilationContext::GetInstance()->Debug)
        printf(",, ");
    node->GetEndIndex()->Accept(this);
    if(CompilationContext::GetInstance()->Debug)
        printf("]");
}
void ExpressionTreeDumper::Visit(NewRunnerExpression* node)
{
    Expression *target = node->GetTarget();
    std::vector<Expression *> *arguments = node->GetArgumentList();
    std::vector<Expression *> *watching = node->GetWatchingList();
    std::vector<Expression *> *usingList = node->GetUsingList();
    std::vector<Expression *> *space = node->GetSpace();
    // MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("runner ");
    target->Accept(this);
    if(CompilationContext::GetInstance()->Debug)
        printf("(");
    for (size_t i = 0; i < arguments->size(); i++)
    {
        Expression *arg = (*arguments)[i];
        arg->Accept(this);
        if (i != arguments->size() - 1)
        {
            if(CompilationContext::GetInstance()->Debug)
                printf(", ");
        }
    }
    if(CompilationContext::GetInstance()->Debug)
        printf(") watching (");
    for (size_t i = 0; i < watching->size(); i++)
    {
        Expression *arg = (*watching)[i];
        arg->Accept(this);
        if (i != watching->size() - 1)
        {
            if(CompilationContext::GetInstance()->Debug)
                printf(", ");
        }
    }
    if(CompilationContext::GetInstance()->Debug)
        printf(") using (");
    for (size_t i = 0; i < usingList->size(); i++)
    {
        Expression *arg = (*usingList)[i];
        arg->Accept(this);
        if (i != usingList->size() - 1)
        {
            if(CompilationContext::GetInstance()->Debug)
                printf(", ");
        }
    }
    if(CompilationContext::GetInstance()->Debug)
        printf(") in (");
    for (size_t i = 0; i < space->size(); i++)
    {
        Expression *arg = (*space)[i];
        arg->Accept(this);
        if (i != space->size() - 1)
        {
            if(CompilationContext::GetInstance()->Debug)
                printf(", ");
        }
    }
    if(CompilationContext::GetInstance()->Debug)
        printf(")");

}
void ExpressionTreeDumper::Visit(CommitExpression* node)
{
    // MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("commit");
    if (node->GetDeleteWatcher())
    {
        if(CompilationContext::GetInstance()->Debug)
            printf("d");
    }
    if(CompilationContext::GetInstance()->Debug)
        printf("\n");
}
void ExpressionTreeDumper::Visit(AbortExpression* node)
{
    // MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("abort");
    if (node->GetDeleteWatcher())
    {
        if(CompilationContext::GetInstance()->Debug)
            printf("d");
    }
    if(CompilationContext::GetInstance()->Debug)
        printf("\n");
}

void ExpressionTreeDumper::Visit(InlineAssemblyExpression* node)
{
    MakeIndent();
    if(CompilationContext::GetInstance()->Debug)
        printf("asm \"%s\"", node->GetTemplateString().c_str());
    if(CompilationContext::GetInstance()->Debug)
        printf("\n");
}

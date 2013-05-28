#include <core/Core.h>
#include "ExpressionTreeDumper.h"
#include <stdlib.h>
#include <stdio.h>
#include <vector>

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
        printf("    ");
    }
}

void ExpressionTreeDumper::Visit(ProgramExpression* node)
{
    MakeIndent();
    printf("*************************************\n");

    std::vector<Expression *> *functions = node->GetFunctions();
    for (std::vector<Expression *>::iterator it = functions->begin();
            it != functions->end();
            ++it)
    {
        Expression *expr = (*it);
        expr->Accept(this);
    }

    printf("*************************************\n");
}

void ExpressionTreeDumper::Visit(FunctionExpression* node)
{
    MakeIndent();
    printf("FUNCTION: %s\n", node->GetName().c_str());
    MakeIndent();
    printf("RETURN: %s\n", node->GetReturnType()->ToString().c_str());
    MakeIndent();
    printf("PARAMETERS:\n");

    _indentLevel++;
    for (std::vector<Declaration *>::iterator it = node->GetParameters()->begin();
            it != node->GetParameters()->end();
            it++)
    {
        MakeIndent();
        printf("NAME: %s, TYPE: %s\n", (*it)->GetName().c_str(), (*it)->GetType()->ToString().c_str());
    }
    _indentLevel--;

    BlockExpression *body = node->GetBody();
    if (body != NULL)
    {
        MakeIndent();
        printf("BODY:\n");

        body->Accept(this);
    }
}

void ExpressionTreeDumper::Visit(ReturnExpression* node)
{
    MakeIndent();
    printf("return");
    Expression *retValue = node->GetValue();
    if (retValue != NULL)
    {
        printf(" ");
        retValue->Accept(this);
    }
    printf("\n");
}

void ExpressionTreeDumper::Visit(ConvertExpression* node)
{
    MakeIndent();
    Type *targetType = node->GetTargetType();
    printf("(%s)", targetType->ToString().c_str());
    node->GetSourceExpression()->Accept(this);
}

void ExpressionTreeDumper::Visit(UnaryExpression* node)
{
    printf("(%s", this->GetOpeartorString(node->GetAction()).c_str());
    node->GetOperand()->Accept(this);
    printf(")");
}

void ExpressionTreeDumper::Visit(VariableExpression* node)
{
    printf("%s", node->GetSymbol()->Name.c_str());
}

void ExpressionTreeDumper::Visit(MemberExpression* node)
{
    //TODO: Member expression
    node->GetContainer()->Accept(this);
    printf(".%s", node->GetFieldName().c_str());
}

void ExpressionTreeDumper::Visit(LoopExpression* node)
{
    MakeIndent();
    printf("\nLOOP\n");
    MakeIndent();
    printf("INIT\n");
    if (node->GetInitializingExpression() != NULL)
    {
        node->GetInitializingExpression()->Accept(this);
    }
    printf("\n");
    MakeIndent();
    printf("COND\n");
    if (node->GetConditionExpression() != NULL)
    {
        node->GetConditionExpression()->Accept(this);
    }
    printf("\n");
    MakeIndent();
    printf("INCR\n");
    if (node->GetIncrementExpression() != NULL)
    {
        node->GetIncrementExpression()->Accept(this);
    }
    printf("\n");
    MakeIndent();
    printf("BODY\n");
    if (node->GetBodyExpression() != NULL)
    {
        node->GetBodyExpression()->Accept(this);
    }
    printf("\n");
}

void ExpressionTreeDumper::Visit(LabelExpression* node)
{
    _indentLevel--;
    MakeIndent();
    printf("%s:", node->GetName().c_str());
    _indentLevel++;
}

void ExpressionTreeDumper::Visit(IndexExpression* node)
{
    node->GetContainer()->Accept(this);
    printf("[");
    node->GetIndex()->Accept(this);
    printf("]");
}

void ExpressionTreeDumper::Visit(GotoExpression* node)
{
    MakeIndent();
    switch (node->GetTargetType())
    {
        case GotoExpression::NamedLabel:
            printf("goto %s\n", node->GetLabelName().c_str());
            break;
        case GotoExpression::Continue:
            printf("continue\n");
            break;
        case GotoExpression::Break:
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
    printf("(");
    for (int i = 0; i < arguments->size(); i++)
    {
        Expression *arg = (*arguments)[i];
        arg->Accept(this);
        if (i != arguments->size() - 1)
        {
            printf(", ");
        }
    }
    printf(")");
}

void ExpressionTreeDumper::Visit(ConstantExpression* node)
{
    //TODO: More details
    printf("%s", node->GetValue()->ToString().c_str());
}

void ExpressionTreeDumper::Visit(ConditionalExpression* node)
{
    MakeIndent();
    printf("branch\n");
    
    _indentLevel++;
    
    std::map<Expression *, Expression *> *condBranches = node->GetConditionalBranches();
    for (std::map<Expression *, Expression *>::iterator it = condBranches->begin(); it != condBranches->end(); ++it)
    {
        Expression *condtion = it->first;
        Expression *expr = it->second;
        MakeIndent();    
        condtion->Accept(this);
        printf(":\n");
        _indentLevel++;
        MakeIndent();  
        expr->Accept(this);
        _indentLevel--;
    }
    
    Expression *defExpr = node->GetDefaultBranch();
    MakeIndent();    
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
                printf("\n");
            }
        }
    }

    _indentLevel--;

    MakeIndent();
    printf("}\n");
}

void ExpressionTreeDumper::Visit(NopExpression* node)
{
    MakeIndent();
    printf("\n");
    
}

void ExpressionTreeDumper::Visit(AssignExpression* node)
{
    printf("(");
    node->GetTarget()->Accept(this);
    printf(" %s= ", this->GetOpeartorString(node->GetAdditionalOperator()).c_str());
    node->GetSource()->Accept(this);
    printf(")");

}


void ExpressionTreeDumper::Visit(BinaryExpression* node)
{
    printf("(");
    node->GetLeftOperand()->Accept(this);
    printf(" %s ", this->GetOpeartorString(node->GetAction()).c_str());
    node->GetRightOperand()->Accept(this);
    printf(")");
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
    printf("[");
    node->GetStartIndex()->Accept(this);
    printf(",, ");
    node->GetEndIndex()->Accept(this);
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
    printf("runner ");
    target->Accept(this);
    printf("(");
    for (int i = 0; i < arguments->size(); i++)
    {
        Expression *arg = (*arguments)[i];
        arg->Accept(this);
        if (i != arguments->size() - 1)
        {
            printf(", ");
        }
    }
    printf(") watching (");
    for (int i = 0; i < watching->size(); i++)
    {
        Expression *arg = (*watching)[i];
        arg->Accept(this);
        if (i != watching->size() - 1)
        {
            printf(", ");
        }
    }
    printf(") using (");
    for (int i = 0; i < usingList->size(); i++)
    {
        Expression *arg = (*usingList)[i];
        arg->Accept(this);
        if (i != usingList->size() - 1)
        {
            printf(", ");
        }
    }
    printf(") in (");
    for (int i = 0; i < space->size(); i++)
    {
        Expression *arg = (*space)[i];
        arg->Accept(this);
        if (i != space->size() - 1)
        {
            printf(", ");
        }
    }
    printf(")");

}
void ExpressionTreeDumper::Visit(CommitExpression* node)
{
    // MakeIndent();
    printf("commit");
    if (node->GetDeleteWatcher())
    {
        printf("d");
    }
    printf("\n");
}
void ExpressionTreeDumper::Visit(AbortExpression* node)
{
    // MakeIndent();
    printf("abort");
    if (node->GetDeleteWatcher())
    {
        printf("d");
    }
    printf("\n");
}

void ExpressionTreeDumper::Visit(InlineAssemblyExpression* node)
{
    MakeIndent();
    printf("asm \"%s\"", node->GetTemplateString().c_str());
    printf("\n");
}


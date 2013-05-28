#ifndef DECLARATIONTEMPLATE_H
#define DECLARATIONTEMPLATE_H
#include <vector>
#include <string>


class SymbolScope;
class Declaration;
class FunctionType;
class Expression;
class Type;
class DeclarationTemplate
{
public:
    enum SpecifierKind
    {
        Array,
        Pointer,
        Function
    };
    struct DeclarationSpecifier
    {
    public:
        SpecifierKind Kind;
        Expression *ArraySize;
        std::vector<Declaration *>  *Parameters;
    };
private:

    Type * _baseType;
    DeclarationTemplate *_parent;

    std::vector<DeclarationSpecifier> _suffixes;
    std::vector<DeclarationSpecifier> _pointers;
    std::string _name;
protected:
    Type * GetDerivedType(Type *baseType, DeclarationSpecifier spec);
public:
    DeclarationTemplate(DeclarationTemplate *parentDecl);
    DeclarationTemplate(Type *baseType);
    virtual ~DeclarationTemplate();
public:
    Type * GetType();
    std::vector<DeclarationSpecifier> * GetSuffixList();
    std::vector<DeclarationSpecifier> * GetPointerList();
    std::string GetName();
    void SetName(std::string name);
};

#endif // DECLARATIONTEMPLATE_H

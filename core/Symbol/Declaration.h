#pragma once

#include <string>
#include <stdint.h>


class Type;

class Declaration
{
private:
    Type *_type;
    std::string _name;
    int64_t _offset;
public:
    Declaration(std::string name, Type *type);
    virtual ~Declaration();
public:
    Type *GetType();
    std::string GetName();
public:
    int64_t GetOffset();
    void SetOffset(uint64_t offset);
};

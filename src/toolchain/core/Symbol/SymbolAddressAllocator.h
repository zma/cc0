#ifndef SYMBOLADDRESSALLOCATOR_H
#define SYMBOLADDRESSALLOCATOR_H
#include <stdint.h>


class SymbolScope;

class SymbolAddressAllocator
{
private:
    int64_t _globalID;
public:
    SymbolAddressAllocator();
    virtual ~SymbolAddressAllocator();
public:
    void Allocate(SymbolScope *scope);
private:
    int64_t AllocateGlobal(SymbolScope *scope);
    int64_t AllocateFunction(SymbolScope *scope);
    int64_t AllocateBlock(SymbolScope *scope, int64_t startOffset);
};

#endif // SYMBOLADDRESSALLOCATOR_H

#pragma once

#include <vector>
#include <map>
#include "DefInfo.h"
#include "UseInfo.h"

class IL;
class Symbol;
class Tac;

class BasicBlock
{
private:
    std::vector<IL *> *_tacs;
    std::vector<BasicBlock *> *_predecessors;
    std::vector<BasicBlock *> *_successors;
    std::map<Symbol *, DefInfo> *_defs;
    std::map<Symbol *, UseInfo> *_uses;
public:
    BasicBlock();
    virtual ~BasicBlock();
public:
    std::vector<IL *> * GetILList();
    std::vector<BasicBlock *> *GetPredecessorList();
    std::vector<BasicBlock *> *GetSuccessorList();
    std::map<Symbol *, DefInfo> *GetDefSet();
    std::map<Symbol *, UseInfo> *GetUseSet();
};


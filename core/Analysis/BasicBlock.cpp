#include "BasicBlock.h"

BasicBlock::BasicBlock()
{
    _tacs = new std::vector<IL *>();
    _predecessors = new std::vector<BasicBlock *>();
    _successors = new std::vector<BasicBlock *>();
    _defs = new std::map<Symbol *, DefInfo>();
    _uses = new std::map<Symbol *, UseInfo>();
}

BasicBlock::~BasicBlock()
{
}

std::vector< IL* >* BasicBlock::GetILList()
{
    return _tacs;
}

std::vector< BasicBlock* >* BasicBlock::GetPredecessorList()
{
    return _predecessors;
}

std::vector< BasicBlock* >* BasicBlock::GetSuccessorList()
{
    return _successors;
}

std::map< Symbol*, DefInfo >* BasicBlock::GetDefSet()
{
    return _defs;
}

std::map< Symbol*, UseInfo >* BasicBlock::GetUseSet()
{
    return _uses;
}


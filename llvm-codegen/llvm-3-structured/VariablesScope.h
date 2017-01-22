#pragma once

#include <unordered_map>
#include <cstdint>
#include <climits>

namespace llvm
{
class AllocaInst;
}

class CVariablesScope
{
public:
    CVariablesScope();
    ~CVariablesScope();

    bool HasVariable(unsigned nameId)const;
    void AddVariable(unsigned nameId, llvm::AllocaInst *variable);

    // Может вернуть nullptr
    llvm::AllocaInst *GetVariable(unsigned nameId)const;

private:
    std::unordered_map<unsigned, llvm::AllocaInst *> m_variables;
};

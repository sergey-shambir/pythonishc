#pragma once

#include <unordered_map>
#include <cstdint>
#include <climits>

#include "begin_llvm.h"
#include <llvm/IR/Value.h>
#include "end_llvm.h"

class CVariablesScope
{
public:
    CVariablesScope();
    ~CVariablesScope();

    bool HasVariable(unsigned nameId)const;
    void AssignVariable(unsigned nameId, llvm::Value *value);

    // Может вернуть nullptr
    llvm::Value *GetVariableValue(unsigned nameId)const;

private:
    std::unordered_map<unsigned, llvm::Value *> m_variables;
};

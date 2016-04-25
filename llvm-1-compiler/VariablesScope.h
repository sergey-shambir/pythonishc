#pragma once

#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include <climits>
#include <llvm/IR/Value.h>

class CVariablesScope;

class IInterpreterContext
{
public:
    virtual ~IInterpreterContext() = default;
    virtual void AssignVariable(unsigned nameId, llvm::Value *value) = 0;
    virtual bool HasVariable(unsigned nameId)const = 0;
    virtual void RemoveVariable(unsigned nameId) = 0;
    virtual llvm::Value *TryGetVariableValue(unsigned nameId)const = 0;
    virtual void EnterScope(CVariablesScope & scope) = 0;
    virtual void ExitScope() = 0;
};

class CVariablesScope
{
public:
    CVariablesScope(IInterpreterContext & context);
    ~CVariablesScope();

    // Добавляемая переменная может скрыть старую переменную.
    void AssignVariable(unsigned nameId, llvm::Value *value);

    llvm::Value *TryGetVariableValue(unsigned nameId)const;

private:
    IInterpreterContext & m_context;
    std::unordered_set<unsigned> m_addedVariables;
    std::unordered_map<unsigned, llvm::Value *> m_shadowedVariables;
};

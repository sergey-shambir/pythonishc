#include "VariablesScope.h"

CVariablesScope::CVariablesScope()
{
}

CVariablesScope::~CVariablesScope()
{
}

bool CVariablesScope::HasVariable(unsigned nameId) const
{
    return (m_variables.find(nameId) != m_variables.end());
}

void CVariablesScope::AddVariable(unsigned nameId, llvm::AllocaInst *variable)
{
    if (variable)
    {
        m_variables[nameId] = variable;
    }
}

llvm::AllocaInst *CVariablesScope::GetVariable(unsigned nameId) const
{
    auto it = m_variables.find(nameId);
    if (it != m_variables.end())
    {
        return it->second;
    }
    return nullptr;
}

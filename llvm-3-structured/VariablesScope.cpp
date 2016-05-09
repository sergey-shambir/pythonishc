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

void CVariablesScope::AssignVariable(unsigned nameId, llvm::Value *value)
{
    if (value)
    {
        m_variables[nameId] = value;
    }
}

llvm::Value *CVariablesScope::GetVariableValue(unsigned nameId) const
{
    auto it = m_variables.find(nameId);
    if (it != m_variables.end())
    {
        return it->second;
    }
    return nullptr;
}

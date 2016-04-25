#include "VariablesScope.h"

CVariablesScope::CVariablesScope(IInterpreterContext & context)
    : m_context(context)
{
    context.EnterScope(*this);
}

CVariablesScope::~CVariablesScope()
{
    for (unsigned nameId : m_addedVariables)
    {
        m_context.RemoveVariable(nameId);
    }
    for (auto pair : m_shadowedVariables)
    {
        m_context.AssignVariable(pair.first, pair.second);
    }
    m_context.ExitScope();
}

bool CVariablesScope::HasVariable(unsigned nameId) const
{
    return m_context.HasVariable(nameId);
}

void CVariablesScope::AssignVariable(unsigned nameId, llvm::Value *value)
{
    if (!m_addedVariables.count(nameId))
    {
        m_addedVariables.insert(nameId);
        if (m_context.HasVariable(nameId))
        {
            m_shadowedVariables[nameId] = m_context.TryGetVariableValue(nameId);
        }
    }
    m_context.AssignVariable(nameId, value);
}

llvm::Value *CVariablesScope::TryGetVariableValue(unsigned nameId) const
{
    return m_context.TryGetVariableValue(nameId);
}

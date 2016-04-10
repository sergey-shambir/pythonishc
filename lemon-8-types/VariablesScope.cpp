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

void CVariablesScope::AssignVariable(unsigned nameId, double value)
{
    if (!m_addedVariables.count(nameId))
    {
        m_addedVariables.insert(nameId);
        if (m_context.HasVariable(nameId))
        {
            m_shadowedVariables[nameId] = m_context.GetVariableValue(nameId);
        }
    }
    m_context.AssignVariable(nameId, value);
}

double CVariablesScope::GetVariableValue(unsigned nameId) const
{
    return m_context.GetVariableValue(nameId);
}

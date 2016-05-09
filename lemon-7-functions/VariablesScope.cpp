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

void CVariablesScope::AssignVariable(unsigned nameId, double value)
{
    m_variables[nameId] = value;
}

boost::optional<double> CVariablesScope::GetVariableValue(unsigned nameId) const
{
    auto it = m_variables.find(nameId);
    if (it != m_variables.end())
    {
        return it->second;
    }
    return boost::none;
}

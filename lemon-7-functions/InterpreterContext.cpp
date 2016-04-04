#include "InterpreterContext.h"
#include "StringPool.h"
#include <iostream>

CInterpreterContext::CInterpreterContext(const CStringPool &pool)
    : m_pool(pool)
{
}

void CInterpreterContext::AssignVariable(unsigned nameId, double value)
{
    m_variables[nameId] = value;
}

bool CInterpreterContext::HasVariable(unsigned nameId) const
{
    return m_variables.count(nameId);
}

void CInterpreterContext::RemoveVariable(unsigned nameId)
{
    m_variables.erase(nameId);
}

double CInterpreterContext::GetVariableValue(unsigned nameId) const
{
    try
    {
        return m_variables.at(nameId);
    }
    catch (std::exception const&)
    {
        std::cerr << "error: unknown variable " << m_pool.GetString(nameId) << std::endl;
        return 0;
    }
}

IFunctionAST *CInterpreterContext::GetFunction(unsigned nameId) const
{
    try
    {
        return m_functions.at(nameId);
    }
    catch (std::exception const&)
    {
        std::cerr << "error: unknown function " << m_pool.GetString(nameId) << std::endl;
        return nullptr;
    }
}

void CInterpreterContext::AddFunction(unsigned nameId, IFunctionAST *function)
{
    if (function)
    {
        m_functions[nameId] = function;
    }
}

void CInterpreterContext::PrintResult(double value)
{
    std::cout << "result: " << value << std::endl;
}

void CInterpreterContext::SetReturnValue(boost::optional<double> const& valueOpt)
{
    m_returnValueOpt = valueOpt;
}

boost::optional<double> CInterpreterContext::GetReturnValue() const
{
    return m_returnValueOpt;
}

CVariableScope::CVariableScope(CInterpreterContext &context)
    : m_context(context)
{
}

CVariableScope::~CVariableScope()
{
    for (unsigned nameId : m_addedVariables)
    {
        m_context.RemoveVariable(nameId);
    }
    for (auto pair : m_shadowedVariables)
    {
        m_context.AssignVariable(pair.first, pair.second);
    }
}

void CVariableScope::AddVariable(unsigned nameId, double value)
{
    m_addedVariables.insert(nameId);
    if (m_context.HasVariable(nameId))
    {
        m_shadowedVariables[nameId] = value;
    }
    m_context.AssignVariable(nameId, value);
}

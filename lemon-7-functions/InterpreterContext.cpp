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

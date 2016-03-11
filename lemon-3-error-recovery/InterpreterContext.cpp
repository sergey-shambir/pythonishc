#include "InterpreterContext.h"
#include "StringPool.h"
#include <iostream>

CInterpreterContext::CInterpreterContext(const CStringPool &pool)
    : m_pool(pool)
{
}

void CInterpreterContext::AssignVariable(unsigned stringId, double value)
{
    m_variables[stringId] = value;
}

double CInterpreterContext::GetVariableValue(unsigned stringId)
{
    try
    {
        return m_variables.at(stringId);
    }
    catch (std::exception const&)
    {
        std::cerr << "undefined variable " << m_pool.GetString(stringId);
        return 0;
    }
}

void CInterpreterContext::PrintResult(double value)
{
    std::cout << "result: " << value << std::endl;
}

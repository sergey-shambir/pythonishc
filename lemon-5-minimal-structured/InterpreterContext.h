#pragma once
#include <unordered_map>

class CStringPool;

class CInterpreterContext
{
public:
    CInterpreterContext(CStringPool const& pool);

    void AssignVariable(unsigned stringId, double value);
    double GetVariableValue(unsigned stringId);
    void PrintResult(double value);

private:
    std::unordered_map<unsigned, double> m_variables;
    CStringPool const& m_pool;
};

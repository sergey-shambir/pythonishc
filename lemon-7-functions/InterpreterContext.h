#pragma once
#include <unordered_map>

class CStringPool;
class IFunctionAST;

class CInterpreterContext
{
public:
    CInterpreterContext(CStringPool const& pool);

    void AssignVariable(unsigned nameId, double value);
    double GetVariableValue(unsigned nameId)const;
    IFunctionAST *GetFunction(unsigned nameId)const;
    void AddFunction(unsigned nameId, IFunctionAST *function);

    void PrintResult(double value);

private:
    std::unordered_map<unsigned, double> m_variables;
    std::unordered_map<unsigned, IFunctionAST*> m_functions;
    CStringPool const& m_pool;
};

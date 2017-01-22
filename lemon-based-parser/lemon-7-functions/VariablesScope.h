#pragma once

#include <unordered_map>
#include <boost/optional.hpp>

class CVariablesScope
{
public:
    CVariablesScope();
    ~CVariablesScope();

    bool HasVariable(unsigned nameId)const;
    void AssignVariable(unsigned nameId, double value);
    boost::optional<double> GetVariableValue(unsigned nameId)const;

private:
    std::unordered_map<unsigned, double> m_variables;
};

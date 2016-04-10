#pragma once

#include <unordered_map>
#include <memory>
#include <stack>
#include <boost/optional.hpp>
#include "VariablesScope.h"

class CStringPool;
class IFunctionAST;

class CInterpreterContext : protected IInterpreterContext
{
public:
    CInterpreterContext(CStringPool & pool);

    std::unique_ptr<CVariablesScope> MakeScope();
    CVariablesScope & GetCurrentScope();

    IFunctionAST *GetFunction(unsigned nameId)const;
    void AddFunction(unsigned nameId, IFunctionAST *function);

    void SetReturnValue(const boost::optional<double> &valueOpt);
    boost::optional<double> GetReturnValue()const;

    void PrintResult(double value);

protected:
    void AssignVariable(unsigned nameId, double value) override;
    bool HasVariable(unsigned nameId)const override;
    void RemoveVariable(unsigned nameId) override;
    double GetVariableValue(unsigned nameId)const override;
    void EnterScope(CVariablesScope &scope) override;
    void ExitScope() override;

private:
    void AddBuiltin(const std::string &name, std::unique_ptr<IFunctionAST> && function);

    std::unordered_map<unsigned, double> m_variables;
    std::unordered_map<unsigned, IFunctionAST*> m_functions;
    CStringPool & m_pool;
    boost::optional<double> m_returnValueOpt;
    std::vector<std::unique_ptr<IFunctionAST>> m_builtins;
    std::stack<CVariablesScope *> m_scopes;
};

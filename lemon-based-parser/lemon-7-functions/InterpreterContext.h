#pragma once

#include <unordered_map>
#include <memory>
#include <boost/optional.hpp>

class CStringPool;
class CVariablesScope;
class IFunctionAST;
class CInterpreterContext;

class CInterpreterContext
{
public:
    CInterpreterContext(CStringPool & pool);
    ~CInterpreterContext();

    void DefineVariable(unsigned nameId, double value);
    void AssignVariable(unsigned nameId, double value);
    double GetVariableValue(unsigned nameId)const;

    void PushScope(std::unique_ptr<CVariablesScope> && scope);
    std::unique_ptr<CVariablesScope> PopScope();
    size_t GetScopesCount()const;

    IFunctionAST *GetFunction(unsigned nameId)const;
    void AddFunction(unsigned nameId, IFunctionAST *function);

    void SetReturnValue(const boost::optional<double> &valueOpt);
    boost::optional<double> GetReturnValue()const;

    void PrintResult(double value);

private:
    CVariablesScope *FindScopeWithVariable(unsigned nameId)const;
    void AddBuiltin(const std::string &name, std::unique_ptr<IFunctionAST> && function);

    std::unordered_map<unsigned, IFunctionAST*> m_functions;
    CStringPool & m_pool;
    boost::optional<double> m_returnValueOpt;
    std::vector<std::unique_ptr<IFunctionAST>> m_builtins;
    std::vector<std::unique_ptr<CVariablesScope>> m_scopes;
};

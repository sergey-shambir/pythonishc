#pragma once
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <boost/optional.hpp>

class CStringPool;
class CInterpreterContext;
class IFunctionAST;

class CVariableScope
{
public:
    CVariableScope(CInterpreterContext & context);
    ~CVariableScope();

    // Добавляемая переменная может скрыть старую переменную.
    void AddVariable(unsigned nameId, double value);

private:
    CInterpreterContext & m_context;
    std::unordered_set<unsigned> m_addedVariables;
    std::unordered_map<unsigned, double> m_shadowedVariables;
};

class CInterpreterContext
{
public:
    CInterpreterContext(CStringPool & pool);

    void AssignVariable(unsigned nameId, double value);
    bool HasVariable(unsigned nameId)const;
    void RemoveVariable(unsigned nameId);
    double GetVariableValue(unsigned nameId)const;

    IFunctionAST *GetFunction(unsigned nameId)const;
    void AddFunction(unsigned nameId, IFunctionAST *function);

    void PrintResult(double value);

    void SetReturnValue(const boost::optional<double> &valueOpt);
    boost::optional<double> GetReturnValue()const;

private:
    void AddBuiltin(const std::string &name, std::unique_ptr<IFunctionAST> && function);

    std::unordered_map<unsigned, double> m_variables;
    std::unordered_map<unsigned, IFunctionAST*> m_functions;
    CStringPool & m_pool;
    boost::optional<double> m_returnValueOpt;
    std::vector<std::unique_ptr<IFunctionAST>> m_builtins;
};

#pragma once

#include <unordered_map>
#include <memory>
#include <stack>
#include <boost/optional.hpp>
#include "Value.h"
#include "VariablesScope.h"

class CStringPool;
class IFunctionAST;

class CInterpreterContext : protected IInterpreterContext
{
public:
    CInterpreterContext(std::ostream &output, std::ostream &errors, CStringPool & pool);
    ~CInterpreterContext();

    std::unique_ptr<CVariablesScope> MakeScope();
    CVariablesScope & GetCurrentScope();

    IFunctionAST *GetFunction(unsigned nameId)const;
    void AddFunction(unsigned nameId, IFunctionAST *function);

    std::string GetStringLiteral(unsigned stringId)const;

    void SetReturnValue(const boost::optional<CValue> &valueOpt);
    boost::optional<CValue> GetReturnValue()const;

    void PrintResult(const CValue &value);
    void PrintError(std::string const& message);

protected:
    void AssignVariable(unsigned nameId, const CValue &value) override;
    bool HasVariable(unsigned nameId)const override;
    void RemoveVariable(unsigned nameId) override;
    CValue GetVariableValue(unsigned nameId)const override;
    void EnterScope(CVariablesScope &scope) override;
    void ExitScope() override;

private:
    void AddBuiltin(const std::string &name, std::unique_ptr<IFunctionAST> && function);

    std::unordered_map<unsigned, CValue> m_variables;
    std::unordered_map<unsigned, IFunctionAST*> m_functions;
    CStringPool & m_pool;
    boost::optional<CValue> m_returnValueOpt;
    std::vector<std::unique_ptr<IFunctionAST>> m_builtins;
    std::stack<CVariablesScope *> m_scopes;
    std::ostream &m_output;
    std::ostream &m_errors;
};

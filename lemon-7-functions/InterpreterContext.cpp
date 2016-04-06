#include "BatchAST.h"
#include "InterpreterContext.h"
#include "StringPool.h"
#include <iostream>

class CSinFunction : public IFunctionAST
{
public:
    double Call(CInterpreterContext &context, const std::vector<double> &arguments) const override
    {
        (void)context;
        if (arguments.size() != 1)
        {
            std::cerr << "sin needs exactly 1 argument" << std::endl;
            return std::numeric_limits<double>::quiet_NaN();
        }
        return sin(arguments[0]);
    }

    unsigned GetNameId() const override
    {
        return 0;
    }
};

class CRandFunction : public IFunctionAST
{
public:
    double Call(CInterpreterContext &context, const std::vector<double> &arguments) const override
    {
        (void)context;
        if (arguments.size() != 2)
        {
            std::cerr << "rand needs exactly 2 argument" << std::endl;
            return std::numeric_limits<double>::quiet_NaN();
        }
        if (arguments[0] > arguments[1])
        {
            std::cerr << "rand: first argument must be less than or equal to second" << std::endl;
            return std::numeric_limits<double>::quiet_NaN();
        }
        double rand0to1 = double(std::rand()) / std::numeric_limits<unsigned>::max();
        return (arguments[1] - arguments[0]) * rand0to1 + arguments[0];
    }

    unsigned GetNameId() const override
    {
        return 0;
    }
};

CInterpreterContext::CInterpreterContext(CStringPool &pool)
    : m_pool(pool)
{
    AddBuiltin("sin", std::unique_ptr<IFunctionAST>(new CSinFunction));
    AddBuiltin("rand", std::unique_ptr<IFunctionAST>(new CRandFunction));
}

std::unique_ptr<CVariablesScope> CInterpreterContext::MakeScope()
{
    return std::unique_ptr<CVariablesScope>(new CVariablesScope(*this));
}

CVariablesScope &CInterpreterContext::GetCurrentScope()
{
    return *m_scopes.top();
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

void CInterpreterContext::AddBuiltin(const std::string &name, std::unique_ptr<IFunctionAST> &&function)
{
    m_builtins.emplace_back(std::move(function));
    unsigned nameRand = m_pool.Insert(name);
    m_functions[nameRand] = m_builtins.back().get();
}

void CInterpreterContext::EnterScope(CVariablesScope &scope)
{
    m_scopes.push(&scope);
}

void CInterpreterContext::ExitScope()
{
    m_scopes.pop();
}

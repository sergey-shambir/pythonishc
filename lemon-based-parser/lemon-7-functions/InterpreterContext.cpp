#include "BatchAST.h"
#include "InterpreterContext.h"
#include "StringPool.h"
#include "VariablesScope.h"
#include <iostream>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm.hpp>
#include <cmath>

class CSinFunction : public IFunctionAst
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
        return std::sin(arguments[0]);
    }

    unsigned GetNameId() const override
    {
        return 0;
    }
};

class CRandFunction : public IFunctionAst
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
    AddBuiltin("sin", std::unique_ptr<IFunctionAst>(new CSinFunction));
    AddBuiltin("rand", std::unique_ptr<IFunctionAst>(new CRandFunction));
}

CInterpreterContext::~CInterpreterContext()
{
}

void CInterpreterContext::DefineVariable(unsigned nameId, double value)
{
    m_scopes.back()->AssignVariable(nameId, value);
}

void CInterpreterContext::AssignVariable(unsigned nameId, double value)
{
    if (CVariablesScope *pScope = FindScopeWithVariable(nameId))
    {
        pScope->AssignVariable(nameId, value);
    }
    else
    {
        DefineVariable(nameId, value);
    }
}

double CInterpreterContext::GetVariableValue(unsigned nameId) const
{
    if (CVariablesScope *pScope = FindScopeWithVariable(nameId))
    {
        return *pScope->GetVariableValue(nameId);
    }
    std::cerr << "error: unknown variable " << m_pool.GetString(nameId) << std::endl;
    return std::numeric_limits<double>::quiet_NaN();
}

void CInterpreterContext::PushScope(std::unique_ptr<CVariablesScope> &&scope)
{
    m_scopes.emplace_back(std::move(scope));
}

std::unique_ptr<CVariablesScope> CInterpreterContext::PopScope()
{
    std::unique_ptr<CVariablesScope> ret(m_scopes.back().release());
    m_scopes.pop_back();
    return ret;
}

size_t CInterpreterContext::GetScopesCount() const
{
    return m_scopes.size();
}

IFunctionAst *CInterpreterContext::GetFunction(unsigned nameId) const
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

void CInterpreterContext::AddFunction(unsigned nameId, IFunctionAst *function)
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

CVariablesScope *CInterpreterContext::FindScopeWithVariable(unsigned nameId) const
{
    auto range = boost::adaptors::reverse(m_scopes);
    auto it = boost::find_if(range, [=](const auto &pScope) {
        return pScope->HasVariable(nameId);
    });
    if (it != range.end())
    {
        return it->get();
    }
    return nullptr;
}

void CInterpreterContext::AddBuiltin(const std::string &name, std::unique_ptr<IFunctionAst> &&function)
{
    m_builtins.emplace_back(std::move(function));
    unsigned nameRand = m_pool.Insert(name);
    m_functions[nameRand] = m_builtins.back().get();
}

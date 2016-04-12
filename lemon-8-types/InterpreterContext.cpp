#include "BatchAST.h"
#include "InterpreterContext.h"
#include "StringPool.h"
#include <iostream>

namespace
{

template <class TFunction>
CValue ExecuteSafely(TFunction && fn)
{
    try
    {
        return fn();
    }
    catch (std::exception const&)
    {
        return CValue::FromError(std::current_exception());
    }
}

class CSinFunction : public IFunctionAST
{
public:
    CValue Call(CInterpreterContext &context, const std::vector<CValue> &arguments) const override
    {
        (void)context;
        return ExecuteSafely([&] {
            double radians = arguments.at(0).AsDouble();
            return CValue::FromDouble(sin(radians));
        });
    }

    unsigned GetNameId() const override
    {
        return 0;
    }
};

class CRandFunction : public IFunctionAST
{
public:
    CValue Call(CInterpreterContext &context, const std::vector<CValue> &arguments) const override
    {
        (void)context;
        return ExecuteSafely([&] {
            double minimum = arguments.at(0).AsDouble();
            double maximum = arguments.at(1).AsDouble();
            if (minimum > maximum)
            {
                return CValue::FromErrorMessage("invalid arguments for rand - range maximum is lesser than minimum.");
            }
            double rand0to1 = double(std::rand()) / std::numeric_limits<unsigned>::max();

            return CValue::FromDouble((maximum - minimum) * rand0to1 + minimum);
        });
    }

    unsigned GetNameId() const override
    {
        return 0;
    }
};

}

CInterpreterContext::CInterpreterContext(CStringPool &pool)
    : m_pool(pool)
{
    AddBuiltin("sin", std::unique_ptr<IFunctionAST>(new CSinFunction));
    AddBuiltin("rand", std::unique_ptr<IFunctionAST>(new CRandFunction));
}

CInterpreterContext::~CInterpreterContext()
{
}

std::unique_ptr<CVariablesScope> CInterpreterContext::MakeScope()
{
    return std::unique_ptr<CVariablesScope>(new CVariablesScope(*this));
}

CVariablesScope &CInterpreterContext::GetCurrentScope()
{
    return *m_scopes.top();
}

void CInterpreterContext::AssignVariable(unsigned nameId, CValue const& value)
{
    if (value.IsError())
    {
        std::cerr << "  " << value << std::endl;
    }
    else
    {
        m_variables[nameId] = value;
    }
}

bool CInterpreterContext::HasVariable(unsigned nameId) const
{
    return (0 != m_variables.count(nameId));
}

void CInterpreterContext::RemoveVariable(unsigned nameId)
{
    m_variables.erase(nameId);
}

CValue CInterpreterContext::GetVariableValue(unsigned nameId) const
{
    try
    {
        return m_variables.at(nameId);
    }
    catch (std::exception const&)
    {
        std::string message = "unknown variable " + m_pool.GetString(nameId);
        return CValue::FromErrorMessage(message);
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

std::string CInterpreterContext::GetStringLiteral(unsigned stringId) const
{
    return m_pool.GetString(stringId);
}

void CInterpreterContext::PrintResult(CValue const& value)
{
    std::cerr << "  " << value << std::endl;
}

void CInterpreterContext::SetReturnValue(boost::optional<CValue> const& valueOpt)
{
    m_returnValueOpt = valueOpt;
}

boost::optional<CValue> CInterpreterContext::GetReturnValue() const
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

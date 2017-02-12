#include "BatchAST.h"
#include "InterpreterContext.h"
#include "VariablesScope.h"
#include <limits>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cassert>
#include <boost/range/adaptor/reversed.hpp>

namespace
{
double GetNaN()
{
    return std::numeric_limits<double>::quiet_NaN();
}

bool IsTrueConvertible(double value)
{
    return fabs(value) > std::numeric_limits<double>::epsilon();
}

void ExecuteAll(StatementsList const& list, CInterpreterContext & context)
{
    for (auto const& stmt : list)
    {
        stmt->Execute(context);
    }
}
}

class CScopedVariableScope
{
public:
    CScopedVariableScope(CInterpreterContext &context)
        : m_context(context)
    {
        m_context.PushScope(std::make_unique<CVariablesScope>());
    }

    ~CScopedVariableScope()
    {
        m_context.PopScope();
    }

private:
    CInterpreterContext &m_context;
};

class CVariableScopeCleaner
{
public:
    CVariableScopeCleaner(CInterpreterContext &context)
        : m_context(context)
    {
        // Убираем все области видимости, кроме верхней.
        while (m_context.GetScopesCount() > 1)
        {
            m_scopes.emplace_back(m_context.PopScope());
        }
    }

    ~CVariableScopeCleaner()
    {
        for (std::unique_ptr<CVariablesScope> &pScope : m_scopes)
        {
            m_context.PushScope(std::unique_ptr<CVariablesScope>(pScope.release()));
        }
    }

private:
    CInterpreterContext &m_context;
    std::vector<std::unique_ptr<CVariablesScope>> m_scopes;
};

CPrintAst::CPrintAst(IExpressionAstUniquePtr &&expr)
    : m_expr(std::move(expr))
{
}

void CPrintAst::Execute(CInterpreterContext &context)const
{
    const double result = m_expr->Evaluate(context);
    context.PrintResult(result);
}

CAssignAst::CAssignAst(unsigned nameId, IExpressionAstUniquePtr &&value)
    : m_nameId(nameId)
    , m_value(std::move(value))
{
}

void CAssignAst::Execute(CInterpreterContext &context)const
{
    const double value = m_value->Evaluate(context);
    context.AssignVariable(m_nameId, value);
}

CBinaryExpressionAst::CBinaryExpressionAst(IExpressionAstUniquePtr &&left, BinaryOperation op, IExpressionAstUniquePtr &&right)
    : m_left(std::move(left))
    , m_operation(op)
    , m_right(std::move(right))
{
}

double CBinaryExpressionAst::Evaluate(CInterpreterContext &context) const
{
    const double a = m_left->Evaluate(context);
    const double b = m_right->Evaluate(context);
    switch (m_operation)
    {
    case BinaryOperation::Add:
        return a + b;
    case BinaryOperation::Substract:
        return a - b;
    case BinaryOperation::Multiply:
        return a * b;
    case BinaryOperation::Divide:
        return a / b;
    case BinaryOperation::Modulo:
        return fmod(a, b);
    }
    assert(false); // unknown operation.
    return GetNaN();
}

CUnaryExpressionAst::CUnaryExpressionAst(UnaryOperation op, IExpressionAstUniquePtr &&value)
    : m_operation(op)
    , m_expr(std::move(value))
{
}

double CUnaryExpressionAst::Evaluate(CInterpreterContext &context) const
{
    const double value = m_expr->Evaluate(context);
    switch (m_operation)
    {
    case UnaryOperation::Plus:
        return +value;
    case UnaryOperation::Minus:
        return -value;
    }
    assert(false); // unknown operation.
    return GetNaN();
}

CLiteralAst::CLiteralAst(double value)
    : m_value(value)
{
}

double CLiteralAst::Evaluate(CInterpreterContext &context) const
{
    (void)context;
    return m_value;
}

CVariableRefAst::CVariableRefAst(unsigned nameId)
    : m_nameId(nameId)
{
}

double CVariableRefAst::Evaluate(CInterpreterContext &context) const
{
    return context.GetVariableValue(m_nameId);
}

CIfAst::CIfAst(IExpressionAstUniquePtr &&condition, StatementsList &&thenBody, StatementsList &&elseBody)
    : m_condition(std::move(condition))
    , m_thenBody(std::move(thenBody))
    , m_elseBody(std::move(elseBody))
{
}

void CIfAst::Execute(CInterpreterContext &context) const
{
    double result = m_condition->Evaluate(context);
    if (IsTrueConvertible(result))
    {
        ExecuteAll(m_thenBody, context);
    }
    else
    {
        ExecuteAll(m_elseBody, context);
    }
}

CProgramAst::CProgramAst(CInterpreterContext &context)
    : m_context(context)
{
    m_context.PushScope(std::make_unique<CVariablesScope>());
}

CProgramAst::~CProgramAst()
{
    m_context.PopScope();
}

void CProgramAst::AddStatement(IStatementAstUniquePtr &&stmt)
{
    stmt->Execute(m_context);
}

void CProgramAst::AddFunction(IFunctionAstUniquePtr &&function)
{
    unsigned nameId = function->GetNameId();
    m_context.AddFunction(nameId, function.get());
    m_functions.emplace_back(std::move(function));
}

CWhileAst::CWhileAst(IExpressionAstUniquePtr &&condition, StatementsList &&body)
    : m_condition(std::move(condition))
    , m_body(std::move(body))
{
}

void CWhileAst::Execute(CInterpreterContext &context) const
{
    while (IsTrueConvertible(m_condition->Evaluate(context)))
    {
        ExecuteAll(m_body, context);
    }
}

CRepeatAst::CRepeatAst(IExpressionAstUniquePtr &&condition, StatementsList &&body)
    : m_condition(std::move(condition))
    , m_body(std::move(body))
{
}

void CRepeatAst::Execute(CInterpreterContext &context) const
{
    do
    {
        ExecuteAll(m_body, context);
    }
    while (IsTrueConvertible(m_condition->Evaluate(context)));
}

CCallAst::CCallAst(unsigned nameId, ExpressionList && arguments)
    : m_nameId(nameId)
    , m_arguments(std::move(arguments))
{
}

double CCallAst::Evaluate(CInterpreterContext &context) const
{
    if (IFunctionAst *func = context.GetFunction(m_nameId))
    {
        std::vector<double> args(m_arguments.size());
        std::transform(m_arguments.begin(), m_arguments.end(), args.begin(), [&](IExpressionAstUniquePtr const& ast) {
            return ast->Evaluate(context);
        });
        return func->Call(context, args);
    }
    return GetNaN();
}

CFunctionAst::CFunctionAst(unsigned nameId, std::vector<unsigned> argumentNames, StatementsList && body)
    : m_nameId(nameId)
    , m_argumentNames(argumentNames)
    , m_body(std::move(body))
{
}

unsigned CFunctionAst::GetNameId() const
{
    return m_nameId;
}

double CFunctionAst::Call(CInterpreterContext &context, const std::vector<double> &arguments) const
{
    if (arguments.size() != m_argumentNames.size())
    {
        std::cerr << "arguments and parameters count mismatch" << std::endl;
        return GetNaN();
    }

    CVariableScopeCleaner cleaner(context);
    CScopedVariableScope scopedScope(context);

    auto argumentIt = arguments.begin();
    for (unsigned nameId : m_argumentNames)
    {
        context.DefineVariable(nameId, *argumentIt);
        ++argumentIt;
    }

    boost::optional<double> returnedValue;
    for (IStatementAstUniquePtr const& stmt : m_body)
    {
        stmt->Execute(context);
        returnedValue = context.GetReturnValue();
        if (returnedValue)
        {
            context.SetReturnValue(boost::none);
            break;
        }
    }

    return returnedValue.get_value_or(GetNaN());
}

CReturnAst::CReturnAst(IExpressionAstUniquePtr &&value)
    : m_value(std::move(value))
{
}

void CReturnAst::Execute(CInterpreterContext &context) const
{
    double result = m_value->Evaluate(context);
    context.SetReturnValue(result);
}

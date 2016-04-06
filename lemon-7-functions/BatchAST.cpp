#include "BatchAST.h"
#include "InterpreterContext.h"
#include <limits>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cassert>

namespace
{
double GetNaN()
{
    return std::numeric_limits<double>::quiet_NaN();
}

bool IsNonZero(double value)
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

CPrintAST::CPrintAST(IExpressionASTUniquePtr &&expr)
    : m_expr(std::move(expr))
{
}

void CPrintAST::Execute(CInterpreterContext &context)const
{
    const double result = m_expr->Evaluate(context);
    context.PrintResult(result);
}

CAssignAST::CAssignAST(unsigned nameId, IExpressionASTUniquePtr &&value)
    : m_nameId(nameId)
    , m_value(std::move(value))
{
}

void CAssignAST::Execute(CInterpreterContext &context)const
{
    const double value = m_value->Evaluate(context);
    context.GetCurrentScope().AssignVariable(m_nameId, value);
}

CBinaryExpressionAST::CBinaryExpressionAST(IExpressionASTUniquePtr &&left, BinaryOperation op, IExpressionASTUniquePtr &&right)
    : m_left(std::move(left))
    , m_operation(op)
    , m_right(std::move(right))
{
}

double CBinaryExpressionAST::Evaluate(CInterpreterContext &context) const
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

CUnaryExpressionAST::CUnaryExpressionAST(UnaryOperation op, IExpressionASTUniquePtr &&value)
    : m_operation(op)
    , m_expr(std::move(value))
{
}

double CUnaryExpressionAST::Evaluate(CInterpreterContext &context) const
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

CLiteralAST::CLiteralAST(double value)
    : m_value(value)
{
}

double CLiteralAST::Evaluate(CInterpreterContext &context) const
{
    (void)context;
    return m_value;
}

CVariableRefAST::CVariableRefAST(unsigned nameId)
    : m_nameId(nameId)
{
}

double CVariableRefAST::Evaluate(CInterpreterContext &context) const
{
    return context.GetCurrentScope().GetVariableValue(m_nameId);
}

CIfAst::CIfAst(IExpressionASTUniquePtr &&condition, StatementsList &&thenBody, StatementsList &&elseBody)
    : m_condition(std::move(condition))
    , m_thenBody(std::move(thenBody))
    , m_elseBody(std::move(elseBody))
{
}

void CIfAst::Execute(CInterpreterContext &context) const
{
    double result = m_condition->Evaluate(context);
    if (IsNonZero(result))
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
    , m_pScope(std::move(context.MakeScope()))
{
}

CProgramAst::~CProgramAst()
{
}

void CProgramAst::AddStatement(IStatementASTUniquePtr &&stmt)
{
    stmt->Execute(m_context);
}

void CProgramAst::AddFunction(IFunctionASTUniquePtr &&function)
{
    unsigned nameId = function->GetNameId();
    m_context.AddFunction(nameId, function.get());
    m_functions.emplace_back(std::move(function));
}

CWhileAst::CWhileAst(IExpressionASTUniquePtr &&condition, StatementsList &&body)
    : m_condition(std::move(condition))
    , m_body(std::move(body))
{
}

void CWhileAst::Execute(CInterpreterContext &context) const
{
    while (IsNonZero(m_condition->Evaluate(context)))
    {
        ExecuteAll(m_body, context);
    }
}

CRepeatAst::CRepeatAst(IExpressionASTUniquePtr &&condition, StatementsList &&body)
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
    while (IsNonZero(m_condition->Evaluate(context)));
}

CCallAST::CCallAST(unsigned nameId, ExpressionList && arguments)
    : m_nameId(nameId)
    , m_arguments(std::move(arguments))
{
}

double CCallAST::Evaluate(CInterpreterContext &context) const
{
    if (IFunctionAST *func = context.GetFunction(m_nameId))
    {
        std::vector<double> args(m_arguments.size());
        std::transform(m_arguments.begin(), m_arguments.end(), args.begin(), [&](IExpressionASTUniquePtr const& ast) {
            return ast->Evaluate(context);
        });
        return func->Call(context, args);
    }
    return GetNaN();
}

CFunctionAST::CFunctionAST(unsigned nameId, std::vector<unsigned> argumentNames, StatementsList && body)
    : m_nameId(nameId)
    , m_argumentNames(argumentNames)
    , m_body(std::move(body))
{
}

unsigned CFunctionAST::GetNameId() const
{
    return m_nameId;
}

double CFunctionAST::Call(CInterpreterContext &context, const std::vector<double> &arguments) const
{
    if (arguments.size() != m_argumentNames.size())
    {
        std::cerr << "arguments and parameters count mismatch" << std::endl;
        return GetNaN();
    }

    std::unique_ptr<CVariablesScope> scope = context.MakeScope();
    auto argumentIt = arguments.begin();
    for (unsigned nameId : m_argumentNames)
    {
        scope->AssignVariable(nameId, *argumentIt);
        ++argumentIt;
    }

    boost::optional<double> returnedValue;
    for (IStatementASTUniquePtr const& stmt : m_body)
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

CReturnAST::CReturnAST(IExpressionASTUniquePtr &&value)
    : m_value(std::move(value))
{
}

void CReturnAST::Execute(CInterpreterContext &context) const
{
    double result = m_value->Evaluate(context);
    context.SetReturnValue(result);
}

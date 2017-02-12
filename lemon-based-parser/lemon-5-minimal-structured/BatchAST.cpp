#include "BatchAST.h"
#include "InterpreterContext.h"
#include <limits>
#include <cmath>
#include <cassert>


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
    return std::numeric_limits<double>::quiet_NaN();
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
    return std::numeric_limits<double>::quiet_NaN();
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

void CAbstractBlockAST::AddStatement(IStatementAstUniquePtr &&stmt)
{
    m_body.push_back(std::move(stmt));
}

void CAbstractBlockAST::ExecuteBody(CInterpreterContext & context) const
{
    for (IStatementAstUniquePtr const& stmt : m_body)
    {
        stmt->Execute(context);
    }
}

void CAbstractBlockAST::ExecuteLast(CInterpreterContext &context) const
{
    if (!m_body.empty())
    {
        m_body.back()->Execute(context);
    }
}

CIfAst::CIfAst(IExpressionAstUniquePtr &&condition)
    : m_condition(std::move(condition))
{
}

void CIfAst::Execute(CInterpreterContext &context) const
{
    double result = m_condition->Evaluate(context);
    if (fabs(result) > std::numeric_limits<double>::epsilon())
    {
        ExecuteBody(context);
    }
}

CProgramAst::CProgramAst(CInterpreterContext &context)
    : m_context(context)
{
}

void CProgramAst::AddStatement(IStatementAstUniquePtr &&stmt)
{
    CAbstractBlockAST::AddStatement(std::move(stmt));
    ExecuteLast(m_context);
}

void CProgramAst::Execute(CInterpreterContext &context) const
{
    ExecuteBody(context);
}

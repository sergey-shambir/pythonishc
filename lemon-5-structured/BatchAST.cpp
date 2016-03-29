#include "BatchAST.h"
#include "InterpreterContext.h"
#include <limits>
#include <cmath>
#include <cassert>


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
    context.AssignVariable(m_nameId, value);
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
    return std::numeric_limits<double>::quiet_NaN();
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
    return std::numeric_limits<double>::quiet_NaN();
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
    return context.GetVariableValue(m_nameId);
}

void CAbstractBlockAST::AddStatement(IStatementASTUniquePtr &&stmt)
{
    m_body.push_back(std::move(stmt));
}

void CAbstractBlockAST::ExecuteBody(CInterpreterContext & context) const
{
    for (IStatementASTUniquePtr const& stmt : m_body)
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

CIfAst::CIfAst(IExpressionASTUniquePtr &&condition)
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

void CProgramAst::AddStatement(IStatementASTUniquePtr &&stmt)
{
    CAbstractBlockAST::AddStatement(std::move(stmt));
    ExecuteLast(m_context);
}

void CProgramAst::Execute(CInterpreterContext &context) const
{
    ExecuteBody(context);
}

CWhileAst::CWhileAst(IExpressionASTUniquePtr &&condition)
    : m_condition(std::move(condition))
{
}

void CWhileAst::Execute(CInterpreterContext &context) const
{
    double result = m_condition->Evaluate(context);
    while (fabs(result) > std::numeric_limits<double>::epsilon())
    {
        ExecuteBody(context);
        result = m_condition->Evaluate(context);
    }
}

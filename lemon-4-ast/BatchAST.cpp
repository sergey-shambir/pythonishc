#include "BatchAST.h"
#include "InterpreterContext.h"
#include <limits>
#include <cmath>
#include <cassert>


CPrintAST::CPrintAST(IExpressionAST *value)
    : m_value(value)
{
}

void CPrintAST::Execute(CInterpreterContext &context)const
{
    const double result = m_value->Evaluate(context);
    context.PrintResult(result);
}

CAssignAST::CAssignAST(unsigned nameId, IExpressionAST *value)
    : m_nameId(nameId)
    , m_value(value)
{
}

void CAssignAST::Execute(CInterpreterContext &context)const
{
    const double value = m_value->Evaluate(context);
    context.AssignVariable(m_nameId, value);
}

CBinaryExpressionAST::CBinaryExpressionAST(IExpressionAST *left, BinaryOperation op, IExpressionAST *right)
    : m_left(left)
    , m_operation(op)
    , m_right(right)
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

CUnaryExpressionAST::CUnaryExpressionAST(UnaryOperation op, IExpressionAST *value)
    : m_operation(op)
    , m_value(value)
{
}

double CUnaryExpressionAST::Evaluate(CInterpreterContext &context) const
{
    const double value = m_value->Evaluate(context);
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

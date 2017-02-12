#include "BatchAST.h"
#include "InterpreterContext.h"
#include <limits>
#include <cmath>
#include <cassert>


CPrintAst::CPrintAst(IExpressionAst *value)
    : m_value(value)
{
}

void CPrintAst::Execute(CInterpreterContext &context)const
{
    const double result = m_value->Evaluate(context);
    context.PrintResult(result);
}

CAssignAst::CAssignAst(unsigned nameId, IExpressionAst *value)
    : m_nameId(nameId)
    , m_value(value)
{
}

void CAssignAst::Execute(CInterpreterContext &context)const
{
    const double value = m_value->Evaluate(context);
    context.AssignVariable(m_nameId, value);
}

CBinaryExpressionAst::CBinaryExpressionAst(IExpressionAst *left, BinaryOperation op, IExpressionAst *right)
    : m_left(left)
    , m_operation(op)
    , m_right(right)
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

CUnaryExpressionAst::CUnaryExpressionAst(UnaryOperation op, IExpressionAst *value)
    : m_operation(op)
    , m_value(value)
{
}

double CUnaryExpressionAst::Evaluate(CInterpreterContext &context) const
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

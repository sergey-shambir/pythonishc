#pragma once

#include <memory>

class CInterpreterContext;

class IAnyAst
{
public:
    virtual ~IAnyAst() = default;
};

class IExpressionAst : public IAnyAst
{
public:
    virtual double Evaluate(CInterpreterContext & context)const = 0;
};

class IStatementAst : public IAnyAst
{
public:
    virtual void Execute(CInterpreterContext & context)const = 0;
};

enum class BinaryOperation
{
    Add,
    Substract,
    Multiply,
    Divide,
    Modulo
};

class CBinaryExpressionAst : public IExpressionAst
{
public:
    CBinaryExpressionAst(IExpressionAst *left, BinaryOperation op, IExpressionAst *right);
    double Evaluate(CInterpreterContext & context)const override;

private:
    IExpressionAst *m_left;
    const BinaryOperation m_operation;
    IExpressionAst *m_right;
};

enum class UnaryOperation
{
    Plus,
    Minus
};

class CUnaryExpressionAst : public IExpressionAst
{
public:
    CUnaryExpressionAst(UnaryOperation op, IExpressionAst *value);
    double Evaluate(CInterpreterContext & context)const override;

private:
    const UnaryOperation m_operation;
    IExpressionAst *m_value;
};

class CLiteralAst : public IExpressionAst
{
public:
    CLiteralAst(double value);
    double Evaluate(CInterpreterContext & context)const override;

private:
    const double m_value;
};

class CVariableRefAst : public IExpressionAst
{
public:
    CVariableRefAst(unsigned nameId);
    double Evaluate(CInterpreterContext & context)const override;

private:
    const unsigned m_nameId;
};

class CPrintAst : public IStatementAst
{
public:
    CPrintAst(IExpressionAst *value);
    void Execute(CInterpreterContext & context)const override;

private:
    IExpressionAst *m_value;
};

class CAssignAst : public IStatementAst
{
public:
    CAssignAst(unsigned nameId, IExpressionAst *value);
    void Execute(CInterpreterContext &context)const override;

private:
    const unsigned m_nameId;
    IExpressionAst *m_value;
};

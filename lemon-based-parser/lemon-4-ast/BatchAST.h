#pragma once

#include <memory>

class CInterpreterContext;

class IAnyAST
{
public:
    virtual ~IAnyAST() = default;
};

class IExpressionAST : public IAnyAST
{
public:
    virtual double Evaluate(CInterpreterContext & context)const = 0;
};

class IStatementAST : public IAnyAST
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

class CBinaryExpressionAST : public IExpressionAST
{
public:
    CBinaryExpressionAST(IExpressionAST *left, BinaryOperation op, IExpressionAST *right);
    double Evaluate(CInterpreterContext & context)const override;

private:
    IExpressionAST *m_left;
    const BinaryOperation m_operation;
    IExpressionAST *m_right;
};

enum class UnaryOperation
{
    Plus,
    Minus
};

class CUnaryExpressionAST : public IExpressionAST
{
public:
    CUnaryExpressionAST(UnaryOperation op, IExpressionAST *value);
    double Evaluate(CInterpreterContext & context)const override;

private:
    const UnaryOperation m_operation;
    IExpressionAST *m_value;
};

class CLiteralAST : public IExpressionAST
{
public:
    CLiteralAST(double value);
    double Evaluate(CInterpreterContext & context)const override;

private:
    const double m_value;
};

class CVariableRefAST : public IExpressionAST
{
public:
    CVariableRefAST(unsigned nameId);
    double Evaluate(CInterpreterContext & context)const override;

private:
    const unsigned m_nameId;
};

class CPrintAST : public IStatementAST
{
public:
    CPrintAST(IExpressionAST *value);
    void Execute(CInterpreterContext & context)const override;

private:
    IExpressionAST *m_value;
};

class CAssignAST : public IStatementAST
{
public:
    CAssignAST(unsigned nameId, IExpressionAST *value);
    void Execute(CInterpreterContext &context)const override;

private:
    const unsigned m_nameId;
    IExpressionAST *m_value;
};

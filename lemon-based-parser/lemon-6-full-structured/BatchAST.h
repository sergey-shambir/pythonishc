#pragma once

#include <memory>
#include <vector>

class CInterpreterContext;

class IExpressionAst
{
public:
    virtual ~IExpressionAst() = default;
    virtual double Evaluate(CInterpreterContext & context)const = 0;
};

class IStatementAst
{
public:
    virtual ~IStatementAst() = default;
    virtual void Execute(CInterpreterContext & context)const = 0;
};

using IExpressionAstUniquePtr = std::unique_ptr<IExpressionAst>;
using IStatementAstUniquePtr = std::unique_ptr<IStatementAst>;

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
    CBinaryExpressionAst(IExpressionAstUniquePtr && left, BinaryOperation op, IExpressionAstUniquePtr && right);
    double Evaluate(CInterpreterContext & context)const override;

private:
    IExpressionAstUniquePtr m_left;
    const BinaryOperation m_operation;
    IExpressionAstUniquePtr m_right;
};

enum class UnaryOperation
{
    Plus,
    Minus
};

class CUnaryExpressionAst : public IExpressionAst
{
public:
    CUnaryExpressionAst(UnaryOperation op, IExpressionAstUniquePtr && value);
    double Evaluate(CInterpreterContext & context)const override;

private:
    const UnaryOperation m_operation;
    IExpressionAstUniquePtr m_expr;
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
    CPrintAst(IExpressionAstUniquePtr && expr);

protected:
    void Execute(CInterpreterContext & context)const override;

private:
    IExpressionAstUniquePtr m_expr;
};

class CAssignAst : public IStatementAst
{
public:
    CAssignAst(unsigned nameId, IExpressionAstUniquePtr && value);

protected:
    void Execute(CInterpreterContext &context)const override;

private:
    const unsigned m_nameId;
    IExpressionAstUniquePtr m_value;
};

class CAbstractBlockAST : public IStatementAst
{
public:
    virtual void AddStatement(IStatementAstUniquePtr && stmt);

protected:
    void ExecuteBody(CInterpreterContext &context) const;
    void ExecuteLast(CInterpreterContext &context) const;

private:
    std::vector<IStatementAstUniquePtr> m_body;
};

class CWhileAst : public CAbstractBlockAST
{
public:
    CWhileAst(IExpressionAstUniquePtr && condition);

protected:
    void Execute(CInterpreterContext &context) const override;

private:
    IExpressionAstUniquePtr m_condition;
};

class CRepeatAst : public CAbstractBlockAST
{
public:
    void SetCondition(IExpressionAstUniquePtr && condition);

protected:
    void Execute(CInterpreterContext &context) const override;

private:
    IExpressionAstUniquePtr m_condition;
};

class CIfAst : public CAbstractBlockAST
{
public:
    CIfAst(IExpressionAstUniquePtr && condition);

    void SetElseStatement(IStatementAstUniquePtr && elseBlock);

protected:
    void Execute(CInterpreterContext &context) const override;

private:
    IExpressionAstUniquePtr m_condition;
    IStatementAstUniquePtr m_elseBlock;
};

class CBlockAst : public CAbstractBlockAST
{
protected:
    void Execute(CInterpreterContext &context) const override;
};

class CProgramAst : public CAbstractBlockAST
{
public:
    CProgramAst(CInterpreterContext &context);

    void AddStatement(IStatementAstUniquePtr && stmt) override;

protected:
    void Execute(CInterpreterContext &context) const override;

private:
    CInterpreterContext &m_context;
};

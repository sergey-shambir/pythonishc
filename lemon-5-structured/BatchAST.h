#pragma once

#include <memory>
#include <vector>

class CInterpreterContext;

class IExpressionAST
{
public:
    virtual ~IExpressionAST() = default;
    virtual double Evaluate(CInterpreterContext & context)const = 0;
};

class IStatementAST
{
public:
    virtual ~IStatementAST() = default;
    virtual void Execute(CInterpreterContext & context)const = 0;
};

using IExpressionASTUniquePtr = std::unique_ptr<IExpressionAST>;
using IStatementASTUniquePtr = std::unique_ptr<IStatementAST>;

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
    CBinaryExpressionAST(IExpressionASTUniquePtr && left, BinaryOperation op, IExpressionASTUniquePtr && right);
    double Evaluate(CInterpreterContext & context)const override;

private:
    IExpressionASTUniquePtr m_left;
    const BinaryOperation m_operation;
    IExpressionASTUniquePtr m_right;
};

enum class UnaryOperation
{
    Plus,
    Minus
};

class CUnaryExpressionAST : public IExpressionAST
{
public:
    CUnaryExpressionAST(UnaryOperation op, IExpressionASTUniquePtr && value);
    double Evaluate(CInterpreterContext & context)const override;

private:
    const UnaryOperation m_operation;
    IExpressionASTUniquePtr m_expr;
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
    CPrintAST(IExpressionASTUniquePtr && expr);

protected:
    void Execute(CInterpreterContext & context)const override;

private:
    IExpressionASTUniquePtr m_expr;
};

class CAssignAST : public IStatementAST
{
public:
    CAssignAST(unsigned nameId, IExpressionASTUniquePtr && value);

protected:
    void Execute(CInterpreterContext &context)const override;

private:
    const unsigned m_nameId;
    IExpressionASTUniquePtr m_value;
};

class CAbstractBlockAST : public IStatementAST
{
public:
    virtual void AddStatement(IStatementASTUniquePtr && stmt);

protected:
    void ExecuteBody(CInterpreterContext &context) const;
    void ExecuteLast(CInterpreterContext &context) const;

private:
    std::vector<IStatementASTUniquePtr> m_body;
};

class CWhileAst : public CAbstractBlockAST
{
public:
    CWhileAst(IExpressionASTUniquePtr && condition);

protected:
    void Execute(CInterpreterContext &context) const override;

private:
    IExpressionASTUniquePtr m_condition;
};

class CIfAst : public CAbstractBlockAST
{
public:
    CIfAst(IExpressionASTUniquePtr && condition);

protected:
    void Execute(CInterpreterContext &context) const override;

private:
    IExpressionASTUniquePtr m_condition;
};

class CProgramAst : public CAbstractBlockAST
{
public:
    CProgramAst(CInterpreterContext &context);

    void AddStatement(IStatementASTUniquePtr && stmt) override;

protected:
    void Execute(CInterpreterContext &context) const override;

private:
    CInterpreterContext &m_context;
};

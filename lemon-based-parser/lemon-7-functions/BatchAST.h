#pragma once

#include <memory>
#include <vector>

class CInterpreterContext;

class IExpressionAst;
class IStatementAst;
class IFunctionAst;

using IExpressionAstUniquePtr = std::unique_ptr<IExpressionAst>;
using IStatementAstUniquePtr = std::unique_ptr<IStatementAst>;
using IFunctionAstUniquePtr = std::unique_ptr<IFunctionAst>;
using ExpressionList = std::vector<IExpressionAstUniquePtr>;
using StatementsList = std::vector<IStatementAstUniquePtr>;
using FunctionList = std::vector<IFunctionAstUniquePtr>;

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

class IFunctionAst
{
public:
    virtual ~IFunctionAst() = default;
    virtual double Call(CInterpreterContext & context, std::vector<double> const& arguments)const = 0;
    virtual unsigned GetNameId()const = 0;
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

class CCallAst : public IExpressionAst
{
public:
    CCallAst(unsigned nameId, ExpressionList &&arguments);
    double Evaluate(CInterpreterContext & context)const override;

private:
    const unsigned m_nameId;
    ExpressionList m_arguments;
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

class CReturnAst : public IStatementAst
{
public:
    CReturnAst(IExpressionAstUniquePtr && value);

protected:
    void Execute(CInterpreterContext &context)const override;

private:
    IExpressionAstUniquePtr m_value;
};

class CWhileAst : public IStatementAst
{
public:
    CWhileAst(IExpressionAstUniquePtr && condition,
              StatementsList && body = StatementsList());

protected:
    void Execute(CInterpreterContext &context) const override;

private:
    IExpressionAstUniquePtr m_condition;
    StatementsList m_body;
};

class CRepeatAst : public IStatementAst
{
public:
    CRepeatAst(IExpressionAstUniquePtr && condition,
               StatementsList && body = StatementsList());

protected:
    void Execute(CInterpreterContext &context) const override;

private:
    IExpressionAstUniquePtr m_condition;
    StatementsList m_body;
};

class CIfAst : public IStatementAst
{
public:
    CIfAst(IExpressionAstUniquePtr && condition,
           StatementsList && thenBody = StatementsList(),
           StatementsList && elseBody = StatementsList());

protected:
    void Execute(CInterpreterContext &context) const override;

private:
    IExpressionAstUniquePtr m_condition;
    StatementsList m_thenBody;
    StatementsList m_elseBody;
};

class CFunctionAst : public IFunctionAst
{
public:
    CFunctionAst(unsigned nameId, std::vector<unsigned> argumentNames, StatementsList && body);

    unsigned GetNameId()const override;

protected:
    double Call(CInterpreterContext &context, const std::vector<double> &arguments) const override;

private:
    unsigned m_nameId;
    std::vector<unsigned> m_argumentNames;
    StatementsList m_body;
};

class CProgramAst
{
public:
    CProgramAst(CInterpreterContext &context);
    ~CProgramAst();

    void AddStatement(IStatementAstUniquePtr && stmt);
    void AddFunction(IFunctionAstUniquePtr && function);

private:
    CInterpreterContext &m_context;
    FunctionList m_functions;
};

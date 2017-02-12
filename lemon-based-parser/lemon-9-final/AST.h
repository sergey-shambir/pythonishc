#pragma once

#include <memory>
#include <vector>
#include "Value.h"

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

struct ErrorValue {};

class IExpressionAst
{
public:
    virtual ~IExpressionAst() = default;
    virtual CValue Evaluate(CInterpreterContext & context)const = 0;
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
    virtual CValue Call(CInterpreterContext & context, std::vector<CValue> const& arguments)const = 0;
    virtual unsigned GetNameId()const = 0;
};

enum class BinaryOperation
{
    Less,
    Equals,
    And,
    Or,
    Add,
    Substract,
    Multiply,
    Divide,
    Modulo,
};

class CBinaryExpressionAst : public IExpressionAst
{
public:
    CBinaryExpressionAst(IExpressionAstUniquePtr && left, BinaryOperation op, IExpressionAstUniquePtr && right);
    CValue Evaluate(CInterpreterContext & context)const override;

private:
    IExpressionAstUniquePtr m_left;
    const BinaryOperation m_operation;
    IExpressionAstUniquePtr m_right;
};

enum class UnaryOperation
{
    Plus,
    Minus,
    Not,
};

class CUnaryExpressionAst : public IExpressionAst
{
public:
    CUnaryExpressionAst(UnaryOperation op, IExpressionAstUniquePtr && value);
    CValue Evaluate(CInterpreterContext & context)const override;

private:
    const UnaryOperation m_operation;
    IExpressionAstUniquePtr m_expr;
};

class CLiteralAst : public IExpressionAst
{
public:
    CLiteralAst(CValue value);
    CValue Evaluate(CInterpreterContext & context)const override;

private:
    const CValue m_value;
};

class CCallAst : public IExpressionAst
{
public:
    CCallAst(unsigned nameId, ExpressionList &&arguments);
    CValue Evaluate(CInterpreterContext & context)const override;

private:
    const unsigned m_nameId;
    ExpressionList m_arguments;
};

class CVariableRefAst : public IExpressionAst
{
public:
    CVariableRefAst(unsigned nameId);
    CValue Evaluate(CInterpreterContext & context)const override;

private:
    const unsigned m_nameId;
};

class CPrintAst : public IStatementAst
{
public:
    CPrintAst(ExpressionList && expressions);

protected:
    void Execute(CInterpreterContext & context)const override;

private:
    ExpressionList m_expressions;
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
    CValue Call(CInterpreterContext &context, const std::vector<CValue> &arguments) const override;

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

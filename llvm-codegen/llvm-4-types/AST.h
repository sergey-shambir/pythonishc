#pragma once

#include <memory>
#include <vector>
#include "ASTVisitor.h"
#include <boost/variant.hpp>
#include <boost/optional.hpp>

class IExpressionAST;
class IStatementAST;
class IFunctionAST;
class CParameterDeclAST;

using IExpressionASTUniquePtr = std::unique_ptr<IExpressionAST>;
using IStatementASTUniquePtr = std::unique_ptr<IStatementAST>;
using IFunctionASTUniquePtr = std::unique_ptr<IFunctionAST>;
using CParameterDeclASTUniquePtr = std::unique_ptr<CParameterDeclAST>;
using ExpressionList = std::vector<IExpressionASTUniquePtr>;
using StatementsList = std::vector<IStatementASTUniquePtr>;
using FunctionList = std::vector<IFunctionASTUniquePtr>;
using ParameterDeclList = std::vector<CParameterDeclASTUniquePtr>;

enum class ExpressionType
{
    Boolean,
    Number,
    String,
};

class IExpressionAST
{
public:
    virtual ~IExpressionAST() = default;
    virtual void Accept(IExpressionVisitor & visitor) = 0;
    virtual ExpressionType GetType()const = 0;
};

class IStatementAST
{
public:
    virtual ~IStatementAST() = default;
    virtual void Accept(IStatementVisitor & visitor) = 0;
};

class IFunctionAST
{
public:
    virtual ~IFunctionAST() = default;
    virtual unsigned GetNameId()const = 0;
    virtual ExpressionType GetReturnType()const = 0;
    virtual const ParameterDeclList &GetParameters()const = 0;
    virtual const StatementsList &GetBody()const = 0;
};

class CAbstractExpressionAST : public IExpressionAST
{
public:
    ExpressionType GetType()const override;
    void SetType(ExpressionType type);

private:
    boost::optional<ExpressionType> m_type;
};

enum class BinaryOperation
{
    Less,
    Equals,
    Add,
    Substract,
    Multiply,
    Divide,
    Modulo
};

class CBinaryExpressionAST : public CAbstractExpressionAST
{
public:
    CBinaryExpressionAST(IExpressionASTUniquePtr && left, BinaryOperation op, IExpressionASTUniquePtr && right);
    void Accept(IExpressionVisitor & visitor) override;

    BinaryOperation GetOperation()const;
    IExpressionAST &GetLeft();
    IExpressionAST &GetRight();

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

class CUnaryExpressionAST : public CAbstractExpressionAST
{
public:
    CUnaryExpressionAST(UnaryOperation op, IExpressionASTUniquePtr && value);
    void Accept(IExpressionVisitor & visitor) override;

    UnaryOperation GetOperation()const;
    IExpressionAST &GetOperand();

private:
    const UnaryOperation m_operation;
    IExpressionASTUniquePtr m_expr;
};

class CLiteralAST : public IExpressionAST
{
public:
    typedef boost::variant<
        bool,
        double,
        std::string
    > Value;

    CLiteralAST(Value const& value);
    void Accept(IExpressionVisitor & visitor) override;
    ExpressionType GetType()const override;

    const Value &GetValue()const;

private:
    const Value m_value;
};

class CCallAST : public CAbstractExpressionAST
{
public:
    CCallAST(unsigned nameId, ExpressionList &&arguments);
    void Accept(IExpressionVisitor & visitor) override;

    unsigned GetFunctionNameId()const;
    const ExpressionList &GetArguments()const;

private:
    const unsigned m_nameId;
    ExpressionList m_arguments;
};

class CParameterDeclAST : public CAbstractExpressionAST
{
public:
    CParameterDeclAST(unsigned nameId, ExpressionType type);

    void Accept(IExpressionVisitor & visitor) override;
    const unsigned &GetName()const;

private:
    const unsigned m_nameId;
};

class CVariableRefAST : public CAbstractExpressionAST
{
public:
    CVariableRefAST(unsigned nameId);
    void Accept(IExpressionVisitor & visitor) override;

    unsigned GetNameId()const;

private:
    const unsigned m_nameId;
};

class CPrintAST : public IStatementAST
{
public:
    CPrintAST(IExpressionASTUniquePtr && expr);
    IExpressionAST &GetValue();

protected:
    void Accept(IStatementVisitor & visitor) override;

private:
    IExpressionASTUniquePtr m_expr;
};

class CAssignAST : public IStatementAST
{
public:
    CAssignAST(unsigned nameId, IExpressionASTUniquePtr && value);

    unsigned GetNameId()const;
    IExpressionAST & GetValue();

protected:
    void Accept(IStatementVisitor & visitor) override;

private:
    const unsigned m_nameId;
    IExpressionASTUniquePtr m_value;
};

class CReturnAST : public IStatementAST
{
public:
    CReturnAST(IExpressionASTUniquePtr && value);

    IExpressionAST &GetValue();

protected:
    void Accept(IStatementVisitor & visitor) override;

private:
    IExpressionASTUniquePtr m_value;
};

class CAbstractLoopAst : public IStatementAST
{
public:
    CAbstractLoopAst(IExpressionASTUniquePtr && condition,
              StatementsList && body);

    IExpressionAST &GetCondition()const;
    const StatementsList &GetBody()const;

private:
    IExpressionASTUniquePtr m_condition;
    StatementsList m_body;
};

class CWhileAst : public CAbstractLoopAst
{
public:
    CWhileAst(IExpressionASTUniquePtr && condition,
              StatementsList && body = StatementsList());

protected:
    void Accept(IStatementVisitor & visitor) override;
};

class CRepeatAst : public CAbstractLoopAst
{
public:
    CRepeatAst(IExpressionASTUniquePtr && condition,
               StatementsList && body = StatementsList());

protected:
    void Accept(IStatementVisitor & visitor) override;
};

class CIfAst : public IStatementAST
{
public:
    CIfAst(IExpressionASTUniquePtr && condition,
           StatementsList && thenBody = StatementsList(),
           StatementsList && elseBody = StatementsList());

    IExpressionAST &GetCondition()const;
    const StatementsList &GetThenBody()const;
    const StatementsList &GetElseBody()const;

protected:
    void Accept(IStatementVisitor & visitor) override;

private:
    IExpressionASTUniquePtr m_condition;
    StatementsList m_thenBody;
    StatementsList m_elseBody;
};

class CFunctionAST : public IFunctionAST
{
public:
    CFunctionAST(unsigned nameId, ExpressionType returnType, ParameterDeclList && parameters, StatementsList && body);

    unsigned GetNameId()const override;
    const ParameterDeclList &GetParameters()const override;
    const StatementsList &GetBody()const override;
    ExpressionType GetReturnType() const override;

private:
    unsigned m_nameId;
    ParameterDeclList m_parameters;
    StatementsList m_body;
    ExpressionType m_returnType;
};

class CProgramAst
{
public:
    CProgramAst();
    ~CProgramAst();

    void AddFunction(IFunctionASTUniquePtr && function);
    const FunctionList &GetFunctions()const;

private:
    FunctionList m_functions;
};

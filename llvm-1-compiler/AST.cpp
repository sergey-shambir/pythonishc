#include "AST.h"
#include "InterpreterContext.h"
#include "CodegenVisitor.h"
#include <limits>
#include <algorithm>
#include <iostream>
#include <cmath>
#include <cassert>

CPrintAST::CPrintAST(IExpressionASTUniquePtr &&expr)
    : m_expr(std::move(expr))
{
}

IExpressionAST &CPrintAST::GetValue()
{
    return *m_expr;
}

void CPrintAST::Accept(IStatementVisitor &visitor)
{
    visitor.Visit(*this);
}

CAssignAST::CAssignAST(unsigned nameId, IExpressionASTUniquePtr &&value)
    : m_nameId(nameId)
    , m_value(std::move(value))
{
}

unsigned CAssignAST::GetNameId() const
{
    return m_nameId;
}

void CAssignAST::Accept(IStatementVisitor &visitor)
{
    visitor.Visit(*this);
}

CBinaryExpressionAST::CBinaryExpressionAST(IExpressionASTUniquePtr &&left, BinaryOperation op, IExpressionASTUniquePtr &&right)
    : m_left(std::move(left))
    , m_operation(op)
    , m_right(std::move(right))
{
}

void CBinaryExpressionAST::Accept(IExpressionVisitor &visitor)
{
    visitor.Visit(*this);
}

BinaryOperation CBinaryExpressionAST::GetOperation() const
{
    return m_operation;
}

IExpressionAST &CBinaryExpressionAST::GetLeft()
{
    return *m_left;
}

IExpressionAST &CBinaryExpressionAST::GetRight()
{
    return *m_right;
}

CUnaryExpressionAST::CUnaryExpressionAST(UnaryOperation op, IExpressionASTUniquePtr &&value)
    : m_operation(op)
    , m_expr(std::move(value))
{
}

void CUnaryExpressionAST::Accept(IExpressionVisitor &visitor)
{
    visitor.Visit(*this);
}

UnaryOperation CUnaryExpressionAST::GetOperation() const
{
    return m_operation;
}

IExpressionAST &CUnaryExpressionAST::GetOperand()
{
    return *m_expr;
}

CLiteralAST::CLiteralAST(const Value &value)
    : m_value(value)
{
}

void CLiteralAST::Accept(IExpressionVisitor &visitor)
{
    visitor.Visit(*this);
}

const CLiteralAST::Value &CLiteralAST::GetValue() const
{
    return m_value;
}

CVariableRefAST::CVariableRefAST(unsigned nameId)
    : m_nameId(nameId)
{
}

void CVariableRefAST::Accept(IExpressionVisitor &visitor)
{
    visitor.Visit(*this);
}

unsigned CVariableRefAST::GetNameId() const
{
    return m_nameId;
}

CIfAst::CIfAst(IExpressionASTUniquePtr &&condition, StatementsList &&thenBody, StatementsList &&elseBody)
    : m_condition(std::move(condition))
    , m_thenBody(std::move(thenBody))
    , m_elseBody(std::move(elseBody))
{
}

void CIfAst::Accept(IStatementVisitor &visitor)
{
    visitor.Visit(*this);
}

CWhileAst::CWhileAst(IExpressionASTUniquePtr &&condition, StatementsList &&body)
    : m_condition(std::move(condition))
    , m_body(std::move(body))
{
}

void CWhileAst::Accept(IStatementVisitor &visitor)
{
    visitor.Visit(*this);
}

CRepeatAst::CRepeatAst(IExpressionASTUniquePtr &&condition, StatementsList &&body)
    : m_condition(std::move(condition))
    , m_body(std::move(body))
{
}

void CRepeatAst::Accept(IStatementVisitor &visitor)
{
    visitor.Visit(*this);
}

CCallAST::CCallAST(unsigned nameId, ExpressionList && arguments)
    : m_nameId(nameId)
    , m_arguments(std::move(arguments))
{
}

void CCallAST::Accept(IExpressionVisitor &visitor)
{
    visitor.Visit(*this);
}

unsigned CCallAST::GetFunctionNameId() const
{
    return m_nameId;
}

const ExpressionList &CCallAST::GetArguments() const
{
    return m_arguments;
}

CFunctionAST::CFunctionAST(unsigned nameId, std::vector<unsigned> argumentNames, StatementsList && body)
    : m_nameId(nameId)
    , m_argumentNames(argumentNames)
    , m_body(std::move(body))
{
}

unsigned CFunctionAST::GetNameId() const
{
    return m_nameId;
}

const std::vector<unsigned> &CFunctionAST::GetArgumentNames() const
{
    return m_argumentNames;
}

const StatementsList &CFunctionAST::GetBody() const
{
    return m_body;
}

CReturnAST::CReturnAST(IExpressionASTUniquePtr &&value)
    : m_value(std::move(value))
{
}

IExpressionAST &CReturnAST::GetValue()
{
    return *m_value;
}

void CReturnAST::Accept(IStatementVisitor &visitor)
{
    visitor.Visit(*this);
}

CProgramAst::CProgramAst(CInterpreterContext &context)
    : m_context(context)
    , m_pScope(std::move(context.MakeScope()))
    , m_pCodegen(new CCodeGenerator(context))
{
}

CProgramAst::~CProgramAst()
{
}

void CProgramAst::AddFunction(IFunctionASTUniquePtr &&function)
{
    unsigned nameId = function->GetNameId();
    llvm::Function *pFunction = m_pCodegen->AcceptFunction(*function);
    m_context.AddFunction(nameId, pFunction);
    m_functions.emplace_back(std::move(function));
}

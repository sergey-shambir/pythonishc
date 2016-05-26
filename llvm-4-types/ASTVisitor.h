#pragma once

class IExpressionAST;
class CBinaryExpressionAST;
class CUnaryExpressionAST;
class CLiteralAST;
class CCallAST;
class CVariableRefAST;
class CParameterDeclAST;

class IExpressionVisitor
{
public:
    virtual ~IExpressionVisitor() = default;
    virtual void Visit(CBinaryExpressionAST & expr) = 0;
    virtual void Visit(CUnaryExpressionAST & expr) = 0;
    virtual void Visit(CLiteralAST & expr) = 0;
    virtual void Visit(CCallAST & expr) = 0;
    virtual void Visit(CVariableRefAST & expr) = 0;
    virtual void Visit(CParameterDeclAST & expr) = 0;
};

class IStatementAST;
class CPrintAST;
class CAssignAST;
class CReturnAST;
class CWhileAst;
class CRepeatAst;
class CIfAst;

class IStatementVisitor
{
public:
    virtual ~IStatementVisitor() = default;
    virtual void Visit(CPrintAST & ast) = 0;
    virtual void Visit(CAssignAST & ast) = 0;
    virtual void Visit(CReturnAST & ast) = 0;
    virtual void Visit(CWhileAst & ast) = 0;
    virtual void Visit(CRepeatAst & ast) = 0;
    virtual void Visit(CIfAst & ast) = 0;
};

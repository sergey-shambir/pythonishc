#pragma once

class IExpressionAST;
class CBinaryExpressionAST;
class CUnaryExpressionAST;
class CLiteralAST;
class CCallAST;
class CVariableRefAST;

class IExpressionVisitor
{
public:
    virtual ~IExpressionVisitor() = default;
    virtual void Visit(CBinaryExpressionAST & expr) = 0;
    virtual void Visit(CUnaryExpressionAST & expr) = 0;
    virtual void Visit(CLiteralAST & expr) = 0;
    virtual void Visit(CCallAST & expr) = 0;
    virtual void Visit(CVariableRefAST & expr) = 0;
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
    virtual void Visit(CPrintAST & expr) = 0;
    virtual void Visit(CAssignAST & expr) = 0;
    virtual void Visit(CReturnAST & expr) = 0;
    virtual void Visit(CWhileAst & expr) = 0;
    virtual void Visit(CRepeatAst & expr) = 0;
    virtual void Visit(CIfAst & expr) = 0;
};

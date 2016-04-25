#pragma once

#include <stdint.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include "ASTVisitor.h"
#include "AST.h"
#include <vector>

class CInterpreterContext;

class CExpressionCodeGenerator : protected IExpressionVisitor
{
public:
    CExpressionCodeGenerator(llvm::IRBuilder<> & builder, CInterpreterContext & context);

    // Can throw std::exception.
    llvm::Value *Codegen(IExpressionAST & ast);

protected:
    void Visit(CBinaryExpressionAST &expr) override;
    void Visit(CUnaryExpressionAST &expr) override;
    void Visit(CLiteralAST &expr) override;
    void Visit(CCallAST &expr) override;
    void Visit(CVariableRefAST &expr) override;

private:
    // Стек используется для временного хранения
    // по мере рекурсивного обхода дерева выражения.
    std::vector<llvm::Value *> m_values;
    CInterpreterContext & m_context;
    llvm::IRBuilder<> & m_builder;
};

class CBlockCodeGenerator : protected IStatementVisitor
{
public:
    CBlockCodeGenerator(CInterpreterContext & context);

    void Codegen(llvm::BasicBlock & bb, const StatementsList &block);

    // IStatementVisitor interface
protected:
    void Visit(CPrintAST &ast) override;
    void Visit(CAssignAST &expr) override;
    void Visit(CReturnAST &expr) override;
    void Visit(CWhileAst &expr) override;
    void Visit(CRepeatAst &expr) override;
    void Visit(CIfAst &expr) override;

private:
    CInterpreterContext & m_context;
    llvm::IRBuilder<> m_builder;
    CExpressionCodeGenerator m_exprGen;
};

class CCodeGenerator
{
public:
    CCodeGenerator(CInterpreterContext & context);
    llvm::Function *AcceptFunction(IFunctionAST & ast);

private:
    llvm::Function *GenerateDeclaration(IFunctionAST & ast);
    bool GenerateDefinition(llvm::Function &fn, IFunctionAST & ast);

    CInterpreterContext & m_context;
};

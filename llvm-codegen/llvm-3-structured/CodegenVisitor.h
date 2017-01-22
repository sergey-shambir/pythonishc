#pragma once

#include <stdint.h>
#include "ASTVisitor.h"
#include "AST.h"
#include <vector>

#include "begin_llvm.h"
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include "end_llvm.h"

class CFrontendContext;

class CExpressionCodeGenerator : protected IExpressionVisitor
{
public:
    CExpressionCodeGenerator(llvm::IRBuilder<> & builder, CFrontendContext & context);

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
    CFrontendContext & m_context;
    llvm::IRBuilder<> & m_builder;
};

class CBlockCodeGenerator : protected IStatementVisitor
{
public:
    CBlockCodeGenerator(CFrontendContext & context);

    void Codegen(const std::vector<unsigned> &argumentNames, const StatementsList &block, llvm::Function & fn);
    void AddExitMain();

    // IStatementVisitor interface
protected:
    void Visit(CPrintAST &ast) override;
    void Visit(CAssignAST &ast) override;
    void Visit(CReturnAST &ast) override;
    void Visit(CWhileAst &ast) override;
    void Visit(CRepeatAst &ast) override;
    void Visit(CIfAst &ast) override;

private:
    void LoadParameters(llvm::Function &fn, const std::vector<unsigned> &parameterNames);
    void CodegenLoop(CAbstractLoopAst &ast, bool skipFirstCheck);
    void CodegenForAstList(const StatementsList &block);

    CFrontendContext & m_context;
    llvm::IRBuilder<> m_builder;
    CExpressionCodeGenerator m_exprGen;
};

class CCodeGenerator
{
public:
    CCodeGenerator(CFrontendContext & context);
    llvm::Function *AcceptFunction(IFunctionAST & ast);
    llvm::Function *AcceptMainFunction(IFunctionAST & ast);

private:
    llvm::Function *GenerateDeclaration(IFunctionAST & ast, bool isMain);
    bool GenerateDefinition(llvm::Function &fn, IFunctionAST & ast, bool isMain);

    CFrontendContext & m_context;
};

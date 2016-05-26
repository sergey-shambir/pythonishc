#pragma once

#include <stdint.h>
#include <vector>
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#pragma clang diagnostic pop
#include "ASTVisitor.h"
#include "AST.h"
#include "Utility.h"

class CFrontendContext;
namespace llvm
{
class AllocaInst;
class Function;
class LLVMContext;
class Module;
class Constant;
}

enum class BuiltinFunction
{
    PRINTF,
    STRCAT,
    STRDUP,
    FREE,
};

class CCodegenContext
{
public:
    CCodegenContext(CFrontendContext &context);
    ~CCodegenContext();

    std::string GetString(unsigned stringId)const;
    void PrintError(std::string const& message) const;
    llvm::LLVMContext &GetLLVMContext();
    llvm::Module &GetModule();
    CScopeChain<llvm::AllocaInst*> &GetVariables();
    CScopeChain<llvm::Function*> &GetFunctions();
    std::unordered_map<std::string, llvm::Constant *> GetStrings();
    llvm::Constant *AddStringLiteral(const std::string &value);

    llvm::Function *GetBuiltinFunction(BuiltinFunction id)const;

private:
    void InitLibCBuiltins();

    CFrontendContext &m_context;
    std::unique_ptr<llvm::LLVMContext> m_pLLVMContext;
    std::unique_ptr<llvm::Module> m_pModule;
    std::map<BuiltinFunction, llvm::Function*> m_builtinFunctions;
    CScopeChain<llvm::AllocaInst*> m_variables;
    CScopeChain<llvm::Function*> m_functions;
    std::unordered_map<std::string, llvm::Constant *> m_strings;
};

class CExpressionCodeGenerator : protected IExpressionVisitor
{
public:
    CExpressionCodeGenerator(llvm::IRBuilder<> & builder, CCodegenContext & context);

    // Can throw std::exception.
    llvm::Value *Codegen(IExpressionAST & ast);

protected:
    void Visit(CBinaryExpressionAST &expr) override;
    void Visit(CUnaryExpressionAST &expr) override;
    void Visit(CLiteralAST &expr) override;
    void Visit(CCallAST &expr) override;
    void Visit(CVariableRefAST &expr) override;
    void Visit(CParameterDeclAST & expr) override;

private:
    // Стек используется для временного хранения
    // по мере рекурсивного обхода дерева выражения.
    std::vector<llvm::Value *> m_values;
    CCodegenContext & m_context;
    llvm::IRBuilder<> & m_builder;
};

class CFunctionCodeGenerator : protected IStatementVisitor
{
public:
    CFunctionCodeGenerator(CCodegenContext & context);

    void Codegen(const ParameterDeclList &parameters, const StatementsList &block, llvm::Function & fn);
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
    void LoadParameters(llvm::Function &fn, const ParameterDeclList &parameterNames);
    void CodegenLoop(CAbstractLoopAst &ast, bool skipFirstCheck);
    void CodegenForAstList(const StatementsList &block);

    CCodegenContext & m_context;
    llvm::IRBuilder<> m_builder;
    CExpressionCodeGenerator m_exprGen;
};

class CCodeGenerator
{
public:
    CCodeGenerator(CCodegenContext & context);
    llvm::Function *AcceptFunction(IFunctionAST & ast);
    llvm::Function *AcceptMainFunction(IFunctionAST & ast);

private:
    llvm::Function *GenerateDeclaration(IFunctionAST & ast, bool isMain);
    bool GenerateDefinition(llvm::Function &fn, IFunctionAST & ast, bool isMain);

    CCodegenContext & m_context;
};

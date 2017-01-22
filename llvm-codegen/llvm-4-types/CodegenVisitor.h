#pragma once

#include <stdint.h>
#include <vector>
#include <unordered_set>
#include "ASTVisitor.h"
#include "AST.h"
#include "Utility.h"

#include "begin_llvm.h"
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include "end_llvm.h"

class CFrontendContext;
class CCodegenContext;
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
    STRCPY,
    STRDUP,
    STRLEN,
    STRCMP,
    MALLOC,
    FREE,
};

/*
 * Хранит регистры с указателями на строки, которыми никто не владеет.
 * Пока указателями никто не владеет, этот класс позволяет управлять их временем жизни.
 * Это один из элементов гарантии отсутствия утечек памяти.
 */
class CManagedStrings
{
public:
    CManagedStrings(CCodegenContext &context);
    ~CManagedStrings();

    // Вызывает free() для всех строк.
    void FreeAll(llvm::IRBuilder<> & builder);

    // Сбрасывает список неудалённых строк.
    void Clear();

    // Если строкой никто не владеет, снимает её с контроля и возвращает.
    // Иначе дублирует строку и возвращает дубликат.
    llvm::Value *TakeStringOrCopy(llvm::IRBuilder<> & builder, llvm::Value *pString);

    // Добавляет строку под контроль времени жизни.
    void Manage(llvm::Value *pString);

private:
    CCodegenContext &m_context;
    std::unordered_set<llvm::Value *> m_pointers;
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
    CManagedStrings &GetExpressionStrings();
    CManagedStrings &GetFunctionStrings();

private:
    void InitLibCBuiltins();

    CFrontendContext &m_context;
    std::unique_ptr<llvm::LLVMContext> m_pLLVMContext;
    std::unique_ptr<llvm::Module> m_pModule;
    std::map<BuiltinFunction, llvm::Function*> m_builtinFunctions;
    CScopeChain<llvm::AllocaInst*> m_variables;
    CScopeChain<llvm::Function*> m_functions;
    std::unordered_map<std::string, llvm::Constant *> m_strings;
    CManagedStrings m_expressionStrings;
    CManagedStrings m_functionStrings;
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
    llvm::Value *GenerateNumericExpr(llvm::Value *a, BinaryOperation op, llvm::Value *b);
    llvm::Value *GenerateStringExpr(llvm::Value *a, BinaryOperation op, llvm::Value *b);
    llvm::Value *GenerateBooleanExpr(llvm::Value *a, BinaryOperation op, llvm::Value *b);
    llvm::Value *GenerateStrcmp(llvm::Value *a, llvm::Value *b);

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
    void FillBlockAndJump(const StatementsList &statements,
                          llvm::BasicBlock *block, llvm::BasicBlock *nextBlock);
    llvm::Value *MakeValueCopy(llvm::Value *pValue);
    void FreeExpressionAllocs();
    void FreeFunctionAllocs();
    void RemoveUnusedBlocks(llvm::Function &fn);

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

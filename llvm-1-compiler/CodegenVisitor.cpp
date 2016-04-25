#include "CodegenVisitor.h"
#include "AST.h"
#include "InterpreterContext.h"
#include <llvm/IR/Constants.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/Support/raw_ostream.h>
#include <boost/variant.hpp>

using namespace llvm;

namespace
{

GlobalVariable *AddStringConstant(LLVMContext & context, Module & module, std::string const& value)
{
    // LLVM сам устранит дубликаты одинаковых строковых констант за счёт String Interning.
    Constant *pConstant = ConstantDataArray::getString(context, value, true);

    // Выделение глобальной переменной на куче безопасно
    // после создания объект передаст указатель на себя родителю
    // удалять указатель вручную нельзя.
    GlobalVariable *global = new GlobalVariable(module, pConstant->getType(), true,
                                                GlobalValue::InternalLinkage, pConstant, "str");

    return global;
}

Constant *GetStringConstantPtr(LLVMContext & context, GlobalVariable *pConstant)
{
    Constant *index = Constant::getNullValue(IntegerType::getInt32Ty(context));
    std::vector<Constant*> indices = {index, index};
    return ConstantExpr::getGetElementPtr(pConstant, indices);
}

// Генерирует код константы LLVM.
struct LiteralCodeGenerator : boost::static_visitor<Constant *>
{
    explicit LiteralCodeGenerator(LLVMContext &context, Module & module)
        : m_context(context)
        , m_module(module)
    {
    }

    Constant *operator ()(double const& value) const
    {
        return ConstantFP::get(m_context, APFloat(value));
    }

    Constant *operator ()(bool const& value) const
    {
        return ConstantInt::get(m_context, APInt(8, uint64_t(value), true));
    }

    Constant *operator ()(std::string const& value)
    {
        GlobalVariable *var = AddStringConstant(m_context, m_module, value);
        return GetStringConstantPtr(m_context, var);
    }

private:
    LLVMContext &m_context;
    Module &m_module;
};

Value *GenerateBinaryExpr(IRBuilder<> & builder, LLVMContext &context, Value *a, BinaryOperation op, Value *b)
{
    switch (op)
    {
    case BinaryOperation::Add:
        return builder.CreateFAdd(a, b, "addtmp");
    case BinaryOperation::Substract:
        return builder.CreateFSub(a, b, "subtmp");
    case BinaryOperation::Multiply:
        return builder.CreateFMul(a, b, "multmp");
    case BinaryOperation::Divide:
        return builder.CreateFDiv(a, b, "divtmp");
    case BinaryOperation::Modulo:
        return builder.CreateFRem(a, b, "modtmp");
    case BinaryOperation::Less:
    {
        Value *temp = builder.CreateFCmpULT(a, b, "cmptmp");
        // Convert bool 0/1 to double 0.0 or 1.0
        return builder.CreateUIToFP(temp, Type::getDoubleTy(context), "booltmp");
    }
    case BinaryOperation::Equals:
    {
        Value *temp = builder.CreateFCmpUEQ(a, b, "cmptmp");
        // Convert bool 0/1 to double 0.0 or 1.0
        return builder.CreateUIToFP(temp, Type::getDoubleTy(context), "booltmp");
    }
    }
    throw std::runtime_error("Unknown binary operation");
}

Value *GenerateUnaryExpr(IRBuilder<> & builder, LLVMContext &context, UnaryOperation op, Value *x)
{
    (void)context;
    switch (op)
    {
    case UnaryOperation::Plus:
        return x;
    case UnaryOperation::Minus:
        return builder.CreateFNeg(x, "negtmp");
    }
    throw std::runtime_error("Unknown unary operation");
}
}


CExpressionCodeGenerator::CExpressionCodeGenerator(llvm::IRBuilder<> &builder, CInterpreterContext &context)
    : m_context(context)
    , m_builder(builder)
{
}

Value *CExpressionCodeGenerator::Codegen(IExpressionAST &ast)
{
    llvm::Value *pValue = nullptr;
    try
    {
        ast.Accept(*this);
        pValue = m_values.at(0);
        m_values.clear();
    }
    catch (std::exception const& ex)
    {
        m_context.PrintError(ex.what());
    }
    return pValue;
}

void CExpressionCodeGenerator::Visit(CBinaryExpressionAST &expr)
{
    expr.GetLeft().Accept(*this);
    expr.GetRight().Accept(*this);
    Value *a = m_values.at(m_values.size() - 2);
    Value *b = m_values.at(m_values.size() - 1);
    m_values.pop_back();
    m_values.pop_back();

    auto pValue = GenerateBinaryExpr(m_builder, m_context.GetLLVMContext(), a, expr.GetOperation(), b);
    m_values.push_back(pValue);
}

void CExpressionCodeGenerator::Visit(CUnaryExpressionAST &expr)
{
    expr.GetOperand().Accept(*this);
    Value *x = m_values.at(m_values.size() - 1);
    m_values.pop_back();

    auto pValue = GenerateUnaryExpr(m_builder, m_context.GetLLVMContext(), expr.GetOperation(), x);
    m_values.push_back(pValue);
}

void CExpressionCodeGenerator::Visit(CLiteralAST &expr)
{
    LiteralCodeGenerator generator(m_context.GetLLVMContext(), m_context.GetModule());
    Value *pValue = expr.GetValue().apply_visitor(generator);
    m_values.push_back(pValue);
}

void CExpressionCodeGenerator::Visit(CCallAST &expr)
{
    Function *pFunction = m_context.TryGetFunction(expr.GetFunctionNameId());

    size_t parameterCount = pFunction->arg_size();
    size_t argumentCount = expr.GetArguments().size();

    if (parameterCount != argumentCount)
    {
        std::string message = "incorrect number of arguments passed: expected "
                + std::to_string(parameterCount) + ", got " + std::to_string(argumentCount);
        throw std::runtime_error(message);
    }

    std::vector<Value *> args;
    // swap arguments to visit and codegen
    std::swap(args, m_values);
    for (const IExpressionASTUniquePtr & expr : expr.GetArguments())
    {
        expr->Accept(*this);
    }
    std::swap(args, m_values);

    Value *pValue = m_builder.CreateCall(pFunction, args, "calltmp");
    m_values.push_back(pValue);
}

void CExpressionCodeGenerator::Visit(CVariableRefAST &expr)
{
    Value *pValue = m_context.GetCurrentScope().TryGetVariableValue(expr.GetNameId());
    m_values.push_back(pValue);
}

CBlockCodeGenerator::CBlockCodeGenerator(CInterpreterContext &context)
    : m_context(context)
    , m_builder(m_context.GetLLVMContext())
    , m_exprGen(m_builder, context)
{
}

void CBlockCodeGenerator::Codegen(BasicBlock &bb, const StatementsList &block)
{
    m_builder.SetInsertPoint(&bb);
    for (const IStatementASTUniquePtr & pAst : block)
    {
        pAst->Accept(*this);
    }
}

void CBlockCodeGenerator::Visit(CPrintAST &ast)
{
    auto & context = m_context.GetLLVMContext();
    auto & module = m_context.GetModule();

    GlobalVariable * message = AddStringConstant(context, module, "%lf");
    Constant* pFormatAddress = GetStringConstantPtr(context, message);

    Function *pFunction = m_context.GetPrintF();
    llvm::Value *pValue = m_exprGen.Codegen(ast.GetValue());
    std::vector<llvm::Value *> args = {pFormatAddress, pValue};
    m_builder.CreateCall(pFunction, args);
}

void CBlockCodeGenerator::Visit(CAssignAST &expr)
{
    (void)expr;
    throw std::runtime_error("assignment not implemented");
}

void CBlockCodeGenerator::Visit(CReturnAST &expr)
{
    if (auto *pValue = m_exprGen.Codegen(expr.GetValue()))
    {
        m_builder.CreateRet(pValue);
    }
}

void CBlockCodeGenerator::Visit(CWhileAst &expr)
{
    (void)expr;
    throw std::runtime_error("`while` not implemented");
}

void CBlockCodeGenerator::Visit(CRepeatAst &expr)
{
    (void)expr;
    throw std::runtime_error("`do..while` not implemented");
}

void CBlockCodeGenerator::Visit(CIfAst &expr)
{
    (void)expr;
    throw std::runtime_error("`if..else` not implemented");
}

CCodeGenerator::CCodeGenerator(CInterpreterContext &context)
    : m_context(context)
{
}

Function *CCodeGenerator::AcceptFunction(IFunctionAST &ast)
{
    Function *fn = GenerateDeclaration(ast);
    m_context.AddFunction(ast.GetNameId(), fn);
    GenerateDefinition(*fn, ast);
    fn->dump();

    return fn;
}

Function *CCodeGenerator::GenerateDeclaration(IFunctionAST &ast)
{
    auto & context = m_context.GetLLVMContext();
    auto & module = m_context.GetModule();

    std::vector<Type *> doubles(ast.GetArgumentNames().size(), Type::getDoubleTy(context));
    FunctionType *fnType = FunctionType::get(Type::getDoubleTy(context), doubles, false);
    std::string fnName = m_context.GetString(ast.GetNameId());
    Function *fn = Function::Create(fnType, Function::ExternalLinkage, fnName, &module);

    size_t idx = 0;
    const std::vector<unsigned> &names = ast.GetArgumentNames();
    for (auto &arg : fn->args())
    {
        arg.setName(m_context.GetString(names[idx]));
        ++idx;
    }
    return fn;
}

bool CCodeGenerator::GenerateDefinition(Function &fn, IFunctionAST &ast)
{
    auto & context = m_context.GetLLVMContext();

    std::unique_ptr<CVariablesScope> pScope = m_context.MakeScope();

    const auto &argNames = ast.GetArgumentNames();
    size_t idx = 0;
    for (auto &arg : fn.args())
    {
        pScope->AssignVariable(argNames[idx], &arg);
        ++idx;
    }

    CBlockCodeGenerator generator(m_context);

    // Создаём базовый блок CFG для вставки инструкций в этот блок.
    BasicBlock *bb = BasicBlock::Create(context, "entry", &fn);
    generator.Codegen(*bb, ast.GetBody());

    // Валидация и проверка целостности созданного кода вызовом `llvm::verifyFunction`.
    std::string outputStr;
    raw_string_ostream output(outputStr);
    if (verifyFunction(fn, &output))
    {
        m_context.PrintError("Function verification failed for " + m_context.GetString(ast.GetNameId())
                             + ", '" + output.str() + "'");
        fn.dump();
        fn.eraseFromParent();
        return false;
    }
    return true;
}

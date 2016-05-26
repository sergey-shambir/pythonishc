#include "CodegenVisitor.h"
#include "AST.h"
#include "Utility.h"
#include "FrontendContext.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#include <llvm/IR/Constants.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/Support/raw_ostream.h>
#include <boost/variant.hpp>
#pragma clang diagnostic pop

using namespace llvm;

namespace
{

Constant *AddStringLiteral(LLVMContext & context, Module & module, std::string const& value)
{
    // TODO: use strings pool.
    Constant *pConstant = ConstantDataArray::getString(context, value, true);

    // Выделение глобальной переменной на куче безопасно
    // после создания объект передаст указатель на себя родителю
    // удалять указатель вручную нельзя.
    GlobalVariable *global = new GlobalVariable(module, pConstant->getType(), true,
                                                GlobalValue::InternalLinkage, pConstant, "str");

    Constant *index = Constant::getNullValue(IntegerType::getInt32Ty(context));
    std::vector<Constant*> indices = {index, index};

    return ConstantExpr::getInBoundsGetElementPtr(pConstant->getType(), global, indices);
}

// Генерирует код константы LLVM.
struct LiteralCodeGenerator : boost::static_visitor<Constant *>
{
    explicit LiteralCodeGenerator(CCodegenContext &context)
        : m_context(context)
    {
    }

    Constant *operator ()(double const& value) const
    {
        return ConstantFP::get(m_context.GetLLVMContext(), APFloat(value));
    }

    Constant *operator ()(bool const& value) const
    {
        return ConstantInt::get(m_context.GetLLVMContext(), APInt(8, uint64_t(value), true));
    }

    Constant *operator ()(std::string const& value)
    {
        return m_context.AddStringLiteral(value);
    }

private:
    CCodegenContext &m_context;
};

Value *GenerateBinaryExpr(IRBuilder<> & builder, LLVMContext &context, Value *a, BinaryOperation op, Value *b)
{
    // FIXME: implemented only for double
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
    // FIXME: implemented only for double
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

AllocaInst *MakeLocalVariable(Function &function, Type & type, const std::string &name)
{
    BasicBlock &block = function.getEntryBlock();
    IRBuilder<> temp(&block, block.begin());

    return temp.CreateAlloca(&type, nullptr, name);
}

Type *ConvertType(LLVMContext &context, ExpressionType type)
{
    switch (type)
    {
    case ExpressionType::Boolean:
        return Type::getInt8Ty(context);
    case ExpressionType::Number:
        return Type::getDoubleTy(context);
    case ExpressionType::String:
        return Type::getInt8PtrTy(context);
    }
    throw std::logic_error("ConvertType: unkown expression type");
}

class CVariableScopeLock
{
public:
    CVariableScopeLock(CCodegenContext &context)
        : m_context(context)
    {
        m_context.GetVariables().PushScope();
    }

    ~CVariableScopeLock()
    {
        m_context.GetVariables().PopScope();
    }

private:
    CCodegenContext &m_context;
};
} // anonymous namespace


CCodegenContext::CCodegenContext(CFrontendContext &context)
    : m_context(context)
    , m_pLLVMContext(std::make_unique<llvm::LLVMContext>())
    , m_pModule(std::make_unique<llvm::Module>("main module", *m_pLLVMContext))
{
    m_functions.PushScope();
    InitLibCBuiltins();
}

CCodegenContext::~CCodegenContext()
{
}

std::string CCodegenContext::GetString(unsigned stringId) const
{
    return m_context.GetString(stringId);
}

void CCodegenContext::PrintError(const std::string &message) const
{
    m_context.PrintError(message);
}

LLVMContext &CCodegenContext::GetLLVMContext()
{
    return *m_pLLVMContext;
}

Module &CCodegenContext::GetModule()
{
    return *m_pModule;
}

CScopeChain<AllocaInst *> &CCodegenContext::GetVariables()
{
    return m_variables;
}

CScopeChain<Function *> &CCodegenContext::GetFunctions()
{
    return m_functions;
}

Constant *CCodegenContext::AddStringLiteral(const std::string &value)
{
    auto &elem = m_strings[value];
    if (Constant *pConst = elem)
    {
        return pConst;
    }
    elem = ::AddStringLiteral(*m_pLLVMContext, *m_pModule, value);
    return elem;
}

Function *CCodegenContext::GetBuiltinFunction(BuiltinFunction id) const
{
    return m_builtinFunctions.at(id);
}

void CCodegenContext::InitLibCBuiltins()
{
    auto & context = *m_pLLVMContext;
    auto * pModule = m_pModule.get();
    auto declareFn = [&](llvm::FunctionType *type, const char *name) {
        return llvm::Function::Create(type, llvm::Function::ExternalLinkage, name, pModule);
    };

    llvm::Type *cStringType = llvm::Type::getInt8PtrTy(context);
    llvm::Type *intType = llvm::Type::getInt32Ty(context);
    llvm::Type *voidType = llvm::Type::getVoidTy(context);
    // i32 printf(i8* format, ...)
    {
        auto *fnType = llvm::FunctionType::get(intType, {cStringType}, true);
        m_builtinFunctions[BuiltinFunction::PRINTF] = declareFn(fnType, "printf");
    }
    // i8 *strcat(i8* dest, i8* src)
    {
        auto *fnType = llvm::FunctionType::get(cStringType, {cStringType, cStringType}, false);
        m_builtinFunctions[BuiltinFunction::STRCAT] = declareFn(fnType, "strcat");
    }
    // i8 *strdup(i8 *str)
    {
        auto *fnType = llvm::FunctionType::get(cStringType, {cStringType}, false);
        m_builtinFunctions[BuiltinFunction::STRDUP] = declareFn(fnType, "strdup");
    }
    // void *free(i8 *ptr)
    {
        auto *fnType = llvm::FunctionType::get(voidType, {cStringType}, false);
        m_builtinFunctions[BuiltinFunction::FREE] = declareFn(fnType, "free");
    }
}

CExpressionCodeGenerator::CExpressionCodeGenerator(llvm::IRBuilder<> &builder, CCodegenContext &context)
    : m_context(context)
    , m_builder(builder)
{
}

Value *CExpressionCodeGenerator::Codegen(IExpressionAST &ast)
{
    llvm::Value *pValue = nullptr;
    try
    {
        m_values.clear();
        ast.Accept(*this);
        pValue = m_values.at(0);
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
    LiteralCodeGenerator generator(m_context);
    Value *pValue = expr.GetValue().apply_visitor(generator);
    m_values.push_back(pValue);
}

void CExpressionCodeGenerator::Visit(CCallAST &expr)
{
    Function *pFunction = *m_context.GetFunctions().GetSymbol(expr.GetFunctionNameId());

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
    AllocaInst *pVar = m_context.GetVariables().GetSymbol(expr.GetNameId()).get_value_or(nullptr);
    std::string varName = m_context.GetString(expr.GetNameId());
    if (!pVar)
    {
        throw std::runtime_error("unknown variable " + varName);
    }
    Value *pValue = m_builder.CreateLoad(pVar, varName);
    m_values.push_back(pValue);
}

void CExpressionCodeGenerator::Visit(CParameterDeclAST &expr)
{
    LLVMContext &context = m_context.GetLLVMContext();
    Type *type = ConvertType(context, expr.GetType());
    std::string varName = m_context.GetString(expr.GetName());

    AllocaInst *pVar = m_builder.CreateAlloca(type, nullptr, varName);
    m_values.push_back(pVar);
    m_context.GetVariables().DefineSymbol(expr.GetName(), pVar);
}

CFunctionCodeGenerator::CFunctionCodeGenerator(CCodegenContext &context)
    : m_context(context)
    , m_builder(m_context.GetLLVMContext())
    , m_exprGen(m_builder, context)
{
}

void CFunctionCodeGenerator::Codegen(const ParameterDeclList &parameters, const StatementsList &block, Function &fn)
{
    // Создаём базовый блок CFG для вставки инструкций в этот блок.
    BasicBlock *bb = BasicBlock::Create(m_context.GetLLVMContext(), "entry", &fn);
    m_builder.SetInsertPoint(bb);
    LoadParameters(fn, parameters);
    CodegenForAstList(block);
}

void CFunctionCodeGenerator::AddExitMain()
{
    Constant *exitCode = ConstantInt::get(m_context.GetLLVMContext(), APInt(32, uint64_t(0), true));
    m_builder.CreateRet(exitCode);
}

void CFunctionCodeGenerator::Visit(CPrintAST &ast)
{
    auto & context = m_context.GetLLVMContext();
    auto & module = m_context.GetModule();
    ExpressionType type = ast.GetValue().GetType();
    Value *pValue = m_exprGen.Codegen(ast.GetValue());
    std::string format;

    switch (type)
    {
    case ExpressionType::Boolean:
    {
        // Same as `printf("%s\n", x ? "true" : "false");`
        Value *trueStr = m_context.AddStringLiteral("true");
        Value *falseStr = m_context.AddStringLiteral("false");
        pValue = m_builder.CreateSelect(pValue, trueStr, falseStr, "bool2string");
        format = "%s\n";
        break;
    }
    case ExpressionType::Number:
        format = "%lf\n";
        break;
    case ExpressionType::String:
        format = "%s\n";
        break;
    }

    Constant* pFormatAddress = AddStringLiteral(context, module, format);
    Function *pFunction = m_context.GetBuiltinFunction(BuiltinFunction::PRINTF);
    std::vector<llvm::Value *> args = {pFormatAddress, pValue};
    m_builder.CreateCall(pFunction, args);
}

void CFunctionCodeGenerator::Visit(CAssignAST &ast)
{
    llvm::Value *pValue = m_exprGen.Codegen(ast.GetValue());

    unsigned nameId = ast.GetNameId();
    AllocaInst *pVar = m_context.GetVariables().GetSymbol(nameId).get_value_or(nullptr);
    if (pVar)
    {
        llvm::Type *pVarType = pVar->getType()->getPointerElementType();
        if (pVarType->getTypeID() != pValue->getType()->getTypeID())
        {
            std::string name = m_context.GetString(nameId);
            throw std::runtime_error("Cannot change '" + name + "' variable type in assignment");
        }
    }
    else
    {
        Function *pFunction = m_builder.GetInsertBlock()->getParent();
        pVar = MakeLocalVariable(*pFunction, *pValue->getType(), m_context.GetString(nameId));
        m_context.GetVariables().DefineSymbol(nameId, pVar);
    }
    m_builder.CreateStore(pValue, pVar);
}

void CFunctionCodeGenerator::Visit(CReturnAST &ast)
{
    if (auto *pValue = m_exprGen.Codegen(ast.GetValue()))
    {
        m_builder.CreateRet(pValue);
    }
}

void CFunctionCodeGenerator::Visit(CWhileAst &ast)
{
    CodegenLoop(ast, false);
}

void CFunctionCodeGenerator::Visit(CRepeatAst &ast)
{
    CodegenLoop(ast, true);
}

void CFunctionCodeGenerator::Visit(CIfAst &ast)
{
    auto & context = m_context.GetLLVMContext();
    Function *function = m_builder.GetInsertBlock()->getParent();
    BasicBlock *thenBB = BasicBlock::Create(context, "then", function);
    BasicBlock *elseBB = BasicBlock::Create(context, "else", function);
    BasicBlock *mergeBB = BasicBlock::Create(context, "continue", function);

    Value *condition = m_exprGen.Codegen(ast.GetCondition());
    condition = m_builder.CreateFCmpONE(condition, ConstantFP::get(context, APFloat(0.0)), "if_condition");
    m_builder.CreateCondBr(condition, thenBB, elseBB);
    m_builder.SetInsertPoint(thenBB);
    CodegenForAstList(ast.GetThenBody());
    m_builder.CreateBr(mergeBB);
    m_builder.SetInsertPoint(elseBB);
    CodegenForAstList(ast.GetElseBody());
    m_builder.CreateBr(mergeBB);
    m_builder.SetInsertPoint(mergeBB);
}

void CFunctionCodeGenerator::LoadParameters(Function &fn, const ParameterDeclList &parameters)
{
    std::vector<llvm::Value*> allocs;
    allocs.reserve(fn.arg_size());
    for (const auto &pParam : parameters)
    {
        allocs.push_back(m_exprGen.Codegen(*pParam));
    }
    size_t idx = 0;
    for (auto &arg : fn.args())
    {
        m_builder.CreateStore(&arg, allocs[idx]);
        ++idx;
    }
}

void CFunctionCodeGenerator::CodegenLoop(CAbstractLoopAst &ast, bool skipFirstCheck)
{
    auto & context = m_context.GetLLVMContext();
    Function *function = m_builder.GetInsertBlock()->getParent();
    BasicBlock *conditionBB = BasicBlock::Create(context, "cond", function);
    BasicBlock *loopBB = BasicBlock::Create(context, "loop", function);
    BasicBlock *nextBB = BasicBlock::Create(context, "continue", function);

    m_builder.CreateBr(skipFirstCheck ? loopBB : conditionBB);
    m_builder.SetInsertPoint(conditionBB);
    Value *condition = m_exprGen.Codegen(ast.GetCondition());
    condition = m_builder.CreateFCmpONE(condition, ConstantFP::get(context, APFloat(0.0)), "do_while_condition");
    m_builder.CreateCondBr(condition, loopBB, nextBB);
    m_builder.SetInsertPoint(loopBB);
    CodegenForAstList(ast.GetBody());
    m_builder.CreateBr(conditionBB);
    m_builder.SetInsertPoint(nextBB);
}

void CFunctionCodeGenerator::CodegenForAstList(const StatementsList &block)
{
    for (const IStatementASTUniquePtr & pAst : block)
    {
        pAst->Accept(*this);
    }
}

CCodeGenerator::CCodeGenerator(CCodegenContext &context)
    : m_context(context)
{
}

Function *CCodeGenerator::AcceptFunction(IFunctionAST &ast)
{
    Function *fn = GenerateDeclaration(ast, false);
    m_context.GetFunctions().DefineSymbol(ast.GetNameId(), fn);
    GenerateDefinition(*fn, ast, false);

    return fn;
}

Function *CCodeGenerator::AcceptMainFunction(IFunctionAST &ast)
{
    Function *fn = GenerateDeclaration(ast, true);
    GenerateDefinition(*fn, ast, true);

    return fn;
}

Function *CCodeGenerator::GenerateDeclaration(IFunctionAST &ast, bool isMain)
{
    auto & context = m_context.GetLLVMContext();
    auto & module = m_context.GetModule();

    std::vector<Type *> args;
    Type *returnType = nullptr;
    if (isMain)
    {
        returnType = Type::getInt32Ty(context);
    }
    else
    {
        returnType = ConvertType(context, ast.GetReturnType());
        args.reserve(ast.GetParameters().size());
        for (const auto &pParam : ast.GetParameters())
        {
            args.push_back(ConvertType(context, pParam->GetType()));
        }
    }

    FunctionType *fnType = FunctionType::get(returnType, args, false);
    std::string fnName = m_context.GetString(ast.GetNameId());
    Function *fn = Function::Create(fnType, Function::ExternalLinkage, fnName, &module);

    auto paramIt = ast.GetParameters().begin();
    for (auto &arg : fn->args())
    {
        arg.setName(m_context.GetString((*paramIt)->GetName()));
        ++paramIt;
    }
    return fn;
}

bool CCodeGenerator::GenerateDefinition(Function &fn, IFunctionAST &ast, bool isMain)
{
    CVariableScopeLock scopedScope(m_context);
    CFunctionCodeGenerator generator(m_context);

    generator.Codegen(ast.GetParameters(), ast.GetBody(), fn);
    if (isMain)
    {
        generator.AddExitMain();
    }

    // Валидация и проверка целостности созданного кода вызовом `llvm::verifyFunction`.
    std::string outputStr;
    raw_string_ostream output(outputStr);
    if (verifyFunction(fn, &output))
    {
        m_context.PrintError("Function verification failed for " + m_context.GetString(ast.GetNameId())
                             + ", '" + output.str() + "'");
        fn.eraseFromParent();
        return false;
    }
    return true;
}

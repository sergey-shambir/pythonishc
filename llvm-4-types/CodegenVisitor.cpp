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
#include <llvm/ADT/STLExtras.h>
#define BOOST_RESULT_OF_USE_DECLTYPE
#include <boost/variant.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext/for_each.hpp>
#pragma clang diagnostic pop

using namespace llvm;

namespace
{

// Поскольку сейчас компилятор не совершает кросскомпиляции, то мы просто определяем
// размер size_t и такой же размер используем в сгенерированном коде.
llvm::Type *GetPointerSizeType(LLVMContext &context)
{
    return (sizeof(size_t) == sizeof(uint32_t))
            ? llvm::Type::getInt32Ty(context)
            : llvm::Type::getInt64Ty(context);
}

ConstantInt *AddSizeLiteral(CCodegenContext &context, uint64_t value)
{
    unsigned bitsCount = (sizeof(size_t) == sizeof(uint32_t)) ? 32 : 64;
    return ConstantInt::get(context.GetLLVMContext(), APInt(bitsCount, value, true));
}

ConstantInt *AddInt32Literal(CCodegenContext &context, uint64_t value)
{
    return ConstantInt::get(context.GetLLVMContext(), APInt(32, value, true));
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

AllocaInst *MakeLocalVariable(Function &function, Type & type, const std::string &name)
{
    BasicBlock &block = function.getEntryBlock();
    IRBuilder<> temp(&block, block.begin());

    return temp.CreateAlloca(&type, nullptr, name);
}

// Отображение типов на LLVM-IR:
// Boolean -> i8
// Number -> double
// String -> i8*
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

class CFunctionScopeLock
{
public:
    CFunctionScopeLock(CCodegenContext &context)
        : m_context(context)
    {
        m_context.GetVariables().PushScope();
    }

    ~CFunctionScopeLock()
    {
        m_context.GetVariables().PopScope();
    }

private:
    CCodegenContext &m_context;
};
} // anonymous namespace


CManagedStrings::CManagedStrings(CCodegenContext &context)
    : m_context(context)
{
}

CManagedStrings::~CManagedStrings()
{
}

void CManagedStrings::FreeAll(llvm::IRBuilder<> & builder)
{
    auto *pFree = m_context.GetBuiltinFunction(BuiltinFunction::FREE);
    for (llvm::Value *value : m_pointers)
    {
        builder.CreateCall(pFree, {value});
    }
}

void CManagedStrings::Clear()
{
    m_pointers.clear();
}

Value *CManagedStrings::TakeStringOrCopy(llvm::IRBuilder<> & builder, Value *pString)
{
    // Если значение не является указателем, возвращаем это же значение.
    if (!pString->getType()->isPointerTy())
    {
        return pString;
    }
    auto it = m_pointers.find(pString);
    // Если кто-то уже владеет строкой, вызываем strdup, иначе снимаем со своего контроля.
    if (it == m_pointers.end())
    {
        auto pStrdup = m_context.GetBuiltinFunction(BuiltinFunction::STRDUP);
        return builder.CreateCall(pStrdup, {pString}, "take_str");
    }
    else
    {
        m_pointers.erase(it);
        return pString;
    }
}

void CManagedStrings::Manage(Value *pString)
{
    if (m_pointers.count(pString))
    {
        throw std::logic_error("Attempt to manage string twice");
    }
    m_pointers.insert(pString);
}

CCodegenContext::CCodegenContext(CFrontendContext &context)
    : m_context(context)
    , m_pLLVMContext(std::make_unique<llvm::LLVMContext>())
    , m_pModule(std::make_unique<llvm::Module>("main module", *m_pLLVMContext))
    , m_expressionStrings(*this)
    , m_functionStrings(*this)
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
    if (!elem)
    {
        // Создаём константное значение, которым будет инициализирована глобальная константа.
        Constant *pConstant = ConstantDataArray::getString(*m_pLLVMContext, value, true);
        // Выделение глобальной переменной на куче безопасно
        // после создания объект передаст указатель на себя родителю
        // удалять указатель вручную нельзя.
        GlobalVariable *global = new GlobalVariable(*m_pModule, pConstant->getType(), true,
                                                    GlobalValue::InternalLinkage, pConstant, "str");
        Constant *index = Constant::getNullValue(IntegerType::getInt32Ty(*m_pLLVMContext));
        std::vector<Constant*> indices = {index, index};
        elem = ConstantExpr::getInBoundsGetElementPtr(pConstant->getType(), global, indices);
    }

    return elem;
}

Function *CCodegenContext::GetBuiltinFunction(BuiltinFunction id) const
{
    return m_builtinFunctions.at(id);
}

CManagedStrings &CCodegenContext::GetExpressionStrings()
{
    return m_expressionStrings;
}

CManagedStrings &CCodegenContext::GetFunctionStrings()
{
    return m_functionStrings;
}

void CCodegenContext::InitLibCBuiltins()
{
    auto & context = *m_pLLVMContext;
    auto * pModule = m_pModule.get();
    auto declareFn = [&](llvm::FunctionType *type, const char *name) {
        return llvm::Function::Create(type, llvm::Function::ExternalLinkage, name, pModule);
    };

    llvm::Type *cStringType = llvm::Type::getInt8PtrTy(context);
    llvm::Type *int32Type = llvm::Type::getInt32Ty(context);
    llvm::Type *voidType = llvm::Type::getVoidTy(context);
    llvm::Type *sizeType = GetPointerSizeType(context);
    // i32 printf(i8* format, ...)
    {
        auto *fnType = llvm::FunctionType::get(int32Type, {cStringType}, true);
        m_builtinFunctions[BuiltinFunction::PRINTF] = declareFn(fnType, "printf");
    }
    // i8 *strcat(i8* dest, i8* src)
    {
        auto *fnType = llvm::FunctionType::get(cStringType, {cStringType, cStringType}, false);
        m_builtinFunctions[BuiltinFunction::STRCPY] = declareFn(fnType, "strcpy");
    }
    // i8 *strdup(i8 *str)
    {
        auto *fnType = llvm::FunctionType::get(cStringType, {cStringType}, false);
        m_builtinFunctions[BuiltinFunction::STRDUP] = declareFn(fnType, "strdup");
    }
    // size_t strlen(i8 *str)
    {
        auto *fnType = llvm::FunctionType::get(sizeType, {cStringType}, false);
        m_builtinFunctions[BuiltinFunction::STRLEN] = declareFn(fnType, "strlen");
    }
    // i32 strcmp(i8* str, i8* str)
    {
        auto *fnType = llvm::FunctionType::get(int32Type, {cStringType, cStringType}, false);
        m_builtinFunctions[BuiltinFunction::STRCMP] = declareFn(fnType, "strcmp");
    }
    // i8 *malloc(size_t size)
    {
        auto *fnType = llvm::FunctionType::get(cStringType, {sizeType}, false);
        m_builtinFunctions[BuiltinFunction::MALLOC] = declareFn(fnType, "malloc");
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
    try
    {
        m_values.clear();
        ast.Accept(*this);
        return m_values.at(0);
    }
    catch (std::exception const& ex)
    {
        m_context.PrintError(ex.what());
        return nullptr;
    }
}

void CExpressionCodeGenerator::Visit(CBinaryExpressionAST &expr)
{
    expr.GetLeft().Accept(*this);
    expr.GetRight().Accept(*this);
    Value *a = m_values.at(m_values.size() - 2);
    Value *b = m_values.at(m_values.size() - 1);
    m_values.erase(m_values.end() - 2, m_values.end());

    llvm::Value *pValue = nullptr;
    switch (expr.GetLeft().GetType())
    {
    case ExpressionType::Boolean:
        pValue = GenerateBooleanExpr(a, expr.GetOperation(), b);
        break;
    case ExpressionType::Number:
        pValue = GenerateNumericExpr(a, expr.GetOperation(), b);
        break;
    case ExpressionType::String:
        pValue = GenerateStringExpr(a, expr.GetOperation(), b);
        break;
    }
    m_values.push_back(pValue);
}

void CExpressionCodeGenerator::Visit(CUnaryExpressionAST &expr)
{
    expr.GetOperand().Accept(*this);
    Value *x = m_values.back();
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
    if (pValue->getType()->isPointerTy())
    {
        m_context.GetExpressionStrings().Manage(pValue);
    }
    m_values.push_back(pValue);
}

void CExpressionCodeGenerator::Visit(CVariableRefAST &expr)
{
    AllocaInst *pVar = *m_context.GetVariables().GetSymbol(expr.GetNameId());
    std::string varName = m_context.GetString(expr.GetNameId());
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

Value *CExpressionCodeGenerator::GenerateNumericExpr(Value *a, BinaryOperation op, Value *b)
{
    switch (op)
    {
    case BinaryOperation::Add:
        return m_builder.CreateFAdd(a, b, "addtmp");
    case BinaryOperation::Substract:
        return m_builder.CreateFSub(a, b, "subtmp");
    case BinaryOperation::Multiply:
        return m_builder.CreateFMul(a, b, "multmp");
    case BinaryOperation::Divide:
        return m_builder.CreateFDiv(a, b, "divtmp");
    case BinaryOperation::Modulo:
        return m_builder.CreateFRem(a, b, "modtmp");
    case BinaryOperation::Less:
        return m_builder.CreateFCmpULT(a, b, "cmptmp");
    case BinaryOperation::Equals:
        return m_builder.CreateFCmpUEQ(a, b, "cmptmp");
    }
    throw std::runtime_error("CExpressionCodeGenerator: unknown numeric binary operation");
}

Value *CExpressionCodeGenerator::GenerateStringExpr(Value *a, BinaryOperation op, Value *b)
{
    switch (op)
    {
    case BinaryOperation::Add:
    {
        /*** the same code in C ***
         * size_t lenA = strlen(a);
         * size_t sum = lenA + strlen(b);
         * char *newStr = malloc(sum);
         * strcat(newStr, a);
         * strcat(newStr + lenA, b);
         * return newStr;
         */
        auto *pStrlen = m_context.GetBuiltinFunction(BuiltinFunction::STRLEN);
        auto *pStrcpy = m_context.GetBuiltinFunction(BuiltinFunction::STRCPY);
        auto *pMalloc = m_context.GetBuiltinFunction(BuiltinFunction::MALLOC);
        Value *lenA = m_builder.CreateCall(pStrlen, {a}, "left_length");
        Value *lenB = m_builder.CreateCall(pStrlen, {b}, "right_length");
        Value *lenSum = m_builder.CreateAdd(lenA, lenB, "sum_length");
        lenSum = m_builder.CreateAdd(lenSum, AddSizeLiteral(m_context, 1), "malloc_size");
        Value *newStr = m_builder.CreateCall(pMalloc, {lenSum}, "new_str");
        m_context.GetExpressionStrings().Manage(newStr);
        m_builder.CreateCall(pStrcpy, {newStr, a}, "copy_left");
        Value *nextDest = m_builder.CreateGEP(newStr, lenA, "dest_str");
        m_builder.CreateCall(pStrcpy, {nextDest, b}, "copy_right");
        return newStr;
    }
    case BinaryOperation::Substract:
    case BinaryOperation::Multiply:
    case BinaryOperation::Divide:
    case BinaryOperation::Modulo:
        // disallowed for String.
        break;
    case BinaryOperation::Less:
        return m_builder.CreateICmpSLT(GenerateStrcmp(a, b), AddInt32Literal(m_context, 0), "less_than_0");
    case BinaryOperation::Equals:
        return m_builder.CreateICmpEQ(GenerateStrcmp(a, b), AddInt32Literal(m_context, 0), "is_0");
    }
    throw std::runtime_error("CExpressionCodeGenerator: unknown strings binary operation");
}

Value *CExpressionCodeGenerator::GenerateBooleanExpr(Value *a, BinaryOperation op, Value *b)
{
    switch (op)
    {
    case BinaryOperation::Add:
    case BinaryOperation::Substract:
    case BinaryOperation::Multiply:
    case BinaryOperation::Divide:
    case BinaryOperation::Modulo:
        // disallowed for Boolean.
        break;
    case BinaryOperation::Less:
        return m_builder.CreateICmpSLT(a, b);
    case BinaryOperation::Equals:
        return m_builder.CreateICmpEQ(a, b);
    }
    throw std::runtime_error("CExpressionCodeGenerator: unknown boolean binary operation");
}

Value *CExpressionCodeGenerator::GenerateStrcmp(Value *a, Value *b)
{
    auto *pStrcmp = m_context.GetBuiltinFunction(BuiltinFunction::STRCMP);
    return m_builder.CreateCall(pStrcmp, {a, b}, "strings_cmp");
}

CFunctionCodeGenerator::CFunctionCodeGenerator(CCodegenContext &context)
    : m_context(context)
    , m_builder(m_context.GetLLVMContext())
    , m_exprGen(m_builder, context)
{
}

// Создаёт базовый блок CFG для вставки инструкций в этот блок.
// Затем выгружает параметры и генерирует код тела функции.
void CFunctionCodeGenerator::Codegen(const ParameterDeclList &parameters, const StatementsList &block, Function &fn)
{
    BasicBlock *bb = BasicBlock::Create(m_context.GetLLVMContext(), "entry", &fn);
    m_builder.SetInsertPoint(bb);
    LoadParameters(fn, parameters);
    for (const IStatementASTUniquePtr & pAst : block)
    {
        pAst->Accept(*this);
    }
    RemoveUnusedBlocks(fn);
}

void CFunctionCodeGenerator::AddExitMain()
{
    FreeFunctionAllocs();
    Constant *exitCode = ConstantInt::get(m_context.GetLLVMContext(), APInt(32, uint64_t(0), true));
    m_builder.CreateRet(exitCode);
}

void CFunctionCodeGenerator::Visit(CPrintAST &ast)
{
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

    Constant* pFormatAddress = m_context.AddStringLiteral(format);
    Function *pFunction = m_context.GetBuiltinFunction(BuiltinFunction::PRINTF);
    std::vector<llvm::Value *> args = {pFormatAddress, pValue};
    m_builder.CreateCall(pFunction, args);
    FreeExpressionAllocs();
}

void CFunctionCodeGenerator::Visit(CAssignAST &ast)
{
    llvm::Value *pValue = m_exprGen.Codegen(ast.GetValue());
    unsigned nameId = ast.GetNameId();
    AllocaInst *pVar = m_context.GetVariables().GetSymbol(nameId).get_value_or(nullptr);
    if (!pVar)
    {
        Function *pFunction = m_builder.GetInsertBlock()->getParent();
        pVar = MakeLocalVariable(*pFunction, *pValue->getType(), m_context.GetString(nameId));
        m_context.GetVariables().DefineSymbol(nameId, pVar);
    }
    m_builder.CreateStore(MakeValueCopy(pValue), pVar);
    if (pValue->getType()->isPointerTy())
    {
        m_context.GetFunctionStrings().Manage(pValue);
    }
    FreeExpressionAllocs();
}

void CFunctionCodeGenerator::Visit(CReturnAST &ast)
{
    if (auto *pValue = m_exprGen.Codegen(ast.GetValue()))
    {
        pValue = MakeValueCopy(pValue);
        FreeExpressionAllocs();
        FreeFunctionAllocs();
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
    BasicBlock *mergeBB = BasicBlock::Create(context, "merge_if", function);

    Value *condition = m_exprGen.Codegen(ast.GetCondition());
    m_builder.CreateCondBr(condition, thenBB, elseBB);
    FillBlockAndJump(ast.GetThenBody(), thenBB, mergeBB);
    FillBlockAndJump(ast.GetElseBody(), elseBB, mergeBB);
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
    BasicBlock *nextBB = BasicBlock::Create(context, "after_loop", function);

    m_builder.CreateBr(skipFirstCheck ? loopBB : conditionBB);
    m_builder.SetInsertPoint(conditionBB);
    Value *condition = m_exprGen.Codegen(ast.GetCondition());
    m_builder.CreateCondBr(condition, loopBB, nextBB);
    FillBlockAndJump(ast.GetBody(), loopBB, conditionBB);
    m_builder.SetInsertPoint(nextBB);
}

void CFunctionCodeGenerator::FillBlockAndJump(const StatementsList &statements, BasicBlock *block, BasicBlock *nextBlock)
{
    m_builder.SetInsertPoint(block);
    for (const IStatementASTUniquePtr & pAst : statements)
    {
        pAst->Accept(*this);
    }
    if (nextBlock && (nullptr == block->getTerminator()))
    {
        m_builder.CreateBr(nextBlock);
    }
}

Value *CFunctionCodeGenerator::MakeValueCopy(Value *pValue)
{
    // Если значение строковое, забираем владение оригиналом или дублем строки.
    if (pValue->getType()->isPointerTy())
    {
        return m_context.GetExpressionStrings().TakeStringOrCopy(m_builder, pValue);
    }
    return pValue;
}

void CFunctionCodeGenerator::FreeExpressionAllocs()
{
    m_context.GetExpressionStrings().FreeAll(m_builder);
    m_context.GetExpressionStrings().Clear();
}

void CFunctionCodeGenerator::FreeFunctionAllocs()
{
    // Грубая очистка памяти строк, сохранённых в переменных функции.
    // Не будет корректно работать при наличии более одной области видимости в функции.
    m_context.GetFunctionStrings().FreeAll(m_builder);
}

// Убирает неиспользуемые блоки.
// Они могут возникнуть из-за ранее созданных return, например, в таком коде:
//  function sign(x Number) Number
//      if (x < 0)
//           return -1
//      else
//          return 1
//      end
//  ends
void CFunctionCodeGenerator::RemoveUnusedBlocks(Function &fn)
{
    std::vector<BasicBlock*> unusedBlocks;
    auto &blocks = fn.getBasicBlockList();

    for (auto &bb : boost::make_iterator_range(++blocks.begin(), blocks.end()))
    {
        if (bb.hasNUses(0))
        {
            unusedBlocks.push_back(llvm::dyn_cast<BasicBlock>(&bb));
        }
    }
    for (BasicBlock *bb : unusedBlocks)
    {
        bb->eraseFromParent();
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

    Type *returnType = isMain ? Type::getInt32Ty(context) : ConvertType(context, ast.GetReturnType());
    std::vector<Type *> args(ast.GetParameters().size(), nullptr);
    boost::transform(ast.GetParameters(), args.begin(), [&](const CParameterDeclASTUniquePtr &pAst) {
        return ConvertType(context, pAst->GetType());
    });

    FunctionType *fnType = FunctionType::get(returnType, args, false);
    std::string fnName = m_context.GetString(ast.GetNameId());
    Function *fn = Function::Create(fnType, Function::ExternalLinkage, fnName, &module);

    auto argIt = fn->args().begin();
    for (const auto &pAst : ast.GetParameters())
    {
        argIt->setName(m_context.GetString(pAst->GetName()));
        ++argIt;
    }

    return fn;
}

bool CCodeGenerator::GenerateDefinition(Function &fn, IFunctionAST &ast, bool isMain)
{
    CFunctionScopeLock scopedScope(m_context);
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

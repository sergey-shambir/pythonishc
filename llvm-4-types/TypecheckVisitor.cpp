#include "TypecheckVisitor.h"
#include "FrontendContext.h"
#include <boost/range/algorithm.hpp>

namespace
{
std::string PrettyPrint(ExpressionType type)
{
    switch (type)
    {
    case ExpressionType::Boolean:
        return "Boolean";
    case ExpressionType::Number:
        return "Number";
    case ExpressionType::String:
        return "String";
    }
    throw std::logic_error("cannot print unknown type name");
}

std::string PrettyPrint(BinaryOperation op)
{
    switch (op)
    {
    case BinaryOperation::Less:
        return "<";
    case BinaryOperation::Equals:
        return "==";
    case BinaryOperation::Add:
        return "+";
    case BinaryOperation::Substract:
        return "-";
    case BinaryOperation::Multiply:
        return "*";
    case BinaryOperation::Divide:
        return "/";
    case BinaryOperation::Modulo:
        return "%";
    }
    return "?";
}

std::string PrettyPrint(UnaryOperation op)
{
    switch (op)
    {
    case UnaryOperation::Minus:
        return "-";
    case UnaryOperation::Plus:
        return "+";
    }
    return "?";
}

ExpressionType GetBinaryOperationResultType(BinaryOperation op, ExpressionType left, ExpressionType right)
{
    auto check = [&](bool condition) {
        if (!condition)
        {
            std::string msg = "Operation " + PrettyPrint(op) + " not allowed for types "
                    + PrettyPrint(left) + " and " + PrettyPrint(right);
            throw std::runtime_error(msg);
        }
    };
    switch (op)
    {
    case BinaryOperation::Less:
    case BinaryOperation::Equals:
        check(left == right);
        return ExpressionType::Boolean;
    case BinaryOperation::Add:
        check(left == right && left != ExpressionType::Boolean);
        return left;
    case BinaryOperation::Substract:
    case BinaryOperation::Multiply:
    case BinaryOperation::Divide:
    case BinaryOperation::Modulo:
        check(left == ExpressionType::Number && right == ExpressionType::Number);
        return ExpressionType::Number;
    }
    throw std::logic_error("GetBinaryOperationResultType() not implemented for this type");
}

ExpressionType GetUnaryOperationResultType(UnaryOperation op, ExpressionType operandType)
{
    auto check = [&](bool condition) {
        if (!condition)
        {
            std::string msg = "Operation " + PrettyPrint(op) + " not allowed for type "
                    + PrettyPrint(operandType);
            throw std::runtime_error(msg);
        }
    };
    switch (op)
    {
    case UnaryOperation::Minus:
    case UnaryOperation::Plus:
        check(operandType == ExpressionType::Number);
        return ExpressionType::Number;
    }
    throw std::logic_error("GetUnaryOperationResultType() not implemented for this type");
}
}

CTypeEvaluator::CTypeEvaluator(CFrontendContext &context, CScopeChain<ExpressionType> &variableTypesRef,
                               CScopeChain<IFunctionAST *> &functionsRef)
    : m_context(context)
    , m_variableTypesRef(variableTypesRef)
    , m_functionsRef(functionsRef)
{
}

ExpressionType CTypeEvaluator::EvaluateTypes(IExpressionAST &expr)
{
    expr.Accept(*this);
    ExpressionType result = m_typesStack.back();
    m_typesStack.pop_back();
    return result;
}

void CTypeEvaluator::Visit(CBinaryExpressionAST &expr)
{
    expr.GetLeft().Accept(*this);
    expr.GetRight().Accept(*this);

    size_t top = (m_typesStack.size() - 1);
    ExpressionType left = m_typesStack.at(top - 1);
    ExpressionType right = m_typesStack.at(top);
    ExpressionType resultType = GetBinaryOperationResultType(expr.GetOperation(), left, right);

    expr.SetType(resultType);
    m_typesStack.pop_back();
    m_typesStack.back() = resultType;
}

void CTypeEvaluator::Visit(CUnaryExpressionAST &expr)
{
    expr.GetOperand().Accept(*this);
    ExpressionType resultType = GetUnaryOperationResultType(expr.GetOperation(), m_typesStack.back());
    expr.SetType(resultType);
    m_typesStack.back() = resultType;
}

void CTypeEvaluator::Visit(CLiteralAST &expr)
{
    m_typesStack.push_back(expr.GetType());
}

void CTypeEvaluator::Visit(CCallAST &expr)
{
    const unsigned functionNameId = expr.GetFunctionNameId();
    const auto functionOpt = m_functionsRef.GetSymbol(functionNameId);
    if (!functionOpt)
    {
        std::string fnName = m_context.GetString(functionNameId);
        throw std::logic_error("function " + fnName + " is undefined");
    }
    IFunctionAST &function = **functionOpt;
    const ParameterDeclList &params = function.GetParameters();
    const ExpressionList &args = expr.GetArguments();
    if (params.size() != args.size())
    {
        std::string fnName = m_context.GetString(functionNameId);
        throw std::logic_error("function " + fnName + " requires " + std::to_string(params.size())
                               + " arguments, while " + std::to_string(args.size()) + " provided");
    }
    std::vector<ExpressionType> argTypes = EvaluateArgumentTypes(expr);
    for (size_t i = 0; i < argTypes.size(); ++i)
    {
        ExpressionType expectedType = params.at(i)->GetType();
        if (argTypes.at(i) != params.at(i)->GetType())
        {
            std::string fnName = m_context.GetString(functionNameId);
            throw std::logic_error("function " + fnName + " expects " + PrettyPrint(expectedType)
                                   + " in the " + std::to_string(i) + " parameter");
        }
    }
    expr.SetType(function.GetReturnType());
}

void CTypeEvaluator::Visit(CVariableRefAST &expr)
{
    if (auto typeOpt = m_variableTypesRef.GetSymbol(expr.GetNameId()))
    {
        expr.SetType(*typeOpt);
        m_typesStack.push_back(*typeOpt);
    }
    else
    {
        std::string varName = m_context.GetString(expr.GetNameId());
        throw std::logic_error("used undefined variable " + varName);
    }
}

void CTypeEvaluator::Visit(CParameterDeclAST &expr)
{
    m_typesStack.push_back(expr.GetType());
}

std::vector<ExpressionType> CTypeEvaluator::EvaluateArgumentTypes(CCallAST &expr)
{
    const ExpressionList &args = expr.GetArguments();
    for (const auto &pExpr : args)
    {
        pExpr->Accept(*this);
    }
    std::vector<ExpressionType> argTypes;
    std::move(m_typesStack.end() - ptrdiff_t(args.size()), m_typesStack.end(), std::back_inserter(argTypes));
    return argTypes;
}

CTypecheckVisitor::CTypecheckVisitor(CFrontendContext &context)
    : m_context(context)
    , m_evaluator(m_context, m_variableTypes, m_functions)
{
}

void CTypecheckVisitor::RunSemanticPass(CProgramAst &ast)
{
    m_functions.PushScope();
    for (const auto &pFunction : ast.GetFunctions())
    {
        const unsigned nameId = pFunction->GetNameId();
        if (m_functions.HasSymbol(nameId))
        {
            std::string fnName = m_context.GetString(nameId);
            m_context.PrintError("function " + fnName + " should not be redefined");
        }
        else
        {
            m_functions.DefineSymbol(nameId, pFunction.get());
        }
    }
    for (const auto &pFunction : ast.GetFunctions())
    {
        CheckTypes(*pFunction);
    }
}

void CTypecheckVisitor::CheckTypes(IFunctionAST &ast)
{
    m_variableTypes.PushScope();
    m_returnType = ast.GetReturnType();
    for (const auto &pParam : ast.GetParameters())
    {
        m_variableTypes.DefineSymbol(pParam->GetName(), pParam->GetType());
    }
    for (const auto &pStmt : ast.GetBody())
    {
        pStmt->Accept(*this);
    }
    m_variableTypes.PopScope();
}

void CTypecheckVisitor::Visit(CPrintAST &ast)
{
    m_evaluator.EvaluateTypes(ast.GetValue());
}

void CTypecheckVisitor::Visit(CAssignAST &ast)
{
    ExpressionType type = m_evaluator.EvaluateTypes(ast.GetValue());
    unsigned nameId = ast.GetNameId();
    if (auto typeOpt = m_variableTypes.GetSymbol(nameId))
    {
        if (type != *typeOpt)
        {
            std::string varName = m_context.GetString(nameId);
            throw std::logic_error("Cannot reassign variable " + varName + " to different type");
        }
    }
    else
    {
        m_variableTypes.DefineSymbol(nameId, type);
    }
}

void CTypecheckVisitor::Visit(CReturnAST &ast)
{
    ExpressionType type = m_evaluator.EvaluateTypes(ast.GetValue());
    if (type != *m_returnType)
    {
        std::string typeName = PrettyPrint(type);
        throw std::logic_error("Function cannot return value of type " + typeName);
    }
}

void CTypecheckVisitor::Visit(CWhileAst &ast)
{
    CheckConditionalAstTypes(ast.GetCondition(), ast.GetBody());
}

void CTypecheckVisitor::Visit(CRepeatAst &ast)
{
    CheckConditionalAstTypes(ast.GetCondition(), ast.GetBody());
}

void CTypecheckVisitor::Visit(CIfAst &ast)
{
    CheckConditionalAstTypes(ast.GetCondition(), ast.GetThenBody());
    for (const auto &pStmt : ast.GetElseBody())
    {
        pStmt->Accept(*this);
    }
}

void CTypecheckVisitor::CheckConditionalAstTypes(IExpressionAST &condition, const StatementsList &body)
{
    ExpressionType type = m_evaluator.EvaluateTypes(condition);
    if (type != ExpressionType::Boolean)
    {
        throw std::logic_error("Cannot use " + PrettyPrint(type) + " in condition, expected Boolean");
    }
    for (const auto &pStmt : body)
    {
        pStmt->Accept(*this);
    }
}

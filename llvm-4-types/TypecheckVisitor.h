#include "ASTVisitor.h"
#include "AST.h"
#include "Utility.h"

class CFrontendContext;

// Класс рекурсивно расставляет и проверяет типы в подвыражениях данного выражения.
// Для расстановки типов в программе достаточно обработать один раз на каждое выражение.
class CTypeEvaluator : protected IExpressionVisitor
{
public:
    CTypeEvaluator(CFrontendContext &context, CScopeChain<ExpressionType> &variableTypesRef,
                   CScopeChain<IFunctionAST*> &functionsRef);

    ExpressionType EvaluateTypes(IExpressionAST & expr);

    // IExpressionVisitor interface
protected:
    void Visit(CBinaryExpressionAST &expr) override;
    void Visit(CUnaryExpressionAST &expr) override;
    void Visit(CLiteralAST &expr) override;
    void Visit(CCallAST &expr) override;
    void Visit(CVariableRefAST &expr) override;
    void Visit(CParameterDeclAST &expr) override;

private:
    std::vector<ExpressionType> EvaluateArgumentTypes(CCallAST &expr);

    CFrontendContext & m_context;
    std::vector<ExpressionType> m_typesStack;
    CScopeChain<ExpressionType> &m_variableTypesRef;
    CScopeChain<IFunctionAST*> &m_functionsRef;
};

class CTypecheckVisitor : protected IStatementVisitor
{
public:
    CTypecheckVisitor(CFrontendContext & context);

    void RunSemanticPass(CProgramAst &ast);

protected:
    // Расставляет и проверяет типы в выражениях.
    // Проверяет семантические правила в функции:
    //  - наличие объявлений переменных,
    //  - наличие хотя бы одного return в функции.
    void CheckTypes(IFunctionAST & ast);

    // IStatementVisitor interface
    void Visit(CPrintAST &ast) override;
    void Visit(CAssignAST &ast) override;
    void Visit(CReturnAST &ast) override;
    void Visit(CWhileAst &ast) override;
    void Visit(CRepeatAst &ast) override;
    void Visit(CIfAst &ast) override;
    void CheckConditionalAstTypes(IExpressionAST &condition, const StatementsList &body);

private:
    CFrontendContext & m_context;
    CScopeChain<ExpressionType> m_variableTypes;
    CScopeChain<IFunctionAST*> m_functions;
    boost::optional<ExpressionType> m_returnType;
    CTypeEvaluator m_evaluator;
};

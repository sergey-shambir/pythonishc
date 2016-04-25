#pragma once

#include <unordered_map>
#include <memory>
#include <stack>
#include <boost/optional.hpp>
#include "VariablesScope.h"

class CStringPool;
class IFunctionAST;

class CFrontendContext : protected IInterpreterContext
{
public:
    CFrontendContext(std::ostream &errors, CStringPool & pool);
    ~CFrontendContext();

    std::unique_ptr<CVariablesScope> MakeScope();
    CVariablesScope & GetCurrentScope();

    llvm::Function *GetPrintF()const;
    llvm::Function *TryGetFunction(unsigned nameId)const;
    void AddFunction(unsigned nameId, llvm::Function *function);

    std::string GetString(unsigned stringId)const;
    void PrintError(std::string const& message) const;
    unsigned GetErrorsCount()const;

    llvm::LLVMContext & GetLLVMContext();
    llvm::Module &GetModule();

protected:
    void AssignVariable(unsigned nameId, llvm::Value *value) override;
    bool HasVariable(unsigned nameId)const override;
    void RemoveVariable(unsigned nameId) override;
    llvm::Value *TryGetVariableValue(unsigned nameId)const override;
    void EnterScope(CVariablesScope &scope) override;
    void ExitScope() override;

private:
    std::unordered_map<unsigned, llvm::Value *> m_variables;
    std::unordered_map<unsigned, llvm::Function *> m_functions;
    void InitLibCBuiltins();

    mutable unsigned m_errorsCount = 0;
    llvm::Function *m_printf;
    CStringPool & m_pool;
    std::stack<CVariablesScope *> m_scopes;
    std::ostream &m_errors;
    std::unique_ptr<llvm::LLVMContext> m_pLLVMContext;
    std::unique_ptr<llvm::Module> m_pModule;
};

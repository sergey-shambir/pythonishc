#pragma once

#include <unordered_map>
#include <memory>
#include <stack>
#include <boost/optional.hpp>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#include <llvm/IR/Value.h>
#include <llvm/IR/Function.h>
#pragma clang diagnostic pop

class CStringPool;
class CVariablesScope;
class IFunctionAST;

class CFrontendContext
{
public:
    CFrontendContext(std::ostream &errors, CStringPool & pool);
    ~CFrontendContext();

    void DefineVariable(unsigned nameId, llvm::Value *value);
    void AssignVariable(unsigned nameId, llvm::Value *value);
    llvm::Value *TryGetVariableValue(unsigned nameId)const;

    void PushScope(std::unique_ptr<CVariablesScope> && scope);
    std::unique_ptr<CVariablesScope> PopScope();

    llvm::Function *GetPrintF()const;
    llvm::Function *TryGetFunction(unsigned nameId)const;
    void AddFunction(unsigned nameId, llvm::Function *function);

    std::string GetString(unsigned stringId)const;
    void PrintError(std::string const& message) const;
    unsigned GetErrorsCount()const;

    llvm::LLVMContext & GetLLVMContext();
    llvm::Module &GetModule();

private:
    CVariablesScope *FindScopeWithVariable(unsigned nameId)const;
    void InitLibCBuiltins();

    std::unordered_map<unsigned, llvm::Function *> m_functions;
    mutable unsigned m_errorsCount = 0;
    llvm::Function *m_printf;
    CStringPool & m_pool;
    std::vector<std::unique_ptr<CVariablesScope>> m_scopes;
    std::ostream &m_errors;
    std::unique_ptr<llvm::LLVMContext> m_pLLVMContext;
    std::unique_ptr<llvm::Module> m_pModule;
};

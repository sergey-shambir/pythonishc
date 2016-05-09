#pragma once

#include <unordered_map>
#include <memory>
#include <stack>
#include <boost/optional.hpp>

namespace llvm
{
class AllocaInst;
class Function;
class LLVMContext;
class Module;
}

class CStringPool;
class CVariablesScope;
class IFunctionAST;

class CFrontendContext
{
public:
    CFrontendContext(std::ostream &errors, CStringPool & pool);
    ~CFrontendContext();

    // Ничего не сделает, если передать nullptr.
    void AddVariable(unsigned nameId, llvm::AllocaInst *value);
    // Может вернуть nullptr.
    llvm::AllocaInst *GetVariable(unsigned nameId)const;

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

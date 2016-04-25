#include "AST.h"
#include "InterpreterContext.h"
#include "StringPool.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <iostream>


CInterpreterContext::CInterpreterContext(std::ostream &errors, CStringPool &pool)
    : m_pool(pool)
    , m_errors(errors)
    , m_pLLVMContext(new llvm::LLVMContext)
    , m_pModule(new llvm::Module("jit interpreter", *m_pLLVMContext))
{
    InitLibCBuiltins();
}

CInterpreterContext::~CInterpreterContext()
{
}

std::unique_ptr<CVariablesScope> CInterpreterContext::MakeScope()
{
    return std::unique_ptr<CVariablesScope>(new CVariablesScope(*this));
}

CVariablesScope &CInterpreterContext::GetCurrentScope()
{
    return *m_scopes.top();
}

llvm::Function *CInterpreterContext::GetPrintF() const
{
    return m_printf;
}

void CInterpreterContext::AssignVariable(unsigned nameId, llvm::Value *value)
{
    if (value)
    {
        m_variables[nameId] = value;
    }
}

bool CInterpreterContext::HasVariable(unsigned nameId) const
{
    return (0 != m_variables.count(nameId));
}

void CInterpreterContext::RemoveVariable(unsigned nameId)
{
    m_variables.erase(nameId);
}

llvm::Value *CInterpreterContext::TryGetVariableValue(unsigned nameId) const
{
    try
    {
        return m_variables.at(nameId);
    }
    catch (std::exception const&)
    {
        std::string message = "unknown variable " + m_pool.GetString(nameId);
        throw std::runtime_error(message);
    }
}

llvm::Function *CInterpreterContext::TryGetFunction(unsigned nameId) const
{
    try
    {
        return m_functions.at(nameId);
    }
    catch (std::exception const&)
    {
        std::string message = "unknown function " + m_pool.GetString(nameId);
        throw std::runtime_error(message);
    }
}

void CInterpreterContext::AddFunction(unsigned nameId, llvm::Function *function)
{
    if (function)
    {
        m_functions[nameId] = function;
    }
}

std::string CInterpreterContext::GetString(unsigned stringId) const
{
    return m_pool.GetString(stringId);
}

void CInterpreterContext::PrintError(const std::string &message)const
{
    m_errors << "  Error: " << message << std::endl;
}

llvm::LLVMContext &CInterpreterContext::GetLLVMContext()
{
    return *m_pLLVMContext;
}

llvm::Module &CInterpreterContext::GetModule()
{
    return *m_pModule;
}

void CInterpreterContext::EnterScope(CVariablesScope &scope)
{
    m_scopes.push(&scope);
}

void CInterpreterContext::ExitScope()
{
    m_scopes.pop();
}

void CInterpreterContext::InitLibCBuiltins()
{
    auto & context = *m_pLLVMContext;
    std::vector<llvm::Type *> argTypes = {llvm::Type::getInt8PtrTy(context)};
    llvm::FunctionType *fnType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), argTypes, true);

    m_printf = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, "printf", m_pModule.get());
}

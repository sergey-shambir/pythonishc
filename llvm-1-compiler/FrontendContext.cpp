#include "AST.h"
#include "FrontendContext.h"
#include "StringPool.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <iostream>


CFrontendContext::CFrontendContext(std::ostream &errors, CStringPool &pool)
    : m_pool(pool)
    , m_errors(errors)
    , m_pLLVMContext(new llvm::LLVMContext)
    , m_pModule(new llvm::Module("main module", *m_pLLVMContext))
{
    InitLibCBuiltins();
}

CFrontendContext::~CFrontendContext()
{
}

std::unique_ptr<CVariablesScope> CFrontendContext::MakeScope()
{
    return std::unique_ptr<CVariablesScope>(new CVariablesScope(*this));
}

CVariablesScope &CFrontendContext::GetCurrentScope()
{
    return *m_scopes.top();
}

llvm::Function *CFrontendContext::GetPrintF() const
{
    return m_printf;
}

void CFrontendContext::AssignVariable(unsigned nameId, llvm::Value *value)
{
    if (value)
    {
        m_variables[nameId] = value;
    }
}

bool CFrontendContext::HasVariable(unsigned nameId) const
{
    return (0 != m_variables.count(nameId));
}

void CFrontendContext::RemoveVariable(unsigned nameId)
{
    m_variables.erase(nameId);
}

llvm::Value *CFrontendContext::TryGetVariableValue(unsigned nameId) const
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

llvm::Function *CFrontendContext::TryGetFunction(unsigned nameId) const
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

void CFrontendContext::AddFunction(unsigned nameId, llvm::Function *function)
{
    if (function)
    {
        m_functions[nameId] = function;
    }
}

std::string CFrontendContext::GetString(unsigned stringId) const
{
    return m_pool.GetString(stringId);
}

void CFrontendContext::PrintError(const std::string &message)const
{
    ++m_errorsCount;
    m_errors << "  Error: " << message << std::endl;
}

unsigned CFrontendContext::GetErrorsCount() const
{
    return m_errorsCount;
}

llvm::LLVMContext &CFrontendContext::GetLLVMContext()
{
    return *m_pLLVMContext;
}

llvm::Module &CFrontendContext::GetModule()
{
    return *m_pModule;
}

void CFrontendContext::EnterScope(CVariablesScope &scope)
{
    m_scopes.push(&scope);
}

void CFrontendContext::ExitScope()
{
    m_scopes.pop();
}

void CFrontendContext::InitLibCBuiltins()
{
    auto & context = *m_pLLVMContext;
    std::vector<llvm::Type *> argTypes = {llvm::Type::getInt8PtrTy(context)};
    llvm::FunctionType *fnType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), argTypes, true);

    m_printf = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, "printf", m_pModule.get());
}

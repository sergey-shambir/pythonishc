#include "AST.h"
#include "FrontendContext.h"
#include "StringPool.h"
#include "VariablesScope.h"
#include <iostream>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm.hpp>

#include "begin_llvm.h"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include "end_llvm.h"


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

void CFrontendContext::DefineVariable(unsigned nameId, llvm::Value *value)
{
    m_scopes.back()->AssignVariable(nameId, value);
}

void CFrontendContext::AssignVariable(unsigned nameId, llvm::Value *value)
{
    if (CVariablesScope *pScope = FindScopeWithVariable(nameId))
    {
        pScope->AssignVariable(nameId, value);
    }
    else
    {
        DefineVariable(nameId, value);
    }
}

llvm::Value *CFrontendContext::TryGetVariableValue(unsigned nameId) const
{
    if (CVariablesScope *pScope = FindScopeWithVariable(nameId))
    {
        return pScope->GetVariableValue(nameId);
    }
    throw std::runtime_error("unknown variable " + m_pool.GetString(nameId));
}

void CFrontendContext::PushScope(std::unique_ptr<CVariablesScope> &&scope)
{
    m_scopes.emplace_back(std::move(scope));
}

std::unique_ptr<CVariablesScope> CFrontendContext::PopScope()
{
    std::unique_ptr<CVariablesScope> ret(m_scopes.back().release());
    m_scopes.pop_back();
    return ret;
}

llvm::Function *CFrontendContext::GetPrintF() const
{
    return m_printf;
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

CVariablesScope *CFrontendContext::FindScopeWithVariable(unsigned nameId) const
{
    auto range = boost::adaptors::reverse(m_scopes);
    auto it = boost::find_if(range, [=](const auto &pScope) {
        return pScope->HasVariable(nameId);
    });
    if (it != range.end())
    {
        return it->get();
    }
    return nullptr;
}

void CFrontendContext::InitLibCBuiltins()
{
    auto & context = *m_pLLVMContext;
    std::vector<llvm::Type *> argTypes = {llvm::Type::getInt8PtrTy(context)};
    llvm::FunctionType *fnType = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), argTypes, true);

    m_printf = llvm::Function::Create(fnType, llvm::Function::ExternalLinkage, "printf", m_pModule.get());
}

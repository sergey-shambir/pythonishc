#include "AST.h"
#include "FrontendContext.h"
#include "Utility.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#pragma clang diagnostic pop
#include <iostream>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/algorithm.hpp>


CFrontendContext::CFrontendContext(std::ostream &errors, CStringPool &pool)
    : m_pool(pool)
    , m_errors(errors)
{
}

CFrontendContext::~CFrontendContext()
{
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

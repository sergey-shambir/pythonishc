#include "AST.h"
#include "FrontendContext.h"
#include "Utility.h"
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

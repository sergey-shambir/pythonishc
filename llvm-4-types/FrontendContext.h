#pragma once

#include <unordered_map>
#include <memory>
#include <stack>

class CStringPool;
class IFunctionAST;

class CFrontendContext
{
public:
    CFrontendContext(std::ostream &errors, CStringPool & pool);
    ~CFrontendContext();

    std::string GetString(unsigned stringId)const;
    void PrintError(std::string const& message) const;
    unsigned GetErrorsCount()const;

private:
    mutable unsigned m_errorsCount = 0;
    CStringPool & m_pool;
    std::ostream &m_errors;
};

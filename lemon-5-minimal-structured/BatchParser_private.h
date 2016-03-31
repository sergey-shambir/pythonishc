#pragma once

#include <cmath>
#include <cassert>
#include <utility>
#include <memory>
#include "Token.h"
#include "BatchParser.h"
#include "BatchAST.h"

namespace parser_private
{

template <class T, class ...TArgs>
T *NewAST(TArgs&&... args)
{
    return new T(std::forward<TArgs>(args)...);
}

template <class T, class ...TArgs>
std::unique_ptr<T> MakeAST(TArgs&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<TArgs>(args)...));
}

template <class TRuleRecord>
std::unique_ptr<IExpressionAST> TakeExpr(TRuleRecord & record)
{
    std::unique_ptr<IExpressionAST> ret(record);
    record = nullptr;
    return ret;
}

template <class TRuleRecord>
std::unique_ptr<CAbstractBlockAST> TakeBlock(TRuleRecord & record)
{
    std::unique_ptr<CAbstractBlockAST> ret(record);
    record = nullptr;
    return ret;
}

template <class TRuleRecord>
void DestroyBlock(CBatchParser *pParser, TRuleRecord & record)
{
    if (record)
    {
        pParser->ExitBlock();
        delete record;
        record = nullptr;
    }
}

}


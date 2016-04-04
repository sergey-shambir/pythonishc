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
T *New(TArgs&&... args)
{
    return new (std::nothrow) T(std::forward<TArgs>(args)...);
}

template <class T, class ...TArgs>
std::unique_ptr<T> Make(TArgs&&... args)
{
    return std::unique_ptr<T>(new (std::nothrow) T(std::forward<TArgs>(args)...));
}

template <class T>
std::unique_ptr<T> Take(T* & stackRecord)
{
    std::unique_ptr<T> ret(stackRecord);
    stackRecord = nullptr;
    return ret;
}

template <class T>
T *Move(T *& stackRecord)
{
    T *ret = stackRecord;
    stackRecord = nullptr;
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

using ExpressionListPtr = ExpressionList*;
using ExpressionPtr = IExpressionAST*;
using BlockPtr = CAbstractBlockAST*;

}


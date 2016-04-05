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

template <class TNode, class TRuleNode, class ...TArgs>
void EmplaceAST(TRuleNode *& stackRecord, TArgs&&... args)
{
    stackRecord = new (std::nothrow) TNode(std::forward<TArgs>(args)...);
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
void MoveAST(T *& stackRecord, T *& targetRecord)
{
    targetRecord = stackRecord;
    stackRecord = nullptr;
}

template <class T>
void Destroy(T *& stackRecord)
{
    delete stackRecord;
    stackRecord = nullptr;
}

template <class TTarget, class TItem>
void CreateList(TTarget *& target, TItem *& item)
{
    auto list = Make<TTarget>();
    if (item)
    {
        list->emplace_back(Take(item));
    }
    target = list.release();
};

template <class TTarget, class TItem>
void ConcatList(TTarget *& target, TTarget *& list, TItem *& item)
{
    auto pList = Take(list);
    if (item)
    {
        pList->emplace_back(Take(item));
    }
    target = pList.release();
};

using ExpressionListPtr = ExpressionList*;
using StatementListPtr = StatementsList*;
using StatementPtr = IStatementAST*;
using ExpressionPtr = IExpressionAST*;

}


#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <memory>
#include "BatchAST.h"

struct SToken;
class CInterpreterContext;

/// Wraps LEMON generated parser with Object-Oriented API.
class CBatchParser
{
public:
    CBatchParser(CInterpreterContext & context);
    ~CBatchParser();

    bool Advance(int tokenId, SToken const& tokenData);
#ifndef NDEBUG
    void StartDebugTrace(FILE *output);
#endif

    void OnError(SToken const& token);
    void OnStackOverflow();
    void OnFatalError();
    void AddStatement(IStatementAST *stmt);

    template <class T, class ...TArgs>
    T *MakeAST(TArgs&&... args)
    {
        m_nodePool.emplace_back(new T(std::forward<TArgs>(args)...));
        return static_cast<T*>(m_nodePool.back().get());
    }

private:
#ifndef NDEBUG
    std::string m_tracePrompt;
#endif
    bool m_isFatalError = false;
    void *m_parser = nullptr;
    CInterpreterContext & m_context;
    std::vector<IStatementAST*> m_program;
    std::vector<std::unique_ptr<IAnyAST>> m_nodePool;
};

#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <memory>
#include <stack>
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

    CValue GetStringLiteral(unsigned stringId)const;

    void AddStatement(IStatementASTUniquePtr &&stmt);
    void AddFunction(IFunctionASTUniquePtr && function);

private:
#ifndef NDEBUG
    std::string m_tracePrompt;
#endif
    bool m_isFatalError = false;
    void *m_parser = nullptr;

    CProgramAst m_program;
};

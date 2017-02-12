#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <memory>
#include <stack>
#include "AST.h"

struct Token;
class CInterpreterContext;

/// Wraps LEMON generated parser with Object-Oriented API.
class CParser
{
public:
    CParser(CInterpreterContext & context);
    ~CParser();

    bool Advance(int tokenId, Token const& tokenData);
#ifndef NDEBUG
    void StartDebugTrace(FILE *output);
#endif

    void OnError(Token const& token);
    void OnStackOverflow();
    void OnFatalError();

    CValue GetStringLiteral(unsigned stringId)const;

    void AddStatement(IStatementAstUniquePtr &&stmt);
    void AddFunction(IFunctionAstUniquePtr && function);

private:
#ifndef NDEBUG
    std::string m_tracePrompt;
#endif
    bool m_isFatalError = false;
    void *m_parser = nullptr;

    CInterpreterContext & m_context;
    CProgramAst m_program;
};

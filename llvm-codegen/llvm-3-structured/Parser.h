#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include <memory>
#include <stack>
#include "AST.h"

struct Token;
class CFrontendContext;

/// Wraps LEMON generated parser with Object-Oriented API.
class CParser
{
public:
    CParser(CFrontendContext & context);
    ~CParser();

    bool Advance(int tokenId, Token const& tokenData);
#ifndef NDEBUG
    void StartDebugTrace(FILE *output);
#endif
    std::unique_ptr<CProgramAst> TakeProgram();

    void OnError(Token const& token);
    void OnStackOverflow();
    void OnFatalError();

    std::string GetStringLiteral(unsigned stringId)const;
    void AddFunction(IFunctionASTUniquePtr && function);

private:
#ifndef NDEBUG
    std::string m_tracePrompt;
#endif
    bool m_isFatalError = false;
    void *m_parser = nullptr;

    CFrontendContext & m_context;
    std::unique_ptr<CProgramAst> m_pProgram;
};

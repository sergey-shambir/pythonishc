#pragma once
#include <string>

struct Token;
class CInterpreterContext;

/// Wraps LEMON generated parser with Object-Oriented API.
class CBatchParser
{
public:
    CBatchParser(CInterpreterContext & context);
    ~CBatchParser();

    bool Advance(int tokenId, Token const& tokenData);
#ifndef NDEBUG
    void StartDebugTrace(FILE *output);
#endif

    void OnError(Token const& token);
    void OnStackOverflow();

    void AssignVariable(unsigned stringId, double value);
    double GetVariableValue(unsigned stringId);
    void PrintResult(double value);

private:
#ifndef NDEBUG
    std::string m_tracePrompt;
#endif
    bool m_isErrorState = false;
    void *m_parser = nullptr;
    CInterpreterContext & m_context;
};

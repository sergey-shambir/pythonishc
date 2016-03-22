#pragma once
#include <string>

struct SToken;

/// Wraps LEMON generated parser with Object-Oriented API.
class CCalcParser
{
public:
    CCalcParser();
    ~CCalcParser();

    bool Advance(int tokenId, SToken const& tokenData);
#ifndef NDEBUG
    void StartDebugTrace(FILE *output);
#endif

    void OnError(SToken const& token);
    void OnStackOverflow();
    void PrintResult(double value);

private:
#ifndef NDEBUG
    std::string m_tracePrompt;
#endif
    bool m_isErrorState = false;
    void *m_parser = nullptr;
};

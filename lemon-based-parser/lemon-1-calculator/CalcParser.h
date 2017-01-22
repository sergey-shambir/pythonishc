#pragma once
#include "ICalcParser.h"

#include <string>

struct Token;

class CCalcParser : public ICalcParser
{
public:
    CCalcParser();
    ~CCalcParser();

    bool Advance(int tokenId, Token const& tokenData) final;
#ifndef NDEBUG
    void StartDebugTrace(FILE *output) final;
#endif

    void OnError(Token const& token);
    void OnStackOverflow();
    void PrintResult(double value);

private:
#ifndef NDEBUG
    // Префиксом отладочных сообщений владеет C++ код,
    //  даже если префикс - пустая строка
    std::string m_tracePrompt;
#endif
    bool m_isErrorState = false;
    void *m_parser = nullptr;
};

#pragma once

struct SToken;

/// Wraps LEMON generated parser with Object-Oriented API.
class CCalcParser
{
public:
    CCalcParser();
    ~CCalcParser();

    bool Advance(int tokenId, SToken const& tokenData);

    void OnError(SToken const& token);
    void OnStackOverflow();
    void PrintResult(double value);

private:
    bool m_isErrorState = false;
    void *m_parser = nullptr;
};

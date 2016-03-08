#pragma once

struct SToken;
class CInterpreterContext;

/// Wraps LEMON generated parser with Object-Oriented API.
class CBatchParser
{
public:
    CBatchParser(CInterpreterContext & context);
    ~CBatchParser();

    bool Advance(int tokenId, SToken const& tokenData);

    void OnError(SToken const& token);
    void OnStackOverflow();

    void AssignVariable(unsigned stringId, double value);
    double GetVariableValue(unsigned stringId);
    void PrintResult(double value);

private:
    bool m_isErrorState = false;
    void *m_parser = nullptr;
    CInterpreterContext & m_context;
};

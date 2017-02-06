#pragma once
#include <cstdio>

struct Token;

/// Фасад для созданного утилитой lemon парсера,
///  предоставляющий объектно-ориентированный интерфейс
class ICalcParser
{
public:
    virtual ~ICalcParser() = default;

    virtual bool Advance(int tokenId, Token const& tokenData) = 0;
#ifndef NDEBUG
    virtual void StartDebugTrace(FILE *output) = 0;
#endif
};

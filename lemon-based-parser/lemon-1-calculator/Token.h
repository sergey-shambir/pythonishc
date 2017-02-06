#pragma once

struct Token
{
    // Позиция в исходном коде.
    unsigned position;
    // Числовое значение литерала (0 для остальных токенов).
    double value;
};

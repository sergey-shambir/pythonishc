#pragma once
#include "string_view.h"

class CCalculator
{
public:
    CCalculator() = delete;

    // Выполняет разбор выражений вида "7 / 2 + 12 - 3 * 4 + 17 - 2 * 7"
    // Вычисляет и возвращает результат.
    static double ParseExpr(std::string_view &ref);

private:
    static double ParseDouble(std::string_view &ref);
    static double ParseExprMul(std::string_view &ref);
    static double ParseExprSum(std::string_view &ref);
    static void SkipSpaces(std::string_view &ref);
};

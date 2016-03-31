#pragma once
#include "string_view.h"

struct Calculator
{
    Calculator() = delete;

    // Выполняет разбор выражений вида "7 / 2 + 12 - 3 * 4 + 17 - 2 * 7"
    // Вычисляет и возвращает результат.
    static double parseExpr(std::string_view &ref);

private:
    static double parseDouble(std::string_view &ref);
    static double parseExprMul(std::string_view &ref);
    static double parseExprSum(std::string_view &ref);
    static void skipSpaces(std::string_view &ref);
};

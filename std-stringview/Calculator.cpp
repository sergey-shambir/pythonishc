#include "Calculator.h"
#include <limits>
#include <cctype>


double Calculator::parseExpr(std::string_view &ref)
{
    double value = parseExprSum(ref);
    skipSpaces(ref);
    if (!ref.empty())
    {
        value = std::numeric_limits<double>::quiet_NaN();
    }
    return value;
}

double Calculator::parseDouble(std::string_view &ref)
{
    skipSpaces(ref);

    double value = 0;
    bool parsedAny = false;
    while (!ref.empty() && std::isdigit(ref[0]))
    {
        parsedAny = true;
        const int digit = ref[0] - '0';
        value = value * 10.0 + double(digit);
        ref.remove_prefix(1);
    }
    if (!parsedAny)
    {
        return std::numeric_limits<double>::quiet_NaN();
    }

    if (ref.empty() || (ref[0] != '.'))
    {
        return value;
    }
    ref.remove_prefix(1);
    double factor = 1.0;
    while (!ref.empty() && std::isdigit(ref[0]))
    {
        const int digit = ref[0] - '0';
        factor *= 0.1;
        value += factor * double(digit);
        ref.remove_prefix(1);
    }

    return value;
}

double Calculator::parseExprMul(std::string_view &ref)
{
    double value = parseDouble(ref);
    while (true)
    {
        skipSpaces(ref);
        if (!ref.empty() && ref[0] == '*')
        {
            ref.remove_prefix(1);
            value *= parseDouble(ref);
        }
        else if (!ref.empty() && ref[0] == '/')
        {
            ref.remove_prefix(1);
            value /= parseDouble(ref);
        }
        else
        {
            break;
        }
    }

    return value;
}

double Calculator::parseExprSum(std::string_view &ref)
{
    double value = parseDouble(ref);
    while (true)
    {
        skipSpaces(ref);

        if (!ref.empty() && ref[0] == '+')
        {
            ref.remove_prefix(1);
            value += parseDouble(ref);
        }
        else if (!ref.empty() && ref[0] == '-')
        {
            ref.remove_prefix(1);
            value -= parseDouble(ref);
        }
        else
        {
            break;
        }
    }

    return value;
}

void Calculator::skipSpaces(std::string_view &ref)
{
    size_t i = 0;
    while (i < ref.size() && std::isspace(ref[i]))
        ++i;
    ref.remove_prefix(i);
}

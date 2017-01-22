#include "CalcLexer.h"
#include "CalcGrammar.h"
#include <iostream>
#include <cmath>

CCalcLexer::CCalcLexer(std::string const& sources)
    : m_sources(sources)
    , m_peep(m_sources)
{
}

int CCalcLexer::Scan(Token &data)
{
    SkipSpaces();
    data.value = 0;
    data.position = unsigned(m_peep.data() - m_sources.c_str());

    if (m_peep.empty())
    {
        return 0;
    }
    double value = ParseDouble();
    if (!std::isnan(value))
    {
        data.value = value;
        return TK_NUMBER;
    }
    switch (m_peep[0])
    {
    case '+':
        m_peep.remove_prefix(1);
        return TK_PLUS;
    case '-':
        m_peep.remove_prefix(1);
        return TK_MINUS;
    case '*':
        m_peep.remove_prefix(1);
        return TK_STAR;
    case '/':
        m_peep.remove_prefix(1);
        return TK_SLASH;
    case '%':
        m_peep.remove_prefix(1);
        return TK_PERCENT;
    case '(':
        m_peep.remove_prefix(1);
        return TK_LPAREN;
    case ')':
        m_peep.remove_prefix(1);
        return TK_RPAREN;
    }

    // on error, return EOF
    std::cerr << "Lexical error at position " << data.position << std::endl;
    return 0;
}

double CCalcLexer::ParseDouble()
{
    double value = 0;
    bool parsedAny = false;
    while (!m_peep.empty() && std::isdigit(m_peep[0]))
    {
        parsedAny = true;
        const int digit = m_peep[0] - '0';
        value = value * 10.0f + double(digit);
        m_peep.remove_prefix(1);
    }
    if (!parsedAny)
    {
        return std::numeric_limits<double>::quiet_NaN();
    }
    if (m_peep.empty() || (m_peep[0] != '.'))
    {
        return value;
    }
    m_peep.remove_prefix(1);
    double factor = 1.f;
    while (!m_peep.empty() && std::isdigit(m_peep[0]))
    {
        const int digit = m_peep[0] - '0';
        factor *= 0.1f;
        value += factor * double(digit);
        m_peep.remove_prefix(1);
    }

    return value;
}

void CCalcLexer::SkipSpaces()
{
    size_t i = 0;
    while (i < m_peep.size() && std::isspace(m_peep[i]))
    {
        ++i;
    }
    m_peep.remove_prefix(i);
}

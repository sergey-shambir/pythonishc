#include "BatchLexer.h"
#include "BatchGrammar.h"
#include <iostream>

CBatchLexer::CBatchLexer(unsigned lineNo, std::string const& sources, CStringPool &stringPool)
    : m_lineNo(lineNo)
    , m_sources(sources)
    , m_peep(m_sources)
    , m_stringPool(stringPool)
{
}

int CBatchLexer::Scan(SToken &data)
{
    SkipSpaces();
    data.line = m_lineNo;
    data.column = 1 + unsigned(m_peep.data() - m_sources.c_str());

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
    case '=':
        m_peep.remove_prefix(1);
        return TK_ASSIGN;
    }
    std::string id = ParseIdentifier();
    if (!id.empty())
    {
        if (id == "print")
        {
            return TK_PRINT;
        }
        data.stringId = m_stringPool.Insert(id);
        return TK_ID;
    }

    // on error, return EOF
    std::cerr << "Lexical error at (" << data.line << "," << data.column << ")." << std::endl;
    return 0;
}

double CBatchLexer::ParseDouble()
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

std::string CBatchLexer::ParseIdentifier()
{
    size_t size = 0;
    while (!m_peep.empty() && std::isalnum(m_peep[size]))
    {
        ++size;
    }
    std::string value = m_peep.substr(0, size).to_string();
    m_peep.remove_prefix(size);
    return value;
}

void CBatchLexer::SkipSpaces()
{
    size_t count = 0;
    while (count < m_peep.size() && std::isspace(m_peep[count]))
    {
        ++count;
    }
    m_peep.remove_prefix(count);
}

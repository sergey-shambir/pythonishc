#include "Lexer.h"
#include "Grammar.h"
#include <iostream>
#include <sstream>
#include <cmath>
#include <limits>


namespace {
static const int EOF_TOKEN = 0;

class OperatorsTable
{
public:
    OperatorsTable()
    {
        memset(m_mapping, EOF_TOKEN, SIZE);
        Set('<', TK_LESS);
        Set('+', TK_PLUS);
        Set('-', TK_MINUS);
        Set('*', TK_STAR);
        Set('/', TK_SLASH);
        Set('%', TK_PERCENT);
        Set(',', TK_COMMA);
        Set('[', TK_LBRACKET);
        Set(']', TK_RBRACKET);
        Set('(', TK_LPAREN);
        Set(')', TK_RPAREN);
        Set('=', TK_ASSIGN);
    }

    // Returns EOF_TOKEN if `ch` is not operator.
    int Get(char ch)const
    {
        return m_mapping[static_cast<size_t>(ch)];
    }

private:
    void Set(char ch, int value)
    {
        m_mapping[static_cast<size_t>(ch)] = value;
    }

    static const size_t SIZE = 1u + std::numeric_limits<unsigned char>::max();
    int m_mapping[SIZE];
};

const OperatorsTable OPERATORS_TABLE;

const std::map<std::string, int> KEYWORDS = {
    { "do",     TK_DO },
    { "if",     TK_IF },
    { "end",    TK_END },
    { "else",   TK_ELSE },
    { "while",  TK_WHILE },
    { "print",  TK_PRINT },
    { "return", TK_RETURN },
    { "function", TK_FUNCTION },
    { "String", TK_STRING_TYPE },
    { "Number", TK_NUMBER_TYPE },
    { "Boolean",TK_BOOLEAN_TYPE },
};

} // anonymous namespace


CLexer::CLexer(unsigned lineNo, std::string const& line, CStringPool &pool, const ErrorHandler &handler)
    : m_lineNo(lineNo)
    , m_sources(line)
    , m_peep(m_sources)
    , m_stringPool(pool)
    , m_onError(handler)
{
}

int CLexer::Scan(SToken &data)
{
    SkipSpaces();
    data.line = m_lineNo;
    data.column = 1 + unsigned(m_peep.data() - m_sources.c_str());

    if (m_peep.empty())
    {
        return EOF_TOKEN;
    }
    const int operatorId = OPERATORS_TABLE.Get(m_peep[0]);
    switch (operatorId)
    {
    case EOF_TOKEN:
        break; // not an operator
    case TK_ASSIGN:
        if ((m_peep.length() >= 2) && (m_peep[1] == '='))
        {
            m_peep.remove_prefix(2);
            return TK_EQUALS;
        }
        m_peep.remove_prefix(1);
        return operatorId;
    default:
        m_peep.remove_prefix(1);
        return operatorId;
    }

    if (ParseDouble(data))
    {
        return TK_NUMBER_VALUE;
    }
    if (ParseString(data))
    {
        return TK_STRING_VALUE;
    }
    boost::string_ref idRef = ParseIdentifier();
    if (!idRef.empty())
    {
        return AcceptIdOrKeyword(data, idRef.to_string());
    }

    // on error, return EOF
    OnError("unknown lexem", data);
    return EOF_TOKEN;
}

bool CLexer::ParseDouble(SToken &data)
{
    double value = 0;
    bool parsedAny = false;
    while (!m_peep.empty() && std::isdigit(m_peep[0]))
    {
        parsedAny = true;
        const int digit = m_peep[0] - '0';
        value = value * 10.0 + double(digit);
        m_peep.remove_prefix(1);
    }
    if (parsedAny && !m_peep.empty() && (m_peep[0] == '.'))
    {
        m_peep.remove_prefix(1);
        double factor = 1.;
        while (!m_peep.empty() && std::isdigit(m_peep[0]))
        {
            const int digit = m_peep[0] - '0';
            factor *= 0.1;
            value += factor * double(digit);
            m_peep.remove_prefix(1);
        }
    }
    if (parsedAny)
    {
        data.numberValue = value;
    }

    return parsedAny;
}

boost::string_ref CLexer::ParseIdentifier()
{
    size_t size = 0;
    while (!m_peep.empty() && std::isalnum(m_peep[size]))
    {
        ++size;
    }
    boost::string_ref value = m_peep.substr(0, size);
    m_peep.remove_prefix(size);
    return value;
}

void CLexer::SkipSpaces()
{
    size_t count = 0;
    while (count < m_peep.size() && std::isspace(m_peep[count]))
    {
        ++count;
    }
    m_peep.remove_prefix(count);
}

bool CLexer::ParseString(SToken &data)
{
    if (m_peep[0] != '\"')
    {
        return false;
    }
    m_peep.remove_prefix(1);
    size_t quotePos = m_peep.find('\"');
    if (quotePos == boost::string_ref::npos)
    {
        OnError("missed end quote", data);
        data.stringId = m_stringPool.Insert(m_peep.to_string());
        m_peep.clear();

        return true;
    }

    boost::string_ref value = m_peep.substr(0, quotePos);
    data.stringId = m_stringPool.Insert(value.to_string());
    m_peep.remove_prefix(quotePos + 1);

    return true;
}

int CLexer::AcceptIdOrKeyword(SToken &data, std::string && id)
{
    if (id == "true")
    {
        data.boolValue = true;
        return TK_BOOLEAN_VALUE;
    }
    else if (id == "false")
    {
        data.boolValue = false;
        return TK_BOOLEAN_VALUE;
    }

    auto it = KEYWORDS.find(id);
    if (it != KEYWORDS.end())
    {
        return it->second;
    }

    data.stringId = m_stringPool.Insert(id);
    return TK_ID;
}

void CLexer::OnError(const char message[], SToken &data)
{
    if (m_onError)
    {
        std::stringstream formatter;
        formatter << message << " at (" << data.line << "," << data.column << ")";
        m_onError(formatter.str());
    }
}

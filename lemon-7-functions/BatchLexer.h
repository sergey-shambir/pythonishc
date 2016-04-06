#pragma once

#include <cctype>
#include <string>
#include <map>
#include <boost/utility/string_ref.hpp>
#include "Token.h"
#include "StringPool.h"

class CBatchLexer
{
public:
    CBatchLexer(unsigned lineNo, std::string const& sources, CStringPool & stringPool);

    // returns next token or 0 on EOF
    // tokens defined in CalcGrammar.h
    int Scan(SToken &data);

private:
    // returns NaN if cannot parse double.
    double ParseDouble();
    std::string ParseIdentifier();
    void SkipSpaces();
    int AcceptIdOrKeyword(SToken &data, std::string && id);

    const unsigned m_lineNo;
    const std::string m_sources;
    boost::string_ref m_peep;
    CStringPool & m_stringPool;
    const std::map<std::string, int> m_keywords;
};

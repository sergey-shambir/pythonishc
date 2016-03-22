#pragma once

#include <string>
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

    const unsigned m_lineNo;
    const std::string m_sources;
    boost::string_ref m_peep;
    CStringPool & m_stringPool;
};

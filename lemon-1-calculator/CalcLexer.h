#pragma once

#include <string>
#include <boost/utility/string_ref.hpp>
#include "Token.h"

class CCalcLexer
{
public:
    CCalcLexer(std::string const& sources);

    // returns next token or 0 on EOF
    // tokens defined in CalcGrammar.h
    int Scan(SToken &data);

private:
    // returns NaN if cannot parse double.
    double ParseDouble();
    void SkipSpaces();

    const std::string m_sources;
    boost::string_ref m_peep;
};

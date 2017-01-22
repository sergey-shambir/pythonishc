#pragma once

#include <string>
#include <boost/utility/string_ref.hpp>
#include "Token.h"

class CCalcLexer
{
public:
    CCalcLexer(std::string const& sources);

    // Возвращает следующий токен (лексему) либо 0, если входной файл кончился.
    // Токены объявлены в CalcGrammar.h
    int Scan(SToken &data);

private:
    // returns NaN if cannot parse double.
    double ParseDouble();
    void SkipSpaces();

    const std::string m_sources;
    boost::string_ref m_peep;
};

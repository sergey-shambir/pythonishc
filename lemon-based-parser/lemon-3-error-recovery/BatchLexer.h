#pragma once

#include <string>
#include <boost/utility/string_ref.hpp>
#include "Token.h"
#include "StringPool.h"

class CBatchLexer
{
public:
    CBatchLexer(unsigned lineNo, std::string const& sources, CStringPool & stringPool);

    // Возвращает следующий токен (лексему) либо 0, если входной файл кончился.
    // Токены объявлены в BatchGrammar.h
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

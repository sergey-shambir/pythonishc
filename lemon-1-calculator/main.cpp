#include "CalcParser.h"
#include "CalcLexer.h"
#include "Token.h"
#include <iostream>

bool ParseExpr(std::string const& expr)
{
    CCalcLexer lexer(expr);
    CCalcParser parser;

    SToken token;
    int tokenId = 0;
    do
    {
        tokenId = lexer.Scan(token);
        if (!parser.Advance(tokenId, token))
        {
            return false;
        }
    }
    while (tokenId != 0);
    return true;
}

int main()
{
    std::string line;
    while (std::cin)
    {
        std::getline(std::cin, line);
        ParseExpr(line);
    }

    return 0;
}

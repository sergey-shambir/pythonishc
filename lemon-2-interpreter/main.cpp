#include "BatchParser.h"
#include "BatchLexer.h"
#include "StringPool.h"
#include "InterpreterContext.h"
#include "Token.h"
#include "BatchGrammar.h"
#include <iostream>

bool ConsumeLine(unsigned lineNo, std::string const& expr,
                 CStringPool & stringPool, CBatchParser & parser)
{
    CBatchLexer lexer(lineNo, expr, stringPool);
    SToken token;
    bool ok = true;
    for (int tokenId = lexer.Scan(token); tokenId != 0; tokenId = lexer.Scan(token))
    {
        if (!parser.Advance(tokenId, token))
        {
            ok = false;
            break;
        }
    }
    SToken newline;
    newline.line = lineNo;
    newline.column = 1;
    parser.Advance(TK_NEWLINE, newline);
    return ok;
}

void EnterInterpreterLoop()
{
    CStringPool stringPool;
    CInterpreterContext context(stringPool);
    CBatchParser parser(context);

    std::string line;
    unsigned lineNo = 1;
    while (std::cin)
    {
        std::getline(std::cin, line);
        ConsumeLine(lineNo, line, stringPool, parser);
    }
}

int main()
{
    EnterInterpreterLoop();
    return 0;
}

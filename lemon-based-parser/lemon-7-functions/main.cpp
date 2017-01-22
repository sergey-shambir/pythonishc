#include "BatchParser.h"
#include "BatchLexer.h"
#include "StringPool.h"
#include "InterpreterContext.h"
#include "Token.h"
#include "BatchGrammar.h"
#include <iostream>
#include <time.h>

bool ConsumeLine(unsigned lineNo, std::string const& expr,
                 CStringPool & stringPool, CBatchParser & parser)
{
    CBatchLexer lexer(lineNo, expr, stringPool);
    Token token;
    for (int tokenId = lexer.Scan(token); tokenId != 0; tokenId = lexer.Scan(token))
    {
        if (!parser.Advance(tokenId, token))
        {
            return false;
        }
    }
    token.line = lineNo;
    token.column = 1;
    return parser.Advance(TK_NEWLINE, token);
}

void EnterInterpreterLoop()
{
    CStringPool stringPool;
    CInterpreterContext context(stringPool);
    CBatchParser parser(context);

    // Uncomment code below to trace LALR parser shift/reduce.
//    parser.StartDebugTrace(stderr);

    std::string line;
    unsigned lineNo = 1;
    while (std::getline(std::cin, line))
    {
        if (!ConsumeLine(lineNo, line, stringPool, parser))
        {
            return;
        }
    }
}

int main()
{
    std::srand(static_cast<unsigned>(time(nullptr)));
    EnterInterpreterLoop();
    return 0;
}

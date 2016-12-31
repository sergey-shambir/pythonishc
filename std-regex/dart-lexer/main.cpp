#include "lexer.h"
#include <cstdlib>
#include <cstdio>

std::string getWholeInput()
{
    int c;
    std::string ret;
    while ((c = getchar()) != EOF)
        ret += static_cast<char>(c);

    return ret;
}

/**
 * @brief parseText performs lexical analysis for given text, outputs info to stdout
 * @param text - Dart source code
 */
void parseText(const std::string &text)
{
    Lexer lexer(text);
    for (unsigned index = 1; /*forever*/ ; ++index) {
        const char *prettyName;
        switch (lexer.read())
        {
            default:
                prettyName = "Unknown token";
                break;

            case T_EOF_SYMBOL:
                std::puts("The end.");
                return;

            case T_NUMBER:
                prettyName = "Number";
                break;

            case T_STRING:
                prettyName = "String literal";
                break;

            case T_ACTUAL_KEYWORD:
                prettyName = "Keyword";
                break;

            case T_PSEUDO_KEYWORD:
                prettyName = "Pseudo-keyword";
                break;

            case T_IDENTIFIER:
                prettyName = "Identifier";
                break;

            case T_OPERATOR:
                prettyName = "Operator";
                break;

            case T_ERROR:
                prettyName = "Invalid character";
                break;
        }
        std::printf("#%d: %s with value \"%s\"\n",
                    index,
                    prettyName,
                    lexer.lastText().c_str());
    }
}

int main(int /*argc*/, char */*argv*/[])
{
    std::string text = getWholeInput();
    parseText(text);

    return 0;
}

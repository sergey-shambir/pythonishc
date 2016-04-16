#include "Parser.h"
#include "Lexer.h"
#include "StringPool.h"
#include "Interpreter.h"
#include "Token.h"
#include "Grammar.h"
#include <iostream>
#include <time.h>

int main()
{
    std::srand(static_cast<unsigned>(time(nullptr)));
    CInterpreter interpreter;

    interpreter.EnterLoop(std::cin, std::cout, std::cerr);

    return 0;
}

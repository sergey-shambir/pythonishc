#include <iostream>
#include "Parser.h"

using namespace std;

const char COMMANDS[] = R"***(
for duck QUACKQUACK as toy
quack
quack
fly 2 seconds
quack
)***";

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    try
    {
        Parser parser;
        auto duck = parser.Parse(COMMANDS);

        duck->RunProgram(std::cout);
    }
    catch (const std::exception &ex)
    {
        std::cerr << "FATAL ERROR: " << ex.what() << std::endl;
    }

    return 0;
}

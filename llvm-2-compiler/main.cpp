#include "CompilerDriver.h"
#include <iostream>
#include <time.h>

int main()
{
    std::srand(static_cast<unsigned>(time(nullptr)));
    CCompilerDriver driver(std::cerr);

    if (!driver.Compile(std::cin, "program.o"))
    {
        return 1;
    }

    return 0;
}

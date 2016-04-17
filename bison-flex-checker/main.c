#include "grammar.gen.h"
#include "globals.h"
#include <stdio.h>

int g_errorsCount = 0;

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    const int result = checker_parse();
    if (result != 0 || g_errorsCount != 0)
    {
        puts("Entered text does not match grammar.");
    }
    else
    {
        puts("Entered text is valid program.");
    }

    return 0;
}

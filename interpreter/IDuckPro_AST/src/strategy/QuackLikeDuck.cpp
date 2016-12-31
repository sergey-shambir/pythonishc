#include "QuackLikeDuck.hpp"
#include <stdio.h>

QuackLikeDuck::QuackLikeDuck(unsigned timesToQuack)
    :_timesToQuack(timesToQuack)
{
}

QuackLikeDuck::~QuackLikeDuck()
{
}

void QuackLikeDuck::quack() const
{
    for (unsigned time = 0; time < _timesToQuack; ++time)
    {
        printf("Quack!");
    }
    putchar('\n');
}

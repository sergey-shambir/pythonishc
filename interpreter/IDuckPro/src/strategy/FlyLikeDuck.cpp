#include "FlyLikeDuck.hpp"
#include <stdio.h>

FlyLikeDuck::FlyLikeDuck()
{
}

FlyLikeDuck::~FlyLikeDuck()
{
}

void FlyLikeDuck::fly(Course course, int &distEast, int &distNorth) const
{
    static const int MILES_PER_HOUR = 1;
    switch (course)
    {
    case Course_NORTH:
        distNorth += MILES_PER_HOUR;
        break;
    case Course_EAST:
        distEast += MILES_PER_HOUR;
        break;
    case Course_SOUTH:
        distNorth -= MILES_PER_HOUR;
        break;
    case Course_WEST:
    default:
        distEast -= MILES_PER_HOUR;
        break;
    }
    printf("(flying %s)\n", toString<const char*>(course));
}

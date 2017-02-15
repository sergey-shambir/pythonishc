#include "Rotate.hpp"

Rotate::Rotate()
{
}

Rotate::~Rotate()
{
}

void Rotate::right(Course &course) const
{
    switch (course)
    {
    case Course_NORTH:
        course = Course_EAST;
        break;
    case Course_EAST:
        course = Course_SOUTH;
        break;
    case Course_SOUTH:
        course = Course_WEST;
        break;
    case Course_WEST:
    default:
        course = Course_NORTH;
        break;
    }
}

void Rotate::left(Course &course) const
{
    switch (course)
    {
    case Course_NORTH:
        course = Course_WEST;
        break;
    case Course_EAST:
        course = Course_NORTH;
        break;
    case Course_SOUTH:
        course = Course_EAST;
        break;
    case Course_WEST:
    default:
        course = Course_SOUTH;
        break;
    }
}

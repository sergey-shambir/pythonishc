#include "RotateWrongWay.hpp"

RotateWrongWay::RotateWrongWay()
{
}

RotateWrongWay::~RotateWrongWay()
{
}

void RotateWrongWay::right(Course &course) const
{
    Rotate::left(course);
}

void RotateWrongWay::left(Course &course) const
{
    Rotate::right(course);
}

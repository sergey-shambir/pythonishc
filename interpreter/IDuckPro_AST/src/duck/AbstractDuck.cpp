#include "AbstractDuck.hpp"

AbstractDuck::AbstractDuck(const IFly *flyStrategy,
                           const IQuack *quackStrategy,
                           const IRotate *rotateStrategy)
    :_flyStrategy(flyStrategy)
    ,_quackStrategy(quackStrategy)
    ,_rotateStrategy(rotateStrategy)
    ,_course(Course_NORTH)
    ,_distanceEast(0)
    ,_distanceNorth(0)
{
}

AbstractDuck::~AbstractDuck()
{
}

Course AbstractDuck::getCourse() const
{
    return _course;
}

int AbstractDuck::getDistance(Course course) const
{
    switch (course)
    {
    case Course_NORTH:
        return _distanceNorth;
    case Course_EAST:
        return _distanceEast;
    case Course_SOUTH:
        return -_distanceNorth;
    case Course_WEST:
    default:
        return -_distanceEast;
    }
}

void AbstractDuck::fly()
{
    _flyStrategy->fly(_course, _distanceEast, _distanceNorth);
}

void AbstractDuck::quack()
{
    _quackStrategy->quack();
}

void AbstractDuck::right()
{
    _rotateStrategy->right(_course);
}

void AbstractDuck::left()
{
    _rotateStrategy->left(_course);
}

#include "RedheadDuck.hpp"

#include "../strategy/FlyLikeDuck.hpp"
#include "../strategy/QuackLikeDuck.hpp"
#include "../strategy/RotateWrongWay.hpp"

RedheadDuck::RedheadDuck()
    :AbstractDuck(new FlyLikeDuck(), new QuackLikeDuck(2), new RotateWrongWay())
{
}

RedheadDuck::~RedheadDuck()
{
}

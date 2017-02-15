#include "MallardDuck.hpp"

#include "../strategy/FlyLikeDuck.hpp"
#include "../strategy/QuackLikeDuck.hpp"
#include "../strategy/Rotate.hpp"

MallardDuck::MallardDuck()
    :AbstractDuck(new FlyLikeDuck(), new QuackLikeDuck(3), new Rotate())
{
}

MallardDuck::~MallardDuck()
{
}

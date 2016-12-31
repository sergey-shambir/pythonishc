#include "ToyDuck.hpp"

#include "../strategy/FlyInBathroom.hpp"
#include "../strategy/QuackLikeDuck.hpp"
#include "../strategy/Rotate.hpp"

ToyDuck::ToyDuck()
    :AbstractDuck(new FlyInBathroom(), new QuackLikeDuck(1), new Rotate())
{
}

ToyDuck::~ToyDuck()
{
}

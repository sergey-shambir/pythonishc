#ifndef REDHEADDUCK_HPP_05cd8e29_1ced_491d_b768_378277442a32
#define REDHEADDUCK_HPP_05cd8e29_1ced_491d_b768_378277442a32

#include "AbstractDuck.hpp"

/**
  \class RedheadDuck implements interface IDuck
  This duck quacks twice and always confuses right and left wings
  */

class RedheadDuck : public AbstractDuck
{
public:
    RedheadDuck();
    virtual ~RedheadDuck();
};

#endif // REDHEADDUCK_HPP_05cd8e29_1ced_491d_b768_378277442a32

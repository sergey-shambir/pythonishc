#ifndef TOYDUCK_HPP_7e0ef61e_9515_4132_a33a_449b5eef4d00
#define TOYDUCK_HPP_7e0ef61e_9515_4132_a33a_449b5eef4d00

#include "AbstractDuck.hpp"

/**
  \class ToyDuck implements interface IDuck
  This duck quacks once but never flies
  */

class ToyDuck : public AbstractDuck
{
public:
    ToyDuck();
    virtual ~ToyDuck();
};

#endif // TOYDUCK_HPP_7e0ef61e_9515_4132_a33a_449b5eef4d00

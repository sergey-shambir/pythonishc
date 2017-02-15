#ifndef QUACKLIKEDUCK_HPP_a09e6423_102f_4c36_a8ee_6abcdbbc5ba7
#define QUACKLIKEDUCK_HPP_a09e6423_102f_4c36_a8ee_6abcdbbc5ba7

#include "IQuack.hpp"

class QuackLikeDuck : public IQuack
{
public:
    explicit QuackLikeDuck(unsigned timesToQuack);
    virtual ~QuackLikeDuck();

    /**
      quacks a few times
      */
    virtual void quack() const;

private:
    unsigned _timesToQuack;
};

#endif // QUACKLIKEDUCK_HPP_a09e6423_102f_4c36_a8ee_6abcdbbc5ba7

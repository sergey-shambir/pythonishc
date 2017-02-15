#ifndef ABSTRACTDUCK_HPP_85583900_489b_4052_b3f5_0ed2afb280f5
#define ABSTRACTDUCK_HPP_85583900_489b_4052_b3f5_0ed2afb280f5

#include <memory>

#include "IDuck.hpp"
#include "../strategy/IFly.hpp"
#include "../strategy/IQuack.hpp"
#include "../strategy/IRotate.hpp"

/**
  \class AbstractDuck implements interface IDuck
  Provides base duck implementation for child classes
  */

class AbstractDuck : public IDuck
{
public:
    virtual Course getCourse() const;
    virtual int getDistance(Course course) const;
    virtual void fly();
    virtual void quack();
    virtual void right();

protected:
    AbstractDuck(const IFly* flyStrategy,
                 const IQuack* quackStrategy,
                 const IRotate* rotateStrategy,
                 Course course = Course_NORTH);
    virtual ~AbstractDuck();

private:
    std::unique_ptr<const IFly> _flyStrategy;
    std::unique_ptr<const IQuack> _quackStrategy;
    std::unique_ptr<const IRotate> _rotateStrategy;

    Course _course;
    int _distanceEast;
    int _distanceNorth;
};

#endif // ABSTRACTDUCK_HPP_85583900_489b_4052_b3f5_0ed2afb280f5

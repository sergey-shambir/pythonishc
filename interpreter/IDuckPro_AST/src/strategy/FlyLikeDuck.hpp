#ifndef FLYLIKEDUCK_HPP_f01d5b76_5678_48b2_ad80_003833219977
#define FLYLIKEDUCK_HPP_f01d5b76_5678_48b2_ad80_003833219977

#include "IFly.hpp"

class FlyLikeDuck : public IFly
{
public:
    FlyLikeDuck();
    virtual ~FlyLikeDuck();

    /**
      implements flight in given course, changes distance from starting point
      */
    virtual void fly(Course course, int &distEast, int &distNorth) const;
};

#endif // FLYLIKEDUCK_HPP_f01d5b76_5678_48b2_ad80_003833219977

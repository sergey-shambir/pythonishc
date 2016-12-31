#ifndef FLYINBATHROOM_HPP_5ad126ce_698a_41ba_a92f_15a78193fc05
#define FLYINBATHROOM_HPP_5ad126ce_698a_41ba_a92f_15a78193fc05

#include "IFly.hpp"

class FlyInBathroom : public IFly
{
public:
    FlyInBathroom();
    virtual ~FlyInBathroom();

    /**
      implements swimming in bathroom instead of real flight
      */
    virtual void fly(Course course, int &distEast, int &distNorth) const;
};

#endif // FLYINBATHROOM_HPP_5ad126ce_698a_41ba_a92f_15a78193fc05

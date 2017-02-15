#ifndef ROTATEWRONGWAY_HPP_95b17137_7d5b_4258_9193_dfd605c6dcd5
#define ROTATEWRONGWAY_HPP_95b17137_7d5b_4258_9193_dfd605c6dcd5

#include "Rotate.hpp"

class RotateWrongWay : public Rotate
{
public:
    RotateWrongWay();
    ~RotateWrongWay();

    /**
      given course rotates wrong way (anticlockwise when viewed from aerial)
      */
    virtual void right(Course& course) const;

    /**
      given course rotates wrong way (clockwise when viewed from aerial)
      */
    virtual void left(Course &course) const;
};

#endif // ROTATEWRONGWAY_HPP_95b17137_7d5b_4258_9193_dfd605c6dcd5

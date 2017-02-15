#ifndef IRIGHT_HPP_2d5cb031_f3c3_426c_ba1d_7aeb939d4315
#define IRIGHT_HPP_2d5cb031_f3c3_426c_ba1d_7aeb939d4315

#include "IFly.hpp"

class IRotate
{
public:
    /**
      given course rotates clockwise (when viewed from aerial)
      */
    virtual void right(Course& course) const = 0;

    /**
      given course rotates anticlockwise (when viewed from aerial)
      */
    virtual void left(Course &course) const = 0;
};

#endif // IRIGHT_HPP_2d5cb031_f3c3_426c_ba1d_7aeb939d4315

#ifndef ROTATE_HPP_8a4e8e61_9df2_4ca9_87e0_f69537938cda
#define ROTATE_HPP_8a4e8e61_9df2_4ca9_87e0_f69537938cda

#include "IRotate.hpp"

class Rotate : public IRotate
{
public:
    Rotate();
    ~Rotate();

    /**
      given course rotates clockwise (when viewed from aerial)
      */
    virtual void right(Course& course) const;

    /**
      given course rotates anticlockwise (when viewed from aerial)
      */
    virtual void left(Course &course) const;
};

#endif // ROTATE_HPP_8a4e8e61_9df2_4ca9_87e0_f69537938cda

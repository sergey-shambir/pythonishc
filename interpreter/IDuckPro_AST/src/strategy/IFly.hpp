#ifndef IFLY_HPP_00524be7_6525_4766_80b1_be1e4c2a0ae5
#define IFLY_HPP_00524be7_6525_4766_80b1_be1e4c2a0ae5

enum Course {
    Course_NORTH,
    Course_EAST,
    Course_SOUTH,
    Course_WEST
};

class IFly
{
public:
    /**
      implements flight in given course, changes distance from starting point
      */
    virtual void fly(Course course, int &distEast, int& distNorth) const = 0;

    virtual ~IFly() { }
};

template<class TString>
TString toString(Course course)
{
    switch (course)
    {
    case Course_WEST:
        return TString("west");
    case Course_NORTH:
        return TString("north");
    case Course_EAST:
        return TString("east");
    default:
        return TString("south");
    }
}

#endif // IFLY_HPP_00524be7_6525_4766_80b1_be1e4c2a0ae5

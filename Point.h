#ifndef POINT_H
#define POINT_H

#include <iostream>

class Point
{
    public:
        Point();
        virtual ~Point();

        virtual double getX() const = 0;
        virtual double getY() const = 0;
        virtual double getZ() const = 0;
        virtual double getR() const = 0;
        virtual double getTheta() const = 0;
        virtual double getPhi() const = 0;

        void print() const;

    private:
};

inline
void Point::print() const
{
    std::cout << "[x,y,z] = [" << getX() << "," << getY() << "," << getZ() << "]" << std::endl;
    std::cout << "[r,theta,phi] = [" << getR() << "," << getTheta() << "," << getPhi() << "]" << std::endl;
    return;
}


#endif // POINT_H

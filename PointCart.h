#ifndef POINTCART_H
#define POINTCART_H

#include <Point.h>
#include <Constants.h>
#include <cmath>

class PointCart : public Point
{
    public:
        PointCart(const double& x = 0.0, const double& y = 0.0, const double& z = 0.0);
        PointCart(const Point& p);
        virtual ~PointCart();

        PointCart operator=(const Point& p);
        bool operator==(const Point& p);
        PointCart& operator+=(const Point& p);
        PointCart& operator-=(const Point& p);

        double getX() const { return m_x; }
        void setX(const double& val) { m_x = val; }
        double getY() const { return m_y; }
        void setY(const double& val) { m_y = val; }
        double getZ() const { return m_z; }
        void setZ(const double& val) { m_z = val; }

        double getR() const;
        double getTheta() const;
        double getPhi() const;

    private:
        double m_x;
        double m_y;
        double m_z;
};

PointCart operator+(const PointCart& a, const Point& b);
PointCart operator-(const PointCart& a, const Point& b);

#endif // POINTCART_H

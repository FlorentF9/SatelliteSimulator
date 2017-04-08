#ifndef POINTPOL_H
#define POINTPOL_H

#include <Point.h>
#include <PointCart.h>
#include <Constants.h>
#include <cmath>

class PointPol : public Point
{
    public:
        PointPol(double r = 0.0, double theta = 0.0, double phi = 0.0);
        PointPol(const Point& p);
        virtual ~PointPol();

        PointPol operator=(const Point& p);
        bool operator==(const Point& p);
        PointPol& operator+=(const Point& p);
        PointPol& operator-=(const Point& p);

        double getR() const { return m_r; }
        void setR(const double& val) { m_r = val; }
        double getTheta() const { return m_theta; }
        void setTheta(const double& val) { m_theta = val; }
        double getPhi() const { return m_phi; }
        void setPhi(const double& val) { m_phi = val; }

        double getX() const;
        double getY() const;
        double getZ() const;

    private:
        double m_r;
        double m_theta;
        double m_phi;
};

PointPol operator+(const PointPol& a, const Point& b);
PointPol operator-(const PointPol& a, const Point& b);

#endif // POINTPOL_H

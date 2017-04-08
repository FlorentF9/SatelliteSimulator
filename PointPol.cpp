#include <PointPol.h>

PointPol::PointPol(double r, double theta, double phi)
{
    m_r = r;
    m_theta = theta;
    m_phi = phi;
}

PointPol::PointPol(const Point& p)
{
    m_r = p.getR();
    m_theta = p.getTheta();
    m_phi = p.getPhi();
}


PointPol::~PointPol()
{

}

PointPol PointPol::operator=(const Point& p)
{
    m_r = p.getR();
    m_theta = p.getTheta();
    m_phi = p.getPhi();

    return *this;
}

PointPol& PointPol::operator+=(const Point& p)
{
    // Create cartesian copy of point to use adding operator
    PointCart tmp(*this);
    tmp+=p;
    // Reconvert to polar point
    PointPol copy(tmp);
    *this = copy;

    return *this;
}

PointPol& PointPol::operator-=(const Point& p)
{
    // Create cartesian copy of point to use substraction operator
    PointCart tmp(*this);
    tmp-=p;
    // Reconvert to polar point
    PointPol copy(tmp);
    *this = copy;

    return *this;
}

PointPol operator+(const PointPol& a, const Point& b)
{
    PointPol copy(a);
    copy+=b;

    return copy;
}

PointPol operator-(const PointPol& a, const Point& b)
{
    PointPol copy(a);
    copy-=b;

    return copy;
}

double PointPol::getX() const
{
    return m_r * std::cos(m_theta) * std::cos(m_phi);
}

double PointPol::getY() const
{
    return m_r * std::sin(m_theta) * std::cos(m_phi);
}

double PointPol::getZ() const
{
    return m_r * std::sin(m_phi);
}

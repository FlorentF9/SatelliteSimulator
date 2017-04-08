#include <PointCart.h>

PointCart::PointCart(const double& x, const double& y, const double& z)
{
    m_x = x;
    m_y = y;
    m_z = z;
}

PointCart::PointCart(const Point& p)
{
    m_x = p.getX();
    m_y = p.getY();
    m_z = p.getZ();
}

PointCart::~PointCart()
{

}

PointCart PointCart::operator=(const Point& p)
{
    m_x = p.getX();
    m_y = p.getY();
    m_z = p.getZ();

    return *this;
}

PointCart& PointCart::operator+=(const Point& p)
{
    m_x += p.getX();
    m_y += p.getY();
    m_z += p.getZ();

    return *this;
}

PointCart& PointCart::operator-=(const Point& p)
{
    m_x -= p.getX();
    m_y -= p.getY();
    m_z -= p.getZ();

    return *this;
}

PointCart operator+(const PointCart& a, const Point& b)
{
    PointCart copy(a);
    copy+=b;

    return copy;
}

PointCart operator-(const PointCart& a, const Point& b)
{
    PointCart copy(a);
    copy-=b;

    return copy;
}

double PointCart::getR() const
{
    return std::sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
}

double PointCart::getTheta() const
{
    if (m_x > 0.0 && m_y >= 0.0)
        return std::atan(m_y/m_x);
    if (m_x == 0.0 && m_y > 0.0)
        return Constants::pi / 2.0;
    if (m_x < 0.0)
        return Constants::pi + std::atan(m_y/m_x);
    if (m_x == 0.0 && m_y < 0.0)
        return 3.0 * Constants::pi / 2.0;
    if (m_x > 0.0 && m_y < 0.0)
        return Constants::twopi + std::atan(m_y/m_x);

    return 0.0;
}

double PointCart::getPhi() const
{
    // TODO

    return 0.0;
}

#include "Planet.h"

Planet::Planet(double mu, double radius, double day, const std::string& name, const std::string &imgPath)
{
    m_mu      = mu;
    m_radius  = radius;
    m_name    = name;
    m_imgPath = imgPath;
    m_day     = day;
}

Planet::~Planet()
{

}

const std::string Planet::toString(void) const
{
    std::stringstream output;
    output << "Name: " << m_name << std::endl;
    output << "Radius: " << m_radius << std::endl;
    output << "Mu: " << m_mu << std::endl;
    output << "Day: " << m_day << std::endl;
    output << "ImgPath: " << m_imgPath << std::endl;

    return output.str();
}

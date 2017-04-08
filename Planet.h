#ifndef PLANET_H
#define PLANET_H

#include <Constants.h>
#include <string>
#include <sstream>
#include <cmath>

class Planet
{
public:
    Planet(double mu = Constants::mu_earth, double radius = Constants::r_earth, double day = Constants::day_earth, const std::string& name = Constants::defaultPlanetName, const std::string& imgPath = Constants::defaultImgPath);
    ~Planet();

    void update(double dt);

    double getMu(void) const {return m_mu;}
    double getRadius(void) const {return m_radius;}
    double getDay(void) const {return m_day;}
    const std::string& getName(void) const {return m_name;}
    const std::string& getImgPath(void) const {return m_imgPath;}
    void setMu(double mu) {m_mu = mu;}
    void setRadius(double radius) {m_radius = radius;}
    void setDay(double day) {m_day = day;}
    void setName(const std::string& name) {m_name = name;}
    void setImgPath(const std::string& path) {m_imgPath = path;}

    double a_geo(void) const {return std::pow(m_mu*m_day*m_day/(4.0*Constants::pi2) ,1.0/3.0);}

    const std::string toString(void) const;

private:
    double m_mu;
    double m_radius;
    std::string m_name;
    std::string m_imgPath;
    double m_day;
};

#endif // PLANET_H

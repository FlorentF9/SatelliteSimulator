#ifndef SATELLITE_H
#define SATELLITE_H

#include <Orbit.h>
#include <Planet.h>
#include <Propulsion.h>
#include <sstream>

class Satellite
{
public:
    Satellite(const Orbit& orb, const Planet* planet, const Propulsion& prop, const std::string& name = "Satellite",
              double rx = 0.0, double ry = 0.0, double rz = 0.0);
    ~Satellite();

    Orbit*             getOrbit(void) const {return m_orbit;}
    const Planet*      getPlanet(void) const {return m_planet;}
    const Propulsion&  getPropu(void) const {return m_prop;}
    const std::string& getName(void) const {return m_name;}
    const double&      getRx(void) const {return m_rx;}
    const double&      getRy(void) const {return m_ry;}
    const double&      getRz(void) const {return m_rz;}


    void setName(const std::string& name) {m_name = name;}
    void setRx(const double& rx) {m_rx = rx;}
    void setRy(const double& ry) {m_ry = ry;}
    void setRz(const double& rz) {m_rz = rz;}

    void  update(double dt);
    void  reset(void);
    PointPol getCurrentPosition(void) const {return m_orbit->getPositionPoint();}

    const std::string toString(void) const;

private:
    Orbit*        m_orbit;
    const Planet* m_planet;
    Propulsion    m_prop;
    std::string   m_name;
    double        m_rx;
    double        m_ry;
    double        m_rz;
};

#endif // SATELLITE_H

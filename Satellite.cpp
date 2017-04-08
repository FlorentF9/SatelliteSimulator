#include <Satellite.h>

Satellite::Satellite(const Orbit &orb, const Planet* planet, const Propulsion &prop, const std::string &name, double rx, double ry, double rz)
{
    m_orbit  = new Orbit(orb);
    m_planet = planet;
    m_prop   = prop;
    m_name   = name;
    m_rx     = rx;
    m_ry     = ry;
    m_rz     = rz;
}

Satellite::~Satellite()
{

}

void Satellite::update(double dt)
{
    // Update orbit
    m_orbit->update(dt);

    // Update satellite position on its orbit
    m_orbit->updatePosition(dt);

    return;
}

void Satellite::reset()
{
    // Reset position to 0
    m_orbit->reset();
    // Reset other parameters
    //m_prop->reset();
}

const std::string Satellite::toString(void) const
{
    std::stringstream output;
    output << "----------" << std::endl;
    output << "Name: " << m_name << std::endl;
    //output << "Propulsion: " << m_prop.toString() << std::endl;
    output << m_orbit->toString();

    return output.str();
}

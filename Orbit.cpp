#include <Orbit.h>

Orbit::Orbit(const Planet* planet, double a, double e, double i, double Omega, double omega, double tp)
{
    m_planet = planet;
    m_mu     = planet->getMu();
    m_a      = a;
    m_e      = e;
    m_i      = std::fmod(i, Constants::twopi);
    if (m_i < 0.0)
        m_i += Constants::twopi;
    m_Omega  = std::fmod(Omega, Constants::twopi);
    if (m_Omega < 0.0)
        m_Omega += Constants::twopi;
    m_omega  = std::fmod(omega, Constants::twopi);
    if (m_omega < 0.0)
        m_omega += Constants::twopi;
    m_tp     = tp;
    // Reset position to initial state
    reset();
}

 Orbit::Orbit(const Orbit& orbit)
 {
    m_planet = orbit.getPlanet();
    m_mu     = m_planet->getMu();
    m_a      = orbit.getA();
    m_e      = orbit.gete();
    m_i      = orbit.getI();
    m_Omega  = orbit.getOmega();
    m_omega  = orbit.getomega();
    m_tp     = orbit.getTp();
    m_v      = orbit.getV();
    m_E      = orbit.getE();
    m_M      = orbit.getM();
 }

Orbit::~Orbit()
{

}

void Orbit::updatePosition(double dt)
{
    m_M += getN() * dt;
    m_M  = std::fmod(m_M, Constants::twopi);
    if (m_M < 0.0)
        m_M += Constants::twopi;

    // Compute E with dichotomy since E - e*sin(E) is crescent
    const double eps = 1.0e-6;
    double min = 0.0;
    double max = Constants::twopi;
    while((max-min) > eps)
    {
        double mid = 0.5*(max+min);
        if (m_M < mid - m_e*std::sin(mid))
            max = mid;
        else
            min = mid;
    }
    m_E = min;

    // Compute v
    if(m_E <= Constants::pi)
        m_v = std::acos((std::cos(m_E) - m_e) / (1.0 - m_e * std::cos(m_E)));
    else
        m_v = Constants::twopi - std::acos((std::cos(m_E) - m_e) / (1.0 - m_e * std::cos(m_E)));

    /*
    // Compute dM
    double dM = getN() * dt;

    // Compute dE
    double dE = dM / (1.0 - m_e*std::cos(m_E));

    // Update v
    double dv = std::pow((1.0 + m_e*std::cos(m_v)),2.0)/(1.0 - m_e*m_e)
                 * std::sin(m_E) * dE;

    // Update M
    m_M += dM;
    m_M = std::fmod(m_M, Constants::twopi);

    // Update E
    m_E += dE;
    m_E = std::fmod(m_E, Constants::twopi);

    // Update v
    m_v += dv;
    m_v = std::fmod(m_v, Constants::twopi);
    */

    return;
}

void Orbit::update(double dt)
{
    // Update orbit (if perturbation or maneuvers)

    return;
}

void Orbit::setM(double M)
{
    // Set M
    m_M      = M;
    m_M      = std::fmod(m_M, Constants::twopi);
    if (m_M < 0.0)
        m_M += Constants::twopi;

    // Set E and v

    // Compute E with dichotomy since E - e*sin(E) is crescent
    const double eps = 1.0e-6;
    double min = 0.0;
    double max = Constants::twopi;
    while((max-min) > eps)
    {
        double mid = 0.5*(max+min);
        if (m_M < mid - m_e*std::sin(mid))
            max = mid;
        else
            min = mid;
    }
    m_E = min;

    // Compute v
    if(m_E <= Constants::pi)
        m_v = std::acos((std::cos(m_E) - m_e) / (1.0 - m_e * std::cos(m_E)));
    else
        m_v = Constants::twopi - std::acos((std::cos(m_E) - m_e) / (1.0 - m_e * std::cos(m_E)));

    return;
}

void Orbit::reset(void)
{
    //Set M to initial angle
    setM(-getN()*m_tp);

    return;
}

PointPol Orbit::getPositionPoint() const
{
    double r     = m_a * (1.0 - m_e * std::cos(m_E));
    /*
    double theta = fmod(m_Omega + std::cos(m_i) * (m_omega + m_v), Constants::twopi);
    if (theta < 0.0)
        theta += Constants::twopi;
    */
    //double theta = fmod(m_Omega+std::atan(std::tan(m_omega+m_v)*std::cos(m_i)), Constants::twopi);
    double theta = fmod(m_Omega+std::atan2(
                            std::sin(m_omega+m_v)*std::cos(m_i)/std::sqrt(1.0-std::pow(std::sin(m_omega+m_v)*std::sin(m_i), 2.0)),
                            std::cos(m_omega+m_v)/std::sqrt(1.0-std::pow(std::sin(m_omega+m_v)*std::sin(m_i), 2.0))
                            ), Constants::twopi);
    if (theta < 0.0)
        theta += Constants::twopi;

    double phi   = fmod(std::asin(std::sin(m_i)*std::sin(m_omega+m_v)), Constants::twopi);
    if (phi < 0.0)
        phi += Constants::twopi;

    return PointPol(r, theta, phi);
}

PointPol Orbit::getPointAt(double M) const
{
    M      = std::fmod(M, Constants::twopi);
    if (M < 0.0)
        M += Constants::twopi;

    // Compute E with dichotomy since E - e*sin(E) is crescent
    const double eps = 1.0e-6;
    double min = 0.0;
    double max = Constants::twopi;
    while((max-min) > eps)
    {
        double mid = 0.5*(max+min);
        if (M < mid - m_e*std::sin(mid))
            max = mid;
        else
            min = mid;
    }
    double E = min;

    // Compute v
    double v = 0.0;
    if(E <= Constants::pi)
        v = std::acos((std::cos(E) - m_e) / (1.0 - m_e * std::cos(E)));
    else
        v = Constants::twopi - std::acos((std::cos(E) - m_e) / (1.0 - m_e * std::cos(E)));

    // Compute point
    double r     = m_a * (1.0 - m_e * std::cos(E));

    double theta = fmod(m_Omega+std::atan2(
                            std::sin(m_omega+v)*std::cos(m_i)/std::sqrt(1.0-std::pow(std::sin(m_omega+v)*std::sin(m_i), 2.0)),
                            std::cos(m_omega+v)/std::sqrt(1.0-std::pow(std::sin(m_omega+v)*std::sin(m_i), 2.0))
                            ), Constants::twopi);
    if (theta < 0.0)
        theta += Constants::twopi;

    double phi   = fmod(std::asin(std::sin(m_i)*std::sin(m_omega+v)), Constants::twopi);
    if (phi < 0.0)
        phi += Constants::twopi;

    return PointPol(r, theta, phi);
}

const std::string Orbit::toString(void) const
{
    std::stringstream output;

    output << "a: " << m_a << std::endl;
    output << "e: " << m_e << std::endl;
    output << "i: " << m_i << std::endl;
    output << "Omega: " << m_Omega << std::endl;
    output << "omega: " << m_omega << std::endl;
    output << "tp: " << m_tp << std::endl;
    output << "M: " << m_M << std::endl;

    return output.str();
}

void Orbit::print() const
{
    std::cout << "***Orbit members***" << std::endl;
    std::cout << "a  = " << m_a << " km" << std::endl;
    std::cout << "e  = " << m_e << std::endl;
    std::cout << "i  = " << m_i << " rad" << std::endl;
    std::cout << "Om = " << m_Omega << " rad" << std::endl;
    std::cout << "om = " << m_omega << " rad" << std::endl;
    std::cout << "tp = " << m_tp << " s" << std::endl;
    std::cout << "n  = " << getN() << " rad/s" << std::endl;

    std::cout << "***Other info***" << std::endl;
    std::cout << "rp = " << m_a*(1.0-m_e) << " km" << std::endl;
    std::cout << "ra = " << m_a*(1.0+m_e) << " km" << std::endl;
    std::cout << "T  = " << Constants::twopi/getN() << " s" << std::endl;

    std::cout << "***Satellite motion***" << std::endl;
    std::cout << "v  = " << m_v << " rad" << std::endl;
    std::cout << "E  = " << m_E << " rad" << std::endl;
    std::cout << "M  = " << m_M << " rad" << std::endl;

    return;
}

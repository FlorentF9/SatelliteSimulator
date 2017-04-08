#include <Simulation.h>

Simulation::Simulation(Planet* planet, const std::string& name, double speed, double dt)
{
    m_planet     = planet;
    m_name       = name;
    m_satellites = new std::vector<Satellite>();
    sim_t        = 0.0;
    sim_dt       = dt;
    sim_speed    = speed;
    m_play       = Constants::autoPlay;
    m_verbose    = Constants::verbose;
    m_writeLog   = Constants::writeLog;
}

Simulation::~Simulation()
{
    delete m_satellites;
}

void Simulation::update(void)
{
    if(m_play)
    {
        // Add time interval to sim time
        sim_t += dt();

        //Print info
        if(m_verbose)
        {
            std::cout << "t = " << t() << std::endl;
            for(unsigned int i=0; i<m_satellites->size(); ++i)
            {
                std::cout << m_satellites->at(i).getName() << std::endl;
                std::cout << "v = " << m_satellites->at(i).getOrbit()->getV();
                std::cout << " / E = " << m_satellites->at(i).getOrbit()->getE();
                std::cout << " / M = " << m_satellites->at(i).getOrbit()->getM() << std::endl;
                m_satellites->at(i).getCurrentPosition().print();
            }
        }

        // Update each satellite's orbit and position
        for(unsigned int i=0; i<m_satellites->size(); ++i)
        {
            m_satellites->at(i).update(dt());
        }
    }

    return;
}

void Simulation::addSatellite(const Satellite &sat)
{
    if(sat.getPlanet() == m_planet)
        m_satellites->push_back(sat);

    return;
}

void Simulation::reset(void)
{
    //Reset time to 0
    sim_t = 0.0;

    // Reset each satellite's position to initial state
    for(unsigned int i=0; i<m_satellites->size(); ++i)
    {
        m_satellites->at(i).reset();
    }

    return;
}

void Simulation::resetAll(void)
{
    //Reset time to 0
    sim_t = 0.0;

    //Remove all satellites
    m_satellites->clear();

    return;
}

int Simulation::saveToFile(const std::string &path, const std::string &date) const
{
    std::ofstream file(path.c_str());

    if(file)
    {
        file << Constants::programName << std::endl;
        file << date << std::endl;
        file << path << std::endl;
        file << "----------" << std::endl;
        file << "Simulation" << std::endl;
        file << toString();
        file << "----------" << std::endl;
        file << "Planet" << std::endl;
        file << m_planet->toString();
        file << "----------" << std::endl;
        file << "Satellites" << std::endl;
        for(unsigned int i=0; i<m_satellites->size(); ++i)
        {
            file << m_satellites->at(i).toString();
        }

        return 0;
    }

    return 1;
}

const std::string Simulation::toString(void) const
{
    std::stringstream output;
    output << "Name: " << m_name << std::endl;
    output << "t: " << sim_t << std::endl;
    output << "dt: " << sim_dt << std::endl;
    output << "Speed: " << sim_speed << std::endl;
    output << "n: " << m_satellites->size() << std::endl;

    return output.str();
}

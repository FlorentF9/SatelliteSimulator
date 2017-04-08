#ifndef SIMULATION_H
#define SIMULATION_H

#include <vector>
#include <fstream>
#include <sstream>
#include <Satellite.h>

class Simulation
{
public:
    Simulation(Planet *planet, const std::string& name, double speed = 1.0, double dt = 1.0);
    ~Simulation();

    void addSatellite(const Satellite& sat);
    void remSatellite(int i) {m_satellites->erase(m_satellites->begin()+i);}
    Satellite* sat(int i) {return &(m_satellites->at(i));}

    unsigned int nsat(void) const {return m_satellites->size();}

    void update(void);
    double t(void) const {return sim_t;}
    void setT(double t) {sim_t = t;}
    void reset(void);
    void resetAll(void);
    double dt(void) const {return sim_dt;}
    void setDt(double dt) {sim_dt = dt;}
    double speed(void) const {return sim_speed;}
    void setSpeed(double speed = 1.0) {sim_speed = speed;}

    const std::string& name(void) {return m_name;}
    void setWriteLog(bool b) {m_writeLog = b;}
    bool writeLog(void) {return m_writeLog;}

    bool play(void) const {return m_play;}
    void setPlay(bool b) {m_play = b;}
    void togglePlay(void) {m_play = !m_play;}
    void toggleVerbose(void) {m_verbose = !m_verbose;}

    Planet* getPlanet(void) {return m_planet;}
    void    setName(const std::string& name) {m_name = name;}

    const std::string toString(void) const;
    int saveToFile(const std::string& path, const std::string &date) const;

private:
    Planet* m_planet;
    std::string m_name;
    std::vector<Satellite>* m_satellites;
    double sim_t;
    double sim_dt;
    double sim_speed;
    bool   m_play;
    bool   m_verbose;
    bool   m_writeLog;
};

#endif // SIMULATION_H

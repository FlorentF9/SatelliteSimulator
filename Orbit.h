#ifndef ORBIT_H
#define ORBIT_H

#include <Constants.h>
#include <PointPol.h>
#include <Planet.h>
#include <sstream>

class Orbit
{
    public:
        Orbit(const Orbit& orbit);
        Orbit(const Planet* planet, double a, double e, double i, double Omega = 0.0, double omega = 0.0, double tp = 0.0);
        virtual ~Orbit();

        void     update(double dt);
        void     updatePosition(double dt);
        PointPol getPositionPoint() const;
        PointPol getPointAt(double M) const;
        void     print() const;
        void     updateSim(double dt);
        void     reset(void);

        double getA(void) const     {return m_a;}
        void   setA(double a)       {m_a = a;}
        double gete(void) const     {return m_e;}
        void   sete(double e)       {m_e = e;}
        double getI(void) const     {return m_i;}
        void   setI(double i)       {m_i = i;}
        double getOmega(void) const {return m_Omega;}
        void   setOmega(double Om)  {m_Omega = Om;}
        double getomega(void) const {return m_omega;}
        void   setomega(double om)  {m_omega = om;}
        double getN(void) const     {return std::sqrt(m_mu/std::pow(m_a, 3.0));}
        double getV(void) const     {return m_v;}
        double getE(void) const     {return m_E;}
        double getM(void) const     {return m_M;}
        void   setM(double M);
        double getTp(void) const    {return m_tp;}
        void   setTp(double tp)     {m_tp = tp;}
        const Planet* getPlanet(void) const {return m_planet;}

        double getRa(void) const {return m_a*(1.0+m_e);}
        double getRp(void) const {return m_a*(1.0-m_e);}

        const std::string toString(void) const;

    private:
        const Planet* m_planet;
        double m_a;
        double m_e;
        double m_i;
        double m_Omega; // longitude of ascending node
        double m_omega; // argument of periapsis

        // satellite motion
        double m_v;
        double m_E;
        double m_M;

        // Initial constants
        double m_tp;
        double m_mu;

};

#endif // ORBIT_H

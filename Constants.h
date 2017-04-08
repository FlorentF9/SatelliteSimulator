#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>

// Useful constants
namespace Constants
{
        // Numerical constants
        static const double pi     = 3.14159265358979323846;
        static const double twopi  = 2.0*3.14159265358979323846;
        static const double halfpi = 0.5*3.14159265358979323846;
        static const double pi2    = 3.14159265358979323846*3.14159265358979323846;

        // Physical constants
        static const double G = 6.67384e-11; // Gravitational constant (m^3/kg/s^2)

        // Earth constants
        static const double r_earth   = 6.3781366e3;  // Earth equatorial radius (km)
        static const double J2_earth  = 0.0010826359; // J2 factor
        static const double mu_earth  = 398600.4415;  // Earth mu (km^3/s^2)
        static const double day_earth = 86164.10;     // Duration of an earth rotation (s)
        static const double a_geo     = 42164.2;      // Geostationary orbit (km)

        // Program constants
        static const std::string programName = "Satellite Simulator beta 0.1";
        static const bool   autoPlay        = false; // Start simulation automatically?
        static const bool   verbose         = false; // Print position on stdout?
        static const bool   writeLog        = true;  // Keep log file?
        static const std::string defaultImgPath    = "textures/earth_4k.jpg"; // Default planet texture
        static const std::string defaultPlanetName = "Earth";
        static const double minPlanetRadius = 0.1;   // Minimal radius of a planet (km)
        static const double maxPlanetRadius = 1.0e6; // Maxmimum radius of a planet (km)
        static const double minPlanetMu     = 0.001; // Minimal mu of a planet (km^3/s^2)
        static const double maxPlanetMu     = 1.0e9; // Maximum mu of a planet (km^3/s^2)
        static const double minPlanetDay    = 1.0;   // Minimal duration of a sideral day (s)
        static const double maxPlanetDay    = 1.0e8; // Maximum duration of a sideral day (s)
        static const double minTimeStep     = 0.001; // Minimal simulation time step (s)
        static const double maxTimeStep     = 60.0;  // Maximum simulation time step (s)
        static const double maxSatA         = 1.0e6;
        static const double minSatTp        = 1.0e6;
        static const double maxSatTp        = 1.0e6;

}

#endif // CONSTANTS_H

#include <pigpio.h>
#include <iostream>
#include <unistd.h> 
#include <iomanip>
#include "../../lib/gps/Ada_GPS.h"

class GPS{
public:
    GPS(const std::string &serialPort);
    struct GpsData read();
private:
    Ada_GPS gps; 
    const std::string port;
};

struct GpsData {
    bool fix;
    nmea_float_t latitudeDegrees;  ///< Latitude in decimal degrees
    nmea_float_t longitudeDegrees; ///< Longitude in decimal degrees
    nmea_float_t geoidheight;      ///< Diff between geoid height and WGS84 height
    nmea_float_t altitude;         ///< Altitude in meters above MSL
    nmea_float_t speed;            ///< Current speed over ground in knots
    int fixquality;    ///< Fix quality (0, 1, 2 = Invalid, GPS, DGPS)
    int satellites;    ///< Number of satellites in use
};
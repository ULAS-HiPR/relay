#include "gps/GPS.h"
#include "altimeter/altimeter.h"
#include <unistd.h>
#include <iostream>
#include <pigpio.h>

struct sensorData{
    struct AltData altData;
    struct GpsData gpsData;
};


struct Sensors {
    GPS gps;
    Altimeter alt;

    Sensors(const std::string &gpsSerialPort, double sea_pressure) : gps(gpsSerialPort), alt(sea_pressure) {}
};

struct Sensors startUp(void);
struct sensorData getData(struct Sensors sens);
int main() {};


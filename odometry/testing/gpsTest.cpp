#include "gps/GPS.h"
#include "altimeter/altimeter.h"
#include <unistd.h>
#include <iostream>
#include <wiringPi.h> 

//g++ gpsTest.cpp gps/GPS.cpp ../lib/gps/Ada_GPS.cpp ../lib/gps/NMEA_build.cpp ../lib/gps/NMEA_data.cpp ../lib/gps/NMEA_parse.cpp -o gpsTest -lwiringPi -lrt -lpthread

int main() {
    wiringPiSetupGpio();
    GPS gps("/dev/ttyAMA10");
    
    while (true) {
        struct GpsData gpsData = gps.read();

        if (gpsData.fix) {
            std::cout << "Fix: Yes" << std::endl;
            std::cout << std::fixed << std::setprecision(6);
            std::cout << "Latitude: " << gpsData.latitudeDegrees << std::endl;
            std::cout << "Longitude: " << gpsData.longitudeDegrees << std::endl;
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "Geoid Height: " << gpsData.geoidheight << std::endl;
            std::cout << "AltitudeGPS: " << gpsData.altitude << std::endl;
            std::cout << "Speed: " << gpsData.speed << std::endl;
            std::cout << "Fix Quality: " << gpsData.fixquality << std::endl;
            std::cout << "Satellites: " << gpsData.satellites << std::endl;
            //rate limiting
            usleep(500000);
        } else {
          std::cout << "Fix: No" << std::endl;
          usleep(10000);
        }
        usleep(500000);
        std::cout<< "\n"; 
    }
    return 0;
}

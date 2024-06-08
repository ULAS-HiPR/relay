#include "gps/GPS.h"
#include "altimeter/altimeter.h"
#include <unistd.h>
#include <iostream>
#include <pigpio.h>

int main() {
    if (gpioInitialise() < 0) {
        printf("pigpio initialization failed");
        return 0;
    }

    Altimeter alt(1016.0);
    GPS gps("/dev/ttyS0");
    while (true) {
        struct AltData altData = alt.read();
        struct GpsData gpsData = gps.read();
        std::cout << "Temperature: " << altData.temp << " C\n";
        std::cout << "Pressure: " << altData.pressure << " Pa\n";
        std::cout << "Altitude: " << altData.alt << " m\n";

        if (gpsData.fix) {
            std::cout << "Fix: Yes" << std::endl;
            std::cout << "Latitude: " << gpsData.latitudeDegrees << std::endl;
            std::cout << "Longitude: " << gpsData.longitudeDegrees << std::endl;
            std::cout << "Geoid Height: " << gpsData.geoidheight << std::endl;
            std::cout << "AltitudeGPS: " << gpsData.altitude << std::endl;
            std::cout << "Speed: " << gpsData.speed << std::endl;
            std::cout << "Fix Quality: " << gpsData.fixquality << std::endl;
            std::cout << "Satellites: " << gpsData.satellites << std::endl;
            usleep(500000);
        } else {
          std::cout << "Fix: No" << std::endl;
          usleep(50000);
        }
        std::cout<< u8"\033[2J\033[1;1H"; 
    }
    return 0;
}

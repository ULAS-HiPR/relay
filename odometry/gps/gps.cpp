#include "GPS.h"
#include <pigpio.h>
#include <iostream>
#include <unistd.h> 
#include <iomanip>

GPS gps("/dev/ttyS0"); // Assuming the GPS is connected to the default serial port on Raspberry Pi (GPIO14, GPIO15)

uint32_t timer = 0;

inline uint32_t millis() {
    return gpioTick() / 1000;
}

void setup() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed" << std::endl;
        return;
    }

    if (!gps.begin(9600)) {
        std::cerr << "Failed to initialize GPS" << std::endl;
        return;
    }

    std::cout << "GPS initialized successfully" << std::endl;
    
    // Ask for firmware version
    gps.sendCommand(PMTK_Q_RELEASE);

    timer = millis(); // Initialize the timer
}

void loop() {
    // read data from the GPS
    char c = gps.read();
    
    // if a sentence is received, parse it
    if (gps.newNMEAreceived()) {
        if (!gps.parse(gps.lastNMEA())) {
            std::cerr << "Failed to parse GPS data" << std::endl;
            return;
        }
    }

    // Print out the current stats approximately every 2 seconds
    if (millis() - timer > 2000) {
        timer = millis(); // reset the timer
        std::cout << "\nTime: ";
        if (gps.hour < 10) { std::cout << '0'; }
        std::cout << gps.hour << ':';
        if (gps.minute < 10) { std::cout << '0'; }
        std::cout << gps.minute << ':';
        if (gps.seconds < 10) { std::cout << '0'; }
        std::cout << gps.seconds << '.' << std::setw(3) << std::setfill('0') << gps.milliseconds << std::endl;
        std::cout << "Date: " << gps.day << '/' << gps.month << "/20" << gps.year << std::endl;
        std::cout << "Fix: " << static_cast<int>(gps.fix) << " quality: " << static_cast<int>(gps.fixquality) << std::endl;
        if (gps.fix) {
            std::cout << "Location: " << std::fixed << std::setprecision(4) << gps.latitudeDegrees << gps.lat << ", " << gps.longitudeDegrees << gps.lon << std::endl;
            std::cout << "Speed (knots): " << gps.speed << std::endl;
            std::cout << "Angle: " << gps.angle << std::endl;
            std::cout << "Altitude: " << gps.altitude << std::endl;
            std::cout << "Satellites: " << static_cast<int>(gps.satellites) << std::endl;
            std::cout << "Antenna status: " << static_cast<int>(gps.antenna) << std::endl;
        }
    }
    
    usleep(500); // Sleep for a short duration to reduce CPU usage
}

int main() {
    setup();
    while (true) {
        loop();
    }
    return 0;
}

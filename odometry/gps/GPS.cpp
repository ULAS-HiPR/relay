#include "GPS.h"
#include <pigpio.h>
#include <iostream>
#include <unistd.h> 
#include <iomanip>
#include <fstream>

GPS::GPS(const std::string &serialPort) : gps(serialPort), port(serialPort) {
    if (!gps.begin(9600)) {
        std::cerr << "Failed to initialize GPS" << std::endl;
        return;
    }
    std::cout << "GPS initialized successfully" << std::endl;
    gps.sendCommand(PMTK_Q_RELEASE);
}

struct GpsData GPS::read() {
    struct GpsData gpsData;
    char c = gps.read();
    
    // if a sentence is received, parse it
    if (gps.newNMEAreceived()) {
        if (!gps.parse(gps.lastNMEA())) {
            std::cerr << "Failed to parse GPS data" << std::endl;
            return gpsData;
        }
    }

    if (gps.fix){
    gpsData.fix = true;
    gpsData.latitudeDegrees = gps.latitudeDegrees;
    gpsData.longitudeDegrees = gps.longitudeDegrees;
    gpsData.geoidheight = gps.geoidheight;
    gpsData.altitude = gps.altitude;
    gpsData.speed = gps.speed;
    gpsData.fixquality = static_cast<int>(gps.fixquality);
    gpsData.satellites = static_cast<int>(gps.satellites);
    }else{
        gpsData.fix = false;
    }

    struct GpsData gpsDataToWrite = gpsData;
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);
    char timeString[100];
    std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localTime);

    std::ofstream gpsDataFile("gps.csv",std::ios::app);
    gpsDataFile << timeString << "," << gpsDataToWrite.fix << "," <<gpsDataToWrite.latitudeDegrees << "," <<gpsDataToWrite.longitudeDegrees << "," << gpsDataToWrite.geoidheight<< "," << gpsDataToWrite.altitude<< "," << gpsDataToWrite.speed << "," << gpsDataToWrite.fixquality << "," <<  gpsDataToWrite.satellites << std::endl;
    gpsDataFile.close();

    return gpsData;
}

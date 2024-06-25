#include <iostream>
#include "altimeter.h"
#include <pigpio.h>
#include <iomanip>
#include <fstream>

Altimeter::Altimeter(double sea_pressure) : bmp(), sea_pressure(sea_pressure) {
    if (bmp.begin_I2C(0x77)) {
        printf("start up success");
    } else {
        printf("Failed to initialize BMP390 sensor via I2C\n");
    }
}

struct AltData Altimeter::read() {
    struct AltData reading;
    reading.temp = bmp.readTemperature();
    reading.pressure = bmp.readPressure();
    reading.alt = bmp.readAltitude(sea_pressure);

    struct AltData readingToFile = reading;
    std::time_t currentTime = std::time(nullptr);
    std::tm* localTime = std::localtime(&currentTime);
    char timeString[100];
    std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", localTime);

    std::ofstream altDataFile("alt.csv",std::ios::app);
    altDataFile << timeString << "," <<  readingToFile.temp << "," << readingToFile.pressure << "," << readingToFile.alt << std::endl;
    altDataFile.close();

    return reading;
}

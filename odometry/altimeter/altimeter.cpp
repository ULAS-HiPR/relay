#include <iostream>
#include "altimeter.h"
#include <pigpio.h>

Altimeter::Altimeter(double sea_pressure) : bmp(), sea_pressure(sea_pressure) {
    if (bmp.begin_I2C(0x77)) {
        printf("start up success");
    } else {
        printf("Failed to initialize BMP390 sensor via I2C\n");
    }
}

struct AltData Altimeter::read() {
    struct AltData reading;

    // Assuming there are functions in BMP390 class to read temperature, pressure, and altitude
    reading.temp = bmp.readTemperature();
    reading.pressure = bmp.readPressure();
    reading.alt = bmp.readAltitude(sea_pressure);

    return reading;
}

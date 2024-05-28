#include "BMP390.h" // Include WiringPi library!
#include <iostream>

#include <pigpio.h>


int main(void) {
    // Initialize the pigpio library
    if (gpioInitialise() >= 0) {
        // Create an instance of the BMP390 class
        BMP390 bmp;
        
        // Initialize the BMP390 sensor via I2C
        if (bmp.begin_I2C(0x77)) {
            // Successfully initialized BMP390 sensor
            // Now you can perform sensor readings or other operations
            // For example:
            float temperature = bmp.readTemperature();
            float pressure = bmp.readPressure();
            float altitude = bmp.readAltitude(1013.25); // Provide sea level pressure as argument
            
            // Print sensor readings
            printf("Temperature: %.2f°C\n", temperature);
            printf("Pressure: %.2f Pa\n", pressure);
            printf("Altitude: %.2f meters\n", altitude);
        } else {
            // Failed to initialize BMP390 sensor
            printf("Failed to initialize BMP390 sensor via I2C\n");
        }

        // Close pigpio library
        gpioTerminate();
    } else {
        // Failed to initialize pigpio library
        printf("Failed to initialize pigpio library\n");
    }

    return 0;
}

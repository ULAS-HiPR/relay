#include "gps/GPS.h"
#include "altimeter/altimeter.h"
#include <unistd.h>
#include <iostream>
#include <wiringPi.h> 

//g++ altimeterTest.cpp altimeter/altimeter.cpp ../lib/altimeter/BMP390.cpp ../lib/altimeter/bmp3.c -o altTest -lwiringPi -lrt -lpthread
int main() {
    wiringPiSetupGpio();

    Altimeter alt(1016.8);
    int i = 0;
    while (true) {
        std::cout << i << "\n";
        struct AltData altData = alt.read();
        std::cout << "Temperature: " << altData.temp << " C\n";
        std::cout << "Pressure: " << altData.pressure << " Pa\n";
        std::cout << "Altitude: " << altData.alt << " m\n";

        i++;
        usleep(500000);
        std::cout<< "\n"; 
    }
    return 0;
}

#include "BMP390.h" // Include WiringPi library!
#include <iostream>

#include <pigpio.h>

int main(void)
{
   if (gpioInitialise() >= 0)
   {
  BMP390 bmp;
  bmp.begin_I2C(0x77);
   }
}


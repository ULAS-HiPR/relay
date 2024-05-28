#include "BMP390.h"
#include <iostream>
#include <pigpio.h>

#define BMP3XX_DEFAULT_ADDRESS (0x77) 

BMP390::BMP390(void) {
  //_meas_end = 0;
  //_filterEnabled = _tempOSEnabled = _presOSEnabled = false;
  std::cout << "work pls";
}

bool BMP390::begin_I2C(uint8_t addr){
  int initBus = i2cOpen(1, addr, 0);
  if (initBus == -1){
    std::cout << "Failed :(";
    return -1;
  }
  std::cout << "Yay :)";

  return true;
}
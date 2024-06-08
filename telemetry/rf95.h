// rf95_client.cpp
//
// Example program showing how to use RH_RF95 on Raspberry Pi
// Uses the bcm2835 library to access the GPIO pins to drive the RFM95 module
// Requires bcm2835 library to be already installed
// http://www.airspayce.com/mikem/bcm2835/
// Use the Makefile in this directory:
// cd example/raspi/rf95
// make
// sudo ./rf95_client
//
// Contributed by Charles-Henri Hallard based on sample RH_NRF24 by Mike Poublon

#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "../lib/telemetry/RH_RF69.h"
#include "../lib/telemetry/RH_RF95.h"

#include "../lib/telemetry/RasPiBoards.h"

class Radio{
public:
  Radio();
  void send(struct Data);
private:
  RH_RF95 rf95;
};

//to be changed
struct Data {
  double gps;
  double alt;
};
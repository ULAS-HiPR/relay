#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <cstdint>
#include <cstring>
#include <sstream>

#include "../lib/telemetry/RH_RF69.h"
#include "../lib/telemetry/RH_RF95.h"

#include "../lib/telemetry/RasPiBoards.h"
#include "dataStruct.h"

class Radio{
public:
  Radio();
  void send(std::string data);
  void printbuffer(const uint8_t* buffer, std::size_t len);
private:
  RH_RF95 rf95;
};
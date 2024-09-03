#include "Ada_GPS.h"
#include <iostream>
#include <wiringSerial.h>
#include <wiringPi.h>
#include <string.h>


static bool strStartsWith(const char *str, const char *prefix);

Ada_GPS::Ada_GPS(const std::string &serialPort) : 
    gpsSerialPort(serialPort), gpsSerialHandler(-1) {
    common_init();  // Set everything to common state
}

bool Ada_GPS::begin(uint32_t baud) {
    gpsSerialHandler = serialOpen(const_cast<char*>(gpsSerialPort.c_str()), baud);

    if (gpsSerialHandler < 0) {
        std::cerr << "Failed to open serial port: " << gpsSerialPort << std::endl;
        return false;
    }

    delay(10);  // Delay for 10 milliseconds
    return true;
}

void Ada_GPS::common_init(void) {
  gpsSerialHandler = -1;

  recvdflag = false;
  paused = false;
  lineidx = 0;
  currentline = line1;
  lastline = line2;

  hour = minute = seconds = year = month = day = fixquality = fixquality_3d =
      satellites = antenna = 0; // uint8_t
  lat = lon = mag = 0;          // char
  fix = false;                  // bool
  milliseconds = 0;             // uint16_t
  latitude = longitude = geoidheight = altitude = speed = angle = magvariation =
      HDOP = VDOP = PDOP = 0.0; // nmea_float_t
#ifdef NMEA_EXTENSIONS
  data_init();
#endif
}

Ada_GPS::~Ada_GPS() {
#ifdef NMEA_EXTENSIONS
  for (int i = 0; i < (int)NMEA_MAX_INDEX; i++)
    removeHistory((nmea_index_t)i); // to free any history mallocs
#endif
}

size_t Ada_GPS::available(void) {
    if (paused){
        return 0;
    }
    if (gpsSerialHandler >= 0) {
         int availableBytes = serialDataAvail(gpsSerialHandler);
        if (availableBytes < 0) {
            std::cerr << "Failed to check available data on serial port" << std::endl;
            return 0;
        }
        return static_cast<size_t>(availableBytes);
    }

    return 0;
}

size_t Ada_GPS::write(uint8_t c) {
    if (gpsSerialHandler >= 0) {
        serialPutchar(gpsSerialHandler, c);
        return 1;
      }
    return 0;  // Failed to write the byte

}

char Ada_GPS::read(void) {
  std::cout << "reading" << std::endl;
    static uint32_t firstChar = 0;
 // first character received in current sentence
    uint32_t tStart = millis();  // as close as we can get to time char was sent
    char c = 0;
  std::cout << "did read" << std::endl;
    if (paused)
        return c;

    if (gpsSerialHandler >= 0) {
        if (serialDataAvail(gpsSerialHandler) <= 0)
            return c;
        c = serialGetchar(gpsSerialHandler);
    }

    // Serial.print(c);
    currentline[lineidx] = c;
    lineidx = lineidx + 1;
    if (lineidx >= MAXLINELENGTH)
        lineidx = MAXLINELENGTH - 1; // ensure there is someplace to put the next received character

    if (c == '\n') {
        currentline[lineidx] = 0;

        if (currentline == line1) {
            currentline = line2;
            lastline = line1;
        } else {
            currentline = line1;
            lastline = line2;
        }

        // Serial.println("----");
        // Serial.println((char *)lastline);
        // Serial.println("----");
        lineidx = 0;
        recvdflag = true;
        recvdTime = millis(); // time we got the end of the string
        sentTime = firstChar;
        firstChar = 0; // there are no characters yet
        return c;      // wait until next character to set time
    }

    if (firstChar == 0)
        firstChar = tStart;
    return c;
}

void Ada_GPS::sendCommand(const char *str) { 
    if (gpsSerialHandler >= 0) {
        serialPuts(gpsSerialHandler, const_cast<char*>(str)); // Changed from serWrite to wiringSerial function
        serialPuts(gpsSerialHandler, const_cast<char*>("\n"));   // Write the newline character
    }
}

bool Ada_GPS::newNMEAreceived(void) { return recvdflag; }

void Ada_GPS::pause(bool p) { paused = p; }

char *Ada_GPS::lastNMEA(void) {
  recvdflag = false;
  return (char *)lastline;
}

bool Ada_GPS::waitForSentence(const char *wait4me, uint8_t max,
                                   bool usingInterrupts) {
  uint8_t i = 0;
  while (i < max) {
    if (!usingInterrupts)
      read();

    if (newNMEAreceived()) {
      char *nmea = lastNMEA();
      i++;

      if (strStartsWith(nmea, wait4me))
        return true;
    }
  }

  return false;
}

bool Ada_GPS::standby(void) {
  if (inStandbyMode) {
    return false; // Returns false if already in standby mode, so that you do
                  // not wake it up by sending commands to GPS
  } else {
    inStandbyMode = true;
    sendCommand(PMTK_STANDBY);
    // return waitForSentence(PMTK_STANDBY_SUCCESS);  // don't seem to be fast
    // enough to catch the message, or something else just is not working
    return true;
  }
}

bool Ada_GPS::wakeup(void) {
  if (inStandbyMode) {
    inStandbyMode = false;
    sendCommand(""); // send byte to wake it up
    return waitForSentence(PMTK_AWAKE);
  } else {
    return false; // Returns false if not in standby mode, nothing to wakeup
  }
}

nmea_float_t Ada_GPS::secondsSinceFix() {
  return (millis() - lastTime) / 1000.;
}


nmea_float_t Ada_GPS::secondsSinceTime() {
  return (millis() - lastTime) / 1000.0;
}

nmea_float_t Ada_GPS::secondsSinceDate() {
  return (millis() - lastDate) / 1000.0;
}

void Ada_GPS::resetSentTime() {
  sentTime = millis();
}

static bool strStartsWith(const char *str, const char *prefix) {
  while (*prefix) {
    if (*prefix++ != *str++)
      return false;
  }
  return true;
}

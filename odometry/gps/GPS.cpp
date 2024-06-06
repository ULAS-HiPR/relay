#include "GPS.h"
#include <iostream>
#include <pigpio.h>
#include <string.h>


static bool strStartsWith(const char *str, const char *prefix);

GPS::GPS(const std::string &serialPort) : 
    gpsSerialPort(serialPort), gpsSerialHandler(-1) {
    common_init();  // Set everything to common state
}

bool GPS::begin(uint32_t baud) {
    gpsSerialHandler = serOpen(const_cast<char*>(gpsSerialPort.c_str()), baud, 0);

    if (gpsSerialHandler < 0) {
        std::cerr << "Failed to open serial port: " << gpsSerialPort << std::endl;
        return false;
    }

    gpioSleep(PI_TIME_RELATIVE, 0, 10000);  // Delay for 10 milliseconds
    return true;
}

void GPS::common_init(void) {
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

GPS::~GPS() {
#ifdef NMEA_EXTENSIONS
  for (int i = 0; i < (int)NMEA_MAX_INDEX; i++)
    removeHistory((nmea_index_t)i); // to free any history mallocs
#endif
}

size_t GPS::available(void) {
    if (paused){
        return 0;
    }
    if (gpsSerialHandler >= 0) {
        int availableBytes = serDataAvailable(gpsSerialHandler);
        if (availableBytes < 0) {
            std::cerr << "Failed to check available data on serial port" << std::endl;
            return 0;
        }
        return static_cast<size_t>(availableBytes);
    }

    return 0;
}

size_t GPS::write(uint8_t c) {
    if (gpsSerialHandler >= 0) {
        if (serWriteByte(gpsSerialHandler, c) == 0) {
            return 1;  // Return 1 byte written successfully
        } else {
            std::cerr << "Failed to write byte to serial port" << std::endl;
            return 0;  // Failed to write the byte
        }
    }

    return 0;  // Serial handler not initialized
}

char GPS::read(void) {
    static uint32_t firstChar = 0; // first character received in current sentence
    uint32_t tStart = gpioTick();  // as close as we can get to time char was sent
    char c = 0;

    if (paused)
        return c;

    if (gpsSerialHandler >= 0) {
        if (serDataAvailable(gpsSerialHandler) <= 0)
            return c;
        c = serReadByte(gpsSerialHandler);
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
        recvdTime = gpioTick(); // time we got the end of the string
        sentTime = firstChar;
        firstChar = 0; // there are no characters yet
        return c;      // wait until next character to set time
    }

    if (firstChar == 0)
        firstChar = tStart;
    return c;
}

void GPS::sendCommand(const char *str) { 
    if (gpsSerialHandler >= 0) {
        serWrite(gpsSerialHandler, const_cast<char*>(str), strlen(str));  // Write the command
        serWrite(gpsSerialHandler,  const_cast<char*>("\n"), 1);           // Write the newline character
    }
}


bool GPS::newNMEAreceived(void) { return recvdflag; }

void GPS::pause(bool p) { paused = p; }

char *GPS::lastNMEA(void) {
  recvdflag = false;
  return (char *)lastline;
}

bool GPS::waitForSentence(const char *wait4me, uint8_t max,
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

bool GPS::standby(void) {
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

bool GPS::wakeup(void) {
  if (inStandbyMode) {
    inStandbyMode = false;
    sendCommand(""); // send byte to wake it up
    return waitForSentence(PMTK_AWAKE);
  } else {
    return false; // Returns false if not in standby mode, nothing to wakeup
  }
}

inline uint32_t millis() {
    return gpioTick() / 1000;
}

nmea_float_t GPS::secondsSinceFix() {
  return (millis() - lastTime) / 1000.;
}


nmea_float_t GPS::secondsSinceTime() {
  return (millis() - lastTime) / 1000.0;
}

nmea_float_t GPS::secondsSinceDate() {
  return (millis() - lastDate) / 1000.0;
}

void GPS::resetSentTime() {
  sentTime = millis();
}

static bool strStartsWith(const char *str, const char *prefix) {
  while (*prefix) {
    if (*prefix++ != *str++)
      return false;
  }
  return true;
}

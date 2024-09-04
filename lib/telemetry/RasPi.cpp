// RasPi.cpp
//
// Routines for implementing RadioHead on Raspberry Pi
// using WiringPi library for GPIO and SPI
//
// Contributed by Mike Poublon and used with permission

#include <RadioHead.h>

#if (RH_PLATFORM == RH_PLATFORM_RASPI)
#include <sys/time.h>
#include <time.h>
#include <wiringPi.h>        // WiringPi library for GPIO
#include <wiringPiSPI.h>     // WiringPi SPI functionality
#include "RasPi.h"

//Initialize the values for sanity
timeval RHStartTime;

void SPIClass::begin()
{
  //Set SPI Defaults
  uint16_t divider = 256;  // WiringPi uses actual clock speed, not dividers
  uint8_t bitorder = 0;    // WiringPi does not directly support setting bit order, but MSB is default
  uint8_t datamode = 0;    // WiringPi SPI mode (0-3)

  begin(divider, bitorder, datamode);
}

void SPIClass::begin(uint16_t divider, uint8_t bitOrder, uint8_t dataMode)
{
  
  wiringPiSPISetupMode(1, 1000000 / divider, dataMode); // Set up SPI at the requested speed and mode

  //Initialize a timestamp for millis calculation
  gettimeofday(&RHStartTime, NULL);
}

void SPIClass::end()
{
  // WiringPi SPI does not have an explicit "end" function, so nothing is needed here
}

void SPIClass::setBitOrder(uint8_t bitOrder)
{
  // WiringPi SPI does not allow changing bit order, so this function is empty
}

void SPIClass::setDataMode(uint8_t mode)
{
  // WiringPi SPI mode is set during setup, so this function is not needed after that
}

void SPIClass::setClockDivider(uint16_t rate)
{
  // WiringPi SPI uses speed in Hz directly, so the divider concept from bcm2835 is replaced by the frequency
  // The frequency is set during setup, so this function is not needed afterward
}

byte SPIClass::transfer(byte _data)
{
  // WiringPi does not explicitly handle chip select; you would need to manage it with digitalWrite if required
  unsigned char data = _data;
  wiringPiSPIDataRW(0, &data, 1);
  return data;
}

void pinMode(unsigned char pin, unsigned char mode)
{
  if (pin == NOT_A_PIN)
    return;

  if (mode == OUTPUT)
  {
    pinMode(pin, OUTPUT);
  }
  else
  {
    pinMode(pin, INPUT);
  }
}

void digitalWrite(unsigned char pin, unsigned char value)
{
  if (pin == NOT_A_PIN)
    return;

  digitalWrite(pin, value);
}

unsigned char digitalRead(unsigned char pin) {
  if (pin == NOT_A_PIN)
    return 0;

  return digitalRead(pin);
}


void delay(unsigned long ms)
{
  delay(ms);  // WiringPi has a built-in delay function that uses milliseconds
}

long random(long min, long max)
{
  long diff = max - min;
  long ret = diff * rand() + min;
  return ret;
}

// Dump a buffer trying to display ASCII or HEX
// depending on contents
void printbuffer(uint8_t buff[], int len)
{
  int i;
  bool ascii = true;

  // Check for only printable characters
  for (i = 0; i < len; i++) {
    if (buff[i] < 32 || buff[i] > 127) {
      if (buff[i] != 0 || i != len - 1) {
        ascii = false;
        break;
      }
    }
  }

  // now do real display according to buffer type
  // note each char one by one because we're not sure 
  // string will have \0 on the end
  for (int i = 0; i < len; i++) {
    if (ascii) {
      printf("%c", buff[i]);
    } else {
      printf(" %02X", buff[i]);
    }
  }
}

void SerialSimulator::begin(int baud)
{
  //No implementation necessary - Serial emulation on Linux = standard console
  //
  //Initialize a timestamp for millis calculation - we do this here as well in case SPI
  //isn't used for some reason
  gettimeofday(&RHStartTime, NULL);
}

size_t SerialSimulator::println(const char* s)
{
  print(s);
  printf("\n");
}

size_t SerialSimulator::print(const char* s)
{
  printf(s);
}

size_t SerialSimulator::print(unsigned int n, int base)
{
  if (base == DEC)
    printf("%d", n);
  else if (base == HEX)
    printf("%02x", n);
  else if (base == OCT)
    printf("%o", n);
  // TODO: BIN
}

size_t SerialSimulator::print(char ch)
{
  printf("%c", ch);
}

size_t SerialSimulator::println(char ch)
{
  printf("%c\n", ch);
}

size_t SerialSimulator::print(unsigned char ch, int base)
{
  return print((unsigned int)ch, base);
}

size_t SerialSimulator::println(unsigned char ch, int base)
{
  print((unsigned int)ch, base);
  printf("\n");
}

#endif

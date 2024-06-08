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

#define BOARD_LORASPI

#include "rf95.h"
#include "../lib/telemetry/RasPi.h"

// Our RFM95 Configuration 
#define RF_FREQUENCY  434.00
#define RF_GATEWAY_ID 1 
#define RF_NODE_ID    10

Radio::Radio() : rf95(RF_CS_PIN, RF_IRQ_PIN){
  if (!bcm2835_init()) {
    fprintf( stderr, "%s bcm2835_init() Failed\n\n", __BASEFILE__ );
    return;
  }

  if (!bcm2835_init()) {
    fprintf( stderr, "%s bcm2835_init() Failed\n\n", __BASEFILE__ );
    return;
  };
  
  // IRQ Pin input/pull down 
  pinMode(RF_IRQ_PIN, INPUT);
  bcm2835_gpio_set_pud(RF_IRQ_PIN, BCM2835_GPIO_PUD_DOWN);
  // Pulse a reset on module
  pinMode(RF_RST_PIN, OUTPUT);
  digitalWrite(RF_RST_PIN, LOW );
  bcm2835_delay(150);
  digitalWrite(RF_RST_PIN, HIGH );
  bcm2835_delay(100);

  if (!rf95.init()) {
    fprintf( stderr, "\nRF95 module init failed, Please verify wiring/module\n" );
    return;
  } ;
  rf95.available();
    bcm2835_gpio_ren(RF_IRQ_PIN);
    rf95.setTxPower(23, false); 
    rf95.setFrequency( RF_FREQUENCY );
    rf95.setThisAddress(RF_NODE_ID);
    rf95.setHeaderFrom(RF_NODE_ID);
    rf95.setHeaderTo(RF_GATEWAY_ID);  
    printf("RF95 node #%d init OK @ %3.2fMHz\n", RF_NODE_ID, RF_FREQUENCY );
  
};
    //Begin the main body of code

void Radio::send(struct Data){
  //format data to string
  uint8_t data[] = "Hi Raspi!";
  uint8_t len = sizeof(data);
        
  printf("Sending %02d bytes to node #%d => ", len, RF_GATEWAY_ID );
  printbuffer(data, len);
  printf("\n" );
  rf95.send(data, len);
  rf95.waitPacketSent();
  bcm2835_delay(100);
};

int main(void){
  Radio rf;

  struct Data data;
  data.alt = 10.1;
  data.gps = 1.1;
  rf.send(data);
};
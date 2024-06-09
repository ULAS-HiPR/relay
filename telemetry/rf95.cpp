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

void Radio::send(const sendingData& data) {
    std::string jsonString = serializeSendingDataToJsonLike(data);
    //std::cout << jsonString;

    std::size_t bufferSize = jsonString.size();
    uint8_t* buffer = new uint8_t[bufferSize + 1];  // Allocate one extra byte for the null terminator
    
    jsonLikeToUint8Array(jsonString, buffer);
    buffer[bufferSize] = '\0';  // Ensure the buffer is null-terminated

    printf("Sending %02zu bytes to node #%d => ", bufferSize, RF_GATEWAY_ID);
    //printbuffer(buffer, bufferSize);
    printf("\n");

    rf95.send(buffer, bufferSize);
    rf95.waitPacketSent();
    bcm2835_delay(100);

    delete[] buffer;
}

void Radio::printbuffer(const uint8_t* buffer, std::size_t len) {
    for (std::size_t i = 0; i < len; ++i) {
        printf("%02x ", buffer[i]);
    }
}

std::string serializeSendingDataToJsonLike(const sendingData& data) {
    std::ostringstream oss;
    oss << "{ "
        << "alt:{"
        << "temp:" << data.altData.temperature << ","
        << "pres:" << data.altData.pressure << ","
        << "alt:" << data.altData.altitude
        << "},"
        << "gps:{"
        << "fix:" << (data.gpsData.fix ? "true" : "false") << ","
        << "lat:" << data.gpsData.lat << ","
        << "lon:" << data.gpsData.lon << ","
        << "alt:" << data.gpsData.altitude << ","
        << "spe:" << data.gpsData.speed << ","
        << "sat:" << data.gpsData.satellites
        << "},"
        << "dev:{"
        << "use:" << data.deviceData.usage << ","
        << "temp:" << data.deviceData.temperature
        << "},"
        << "agri:{"
        << "}"
        << "}";
    return oss.str();
}

void jsonLikeToUint8Array(const std::string& jsonString, uint8_t* buffer) {
    std::memcpy(buffer, jsonString.c_str(), jsonString.size());
}
#include <iostream>
#include <pigpio.h>
#include <cstring>
#include <unistd.h> // For usleep
#include "RH_RF95.h" // Make sure you have the correct RF95 library for Raspberry Pi

#define RFM95_CS 7 // BCM GPIO 7 (CE7)
#define RFM95_RST 25 // BCM GPIO 25
#define RFM95_INT 5 // BCM GPIO 24

#define RF95_FREQ 434.0

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
    if (gpioInitialise() < 0) {
        std::cerr << "pigpio initialization failed" << std::endl;
        return;
    }

    gpioSetMode(RFM95_RST, PI_OUTPUT);
    gpioWrite(RFM95_RST, PI_HIGH);

    std::cout << "Raspberry Pi LoRa TX Test!" << std::endl;

    // manual reset
    gpioWrite(RFM95_RST, PI_LOW);
    usleep(10000); // 10 ms
    gpioWrite(RFM95_RST, PI_HIGH);
    usleep(10000); // 10 ms

    if (!rf95.init()) {
        std::cerr << "LoRa radio init failed" << std::endl;
        while (1);
    }
    std::cout << "LoRa radio init OK!" << std::endl;

    if (!rf95.setFrequency(RF95_FREQ)) {
        std::cerr << "setFrequency failed" << std::endl;
        while (1);
    }
    std::cout << "Set Freq to: " << RF95_FREQ << " MHz" << std::endl;

    rf95.setTxPower(23, false);
}

int16_t packetnum = 0;  // packet counter, we increment per transmission

void loop() {
    std::cout << "Sending to rf95_server" << std::endl;
    // Send a message to rf95_server

    char radiopacket[20] = "Hello World #      ";
    snprintf(radiopacket + 13, 7, "%d", packetnum++);
    std::cout << "Sending " << radiopacket << std::endl;
    radiopacket[19] = 0;

    std::cout << "Sending..." << std::endl;
    usleep(10000); // 10 ms
    rf95.send((uint8_t *)radiopacket, 20);

    std::cout << "Waiting for packet to complete..." << std::endl;
    usleep(10000); // 10 ms
    rf95.waitPacketSent();
    // Now wait for a reply
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    std::cout << "Waiting for reply..." << std::endl;
    usleep(10000); // 10 ms
    if (rf95.waitAvailableTimeout(1000)) {
        // Should be a reply message for us now
        if (rf95.recv(buf, &len)) {
            std::cout << "Got reply: " << (char*)buf << std::endl;
            std::cout << "RSSI: " << rf95.lastRssi() << std::endl;
        } else {
            std::cout << "Receive failed" << std::endl;
        }
    } else {
        std::cout << "No reply, is there a listener around?" << std::endl;
    }
    usleep(1000000); // 1 second
}

int main() {
    setup();
    while (true) {
        loop();
    }
    gpioTerminate();
    return 0;
}

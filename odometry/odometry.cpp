#include "gps/GPS.h"
#include "altimeter/altimeter.h"
#include <unistd.h>
#include <iostream>
#include <wiringPi.h> 
#include <cstring>
#include "MQTTClient.h"

#define ADDRESS     "localhost:1883"
#define CLIENTID    "odometry"



int main() {
    //connecting to mqtt
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    if ((rc = MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL)) != MQTTCLIENT_SUCCESS)
    {
         printf("Failed to create client, return code %d\n", rc);
         exit(EXIT_FAILURE);
    }

    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    pubmsg.qos = 0;
    pubmsg.retained = 0;

    //gpio setup
    wiringPiSetupGpio();

    Altimeter alt(1023.0);
    GPS gps("/dev/ttyAMA10");

    while (true) {
        struct AltData altData = alt.read();
        struct GpsData gpsData = gps.read();

        char* jsonGpsData = gps.serializeGpsData(&gpsData);
        char* jsonAltData = alt.serializeAltData(&altData);

        pubmsg.payload = jsonGpsData;
        pubmsg.payloadlen = strlen(jsonGpsData);

        if ((rc = MQTTClient_publishMessage(client, "GPS", &pubmsg, &token)) != MQTTCLIENT_SUCCESS) {
            printf("Failed to publish message, return code %d\n", rc);
            free(jsonGpsData);  // Free allocated memory before exiting
            exit(EXIT_FAILURE);
        }

        pubmsg.payload = jsonAltData;
        pubmsg.payloadlen = strlen(jsonAltData);

        if ((rc = MQTTClient_publishMessage(client, "ALT", &pubmsg, &token)) != MQTTCLIENT_SUCCESS) {
            printf("Failed to publish message, return code %d\n", rc);
            free(jsonAltData);  // Free allocated memory before exiting
            exit(EXIT_FAILURE);
        }

        free(jsonAltData);
        free(jsonGpsData); 


        std::cout << "Temperature: " << altData.temp << " C\n";
        std::cout << "Pressure: " << altData.pressure << " Pa\n";
        std::cout << "Altitude: " << altData.alt << " m\n";

        if (gpsData.fix) {
            std::cout << "Fix: Yes" << std::endl;
            std::cout << std::fixed << std::setprecision(6);
            std::cout << "Latitude: " << gpsData.latitudeDegrees << std::endl;
            std::cout << "Longitude: " << gpsData.longitudeDegrees << std::endl;
            std::cout << std::fixed << std::setprecision(2);
            std::cout << "Geoid Height: " << gpsData.geoidheight << std::endl;
            std::cout << "AltitudeGPS: " << gpsData.altitude << std::endl;
            std::cout << "Speed: " << gpsData.speed << std::endl;
            std::cout << "Fix Quality: " << gpsData.fixquality << std::endl;
            std::cout << "Satellites: " << gpsData.satellites << std::endl;
            //rate limiting
            usleep(500000);
        } else {
          std::cout << "Fix: No" << std::endl;
          usleep(10000);
        }
        std::cout<< u8"\033[2J\033[1;1H"; 
    }
    return 0;
}
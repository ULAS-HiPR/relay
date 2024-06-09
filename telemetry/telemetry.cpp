#define BOARD_LORASPI
#include "telemetry.h"


sendingData parseData(const std::string& input) {
    std::istringstream iss(input);
    std::string line;
    sendingData data;

    while (std::getline(iss, line)) {
        std::istringstream lineStream(line);
        std::string key;

        if (lineStream >> key) {
            if (key == "Health:") {
                char brace;
                lineStream >> brace >> data.deviceData.usage >> data.deviceData.temperature >> brace;
            } else if (key == "Altitude:") {
                char brace;
                lineStream >> brace >> data.altData.temperature >> data.altData.pressure >> data.altData.altitude >> brace;
            } else if (key == "GPS:") {
                char brace;
                std::string boolValue;
                lineStream >> brace >> boolValue >> data.gpsData.lat >> data.gpsData.lon >> data.gpsData.altitude >> data.gpsData.speed >> data.gpsData.satellites >> brace;
                data.gpsData.fix = (boolValue == "true");
            }
        }
    }
    std::cout<< u8"\033[2J\033[1;1H"; 
    return data;
}

int main() {
    Radio rf95;
    std::string input;
    std::string line;
    bool capturing = false;
    while (true) {
        std::getline(std::cin, line);

        if (line.find('<') != std::string::npos) {
            capturing = true;
            input.clear();
        }

        if (capturing) {
            input += line + "\n";
        }

        if (line.find('>') != std::string::npos) {
            capturing = false;
            sendingData data = parseData(input);
            rf95.send(data);
        }
        
    }

    return 0;
}
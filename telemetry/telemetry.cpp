#define BOARD_LORASPI
#include "telemetry.h"

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
            input = input.substr(2, input.size() - 5);
            rf95.send(input);
            std::cout<< u8"\033[2J\033[1;1H"; 
        }
        
        
    }

    return 0;
}
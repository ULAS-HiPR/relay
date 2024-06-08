#define __BMP3XX_H__

#include "bmp3.h"
#include "BMP390.h"
#define BMP3XX_DEFAULT_ADDRESS (0x77) 

class Altimeter{
public:
    Altimeter(double sea_pressure);
    struct AltData read();
private:
    BMP390 bmp;
    double sea_pressure;
};



struct AltData {
    double temp;
    double pressure;
    double alt;
};
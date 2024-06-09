struct AltData {
    double temperature;
    double pressure;
    double altitude;
};

struct GpsData {
    bool fix;
    double lat;
    double lon;
    double altitude;
    double speed;
    int satellites;
};

struct DeviceData {
    double usage;
    double temperature;
};

struct AgriData {};

struct sendingData {
    AltData altData;
    GpsData gpsData;
    DeviceData deviceData;
    AgriData agriData;
};
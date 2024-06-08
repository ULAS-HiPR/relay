g++ GPS.cpp ../../lib/gps/Ada_GPS.cpp ../../lib/gps/NMEA_build.cpp ../../lib/gps/NMEA_data.cpp ../../lib/gps/NMEA_parse.cpp -o gpsProgram -lpigpio -lrt -lpthread
sudo ./gps
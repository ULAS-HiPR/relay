g++ odometry.cpp gps/GPS.cpp ../lib/gps/Ada_GPS.cpp ../lib/gps/NMEA_build.cpp ../lib/gps/NMEA_data.cpp ../lib/gps/NMEA_parse.cpp altimeter/altimeter.cpp ../lib/altimeter/BMP390.cpp ../lib/altimeter/bmp3.c -o odometryMain -lpigpio -lrt -lpthread
sudo ./odometryMain

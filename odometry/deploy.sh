g++ odometry.cpp gps/GPS.cpp gps/Ada_GPS.cpp gps/NMEA_build.cpp gps/NMEA_data.cpp gps/NMEA_parse.cpp altimeter/altimeter.cpp altimeter/BMP390.cpp altimeter/bmp3.c -o odometryMain -lpigpio -lrt -lpthread
sudo ./odometryMain

#!/bin/bash
#! /home/EanSat/env/bin/python3

echo "Building Health Service"
cd health/
go build
cd ..

echo "Building Telemetry Service"
cd telemetry/
chmod +x telemetryMain.py
pyinstaller telemetryMain.py --clean --noconfirm
sudo mv dist/telemetryMain/telemetryMain /home/EanSat/relay/telemetry
cd ..

echo "Building Odometry Service"
cd odometry/
g++ odometry.cpp gps/GPS.cpp ../lib/gps/Ada_GPS.cpp ../lib/gps/NMEA_build.cpp ../lib/gps/NMEA_data.cpp ../lib/gps/NMEA_parse.cpp altimeter/altimeter.cpp ../lib/altimeter/BMP390.cpp ../lib/altimeter/bmp3.c -o odometryMain -lwiringPi -lrt -lpthread -lpaho-mqtt3c
cd ..

echo "All Services Built"
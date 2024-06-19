#!/bin/bash
PROGRAM_NAME="ASOdometry"
SOURCE_DIR="/home/agrisat/relay/odometry"
SERVICE_DIR="/etc/systemd/system"
BINARY="odometryMain"

g++ odometry.cpp gps/GPS.cpp ../lib/gps/Ada_GPS.cpp ../lib/gps/NMEA_build.cpp ../lib/gps/NMEA_data.cpp ../lib/gps/NMEA_parse.cpp altimeter/altimeter.cpp ../lib/altimeter/BMP390.cpp ../lib/altimeter/bmp3.c -o $SOURCE_DIR/$BINARY -lpigpio -lrt -lpthread

sudo tee "$SERVICE_DIR/$PROGRAM_NAME.service" > /dev/null << EOF
[Unit]
Description=AgriSat Odometry Service
After=network.target
[Service]
Type=simple
Restart=always
ExecStart=$SOURCE_DIR/$BINARY
[Install]
WantedBy=multi-user.target
EOF

sudo systemctl daemon-reload
sudo systemctl enable "$PROGRAM_NAME.service"
sudo systemctl start "$PROGRAM_NAME.service"
sudo systemctl status "$PROGRAM_NAME.service"
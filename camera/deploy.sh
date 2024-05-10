#!/bin/bash

PROGRAM_NAME="ASCamera"
SOURCE_DIR="/home/agrisat/relay/camera"

SERVICE_DIR="/etc/systemd/system"

# Compile the Go program
cd "$SOURCE_DIR" || exit
go build -o .
# Create the systemd service file
sudo tee "$SERVICE_DIR/$PROGRAM_NAME.service" > /dev/null << EOF
[Unit]
Description=Agrisat Camera Service
After=network.target

[Service]
Type=simple
Restart=always
ExecStart=$DEST_DIR/main

[Install]
WantedBy=multi-user.target
EOF

sudo systemctl daemon-reload
sudo systemctl enable "$PROGRAM_NAME.service"
sudo systemctl start "$PROGRAM_NAME.service"
sudo systemctl status "$PROGRAM_NAME.service"

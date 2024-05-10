#!/bin/bash

PROGRAM_NAME="ASCamera"
SOURCE_DIR="/home/agrisat/relay/camera"

DEST_DIR="/usr/local/bin"
SERVICE_DIR="/etc/systemd/system"

# Compile the Go program
cd "$SOURCE_DIR" || exit
go build -o "$PROGRAM_NAME"

# Move the binary to the destination directory
sudo mv "$PROGRAM_NAME" "$DEST_DIR"

# Create the systemd service file
sudo tee "$SERVICE_DIR/$PROGRAM_NAME.service" > /dev/null << EOF
[Unit]
Description=Agrisat Camera Service
After=network.target

[Service]
Type=simple
Restart=always
ExecStart=$DEST_DIR/$PROGRAM_NAME

[Install]
WantedBy=multi-user.target
EOF

sudo systemctl daemon-reload
sudo systemctl enable "$PROGRAM_NAME.service"
sudo systemctl start "$PROGRAM_NAME.service"
sudo systemctl status "$PROGRAM_NAME.service"

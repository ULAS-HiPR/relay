#!/bin/bash
PROGRAM_NAME="ASCamera"
SOURCE_DIR="/home/agrisat/relay/camera"
SERVICE_DIR="/etc/systemd/system"
GO_BINARY="/usr/local/bin/camera"
GO_PROGRAM="$SOURCE_DIR/main.go"

go build -o $GO_BINARY $GO_PROGRAM

sudo tee "$SERVICE_DIR/$PROGRAM_NAME.service" > /dev/null << EOF
[Unit]
Description=AgriSat Camera Service
After=network.target
[Service]
Type=simple
Restart=always
ExecStart=$GO_BINARY
[Install]
WantedBy=multi-user.target
EOF

sudo systemctl daemon-reload
sudo systemctl enable "$PROGRAM_NAME.service"
sudo systemctl start "$PROGRAM_NAME.service"
sudo systemctl status "$PROGRAM_NAME.service"
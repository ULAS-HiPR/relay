#!/bin/bash
PROGRAM_NAME="ASTelemetry"
SOURCE_DIR="/home/agrisat/relay/telemetry"
SERVICE_DIR="/etc/systemd/system"
BINARY="telemetryMain"

make

sudo tee "$SERVICE_DIR/$PROGRAM_NAME.service" > /dev/null << EOF
[Unit]
Description=AgriSat Telemetry Service
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
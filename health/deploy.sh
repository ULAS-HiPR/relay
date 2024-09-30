#!/bin/bash
PROGRAM_NAME="ESHealth"
SOURCE_DIR="/home/EanSat/relay/health"
SERVICE_DIR="/etc/systemd/system"
BINARY="health"

sudo tee "$SERVICE_DIR/$PROGRAM_NAME.service" > /dev/null << EOF
[Unit]
Description=EanSat Telemetry Service
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
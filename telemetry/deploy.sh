#!/bin/bash
PROGRAM_NAME="ESTelemetry"
SOURCE_DIR="/home/EanSat/relay/telemetry"
SERVICE_DIR="/etc/systemd/system"

sudo tee "$SERVICE_DIR/$PROGRAM_NAME.service" > /dev/null << EOF
[Unit]
Description=EanSat Telemetry Service
After=network.target
[Service]
Type=simple
Restart=always
ExecStart= /home/EanSat/env/bin/python /home/EanSat/relay/telemetry/telemetryMain.py
[Install]
WantedBy=multi-user.target
EOF

sudo systemctl daemon-reload
sudo systemctl enable "$PROGRAM_NAME.service"
sudo systemctl start "$PROGRAM_NAME.service"
sudo systemctl status "$PROGRAM_NAME.service"
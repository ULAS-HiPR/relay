#!/bin/bash

PROGRAM_NAME="ASAgriNet"
SOURCE_DIR="/home/agrisat/relay/agrinet"

SERVICE_DIR="/etc/systemd/system"

sudo tee "$SERVICE_DIR/$PROGRAM_NAME.service" > /dev/null << EOF
[Unit]
Description=AgriSat AgriNet Service
After=network.target
[Service]
Type=simple
Restart=always
ExecStart=python $SOURCE_DIR/main.py
[Install]
WantedBy=multi-user.target
EOF

sudo systemctl daemon-reload
sudo systemctl enable "$PROGRAM_NAME.service"
sudo systemctl start "$PROGRAM_NAME.service"
sudo systemctl status "$PROGRAM_NAME.service"
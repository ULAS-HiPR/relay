#!/bin/bash -e

sudo cp health.service /etc/systemd/system/health.service
sudo systemctl daemon-reload
sudo systemctl enable health.service
sudo systemctl start health.service
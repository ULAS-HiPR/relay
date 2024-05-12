#!/bin/bash

SERVICE_FILE=agrinet.service

sudo mv $SERVICE_FILE /etc/systemd/system/

sudo systemctl daemon-reload
sudo systemctl enable $SERVICE_FILE
sudo systemctl start $SERVICE_FILE
sudo systemctl status $SERVICE_FILE
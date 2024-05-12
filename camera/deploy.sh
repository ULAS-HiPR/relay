#!/bin/bash

SERVICE_FILE=camera.service
GO_PROGRAM=/home/agrisat/relay/camera/main.go
GO_BINARY=/usr/local/bin/camera

go build -o $GO_BINARY $GO_PROGRAM
sudo chmod +x $GO_BINARY

sudo mv $SERVICE_FILE /etc/systemd/system/

sudo systemctl daemon-reload
sudo systemctl enable $SERVICE_FILE
sudo systemctl start $SERVICE_FILE
sudo systemctl status $SERVICE_FILE
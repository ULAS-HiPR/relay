#!/bin/bash

SERVICE_FILE=camera.service
GO_PROGRAM=main.go
GO_BINARY=/usr/local/bin/camera

go build -o $GO_BINARY $GO_PROGRAM
sudo chmod +x $GO_BINARY

sudo mv $SERVICE_FILE /etc/systemd/system/

sudo systemctl daemon-reload
sudo systemctl enable camera.service
sudo systemctl start camera.service
sudo systemctl status camera.service
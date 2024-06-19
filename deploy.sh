#!/bin/bash

folders=(
    "camera"
    "agrinet"
    "telemetry"
    "odometry"
)

for folder in "${folders[@]}"
do 
    chmod +x "$folder/deploy.sh"
    bash "$folder/deploy.sh"
done
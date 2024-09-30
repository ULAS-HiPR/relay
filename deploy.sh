#!/bin/bash

folders=(
    "camera"
    "health"
    "odometry"
    "telemetry"
)

for folder in "${folders[@]}"
do 
    # find variable EXECUTABLE in the deploy.sh file (name of process to kill)
    EXECUTABLE=$(grep -oP 'EXECUTABLE="\K[^"]+' "$folder/deploy.sh")
    
    chmod +x "$folder/deploy.sh"
    ./"$folder/deploy.sh"

done
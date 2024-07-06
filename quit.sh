#!/bin/bash

folders=(
    "camera"
    "agrinet"
)

for folder in "${folders[@]}"
do 
    # find variable EXECUTABLE in the deploy.sh file (name of process to kill)
    EXECUTABLE=$(grep -oP 'EXECUTABLE="\K[^"]+' "$folder/deploy.sh")
    
    echo "Quitting $EXECUTABLE service..."
    pkill -f $EXECUTABLE # kill the process by name
    echo "$EXECUTABLE service stopped"
done
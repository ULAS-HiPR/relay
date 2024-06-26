#!/bin/bash

# Configuration
OUTPUT_DIR="/home/agrisat/relay/results/camera"
WIDTH=640
HEIGHT=480
INTERVAL=1 # Interval in seconds

# Ensure output directory exists
mkdir -p "$OUTPUT_DIR"

# Capture images in a loop
while true; do
  TIMESTAMP=$(date +"%Y-%m-%dT%H:%M:%S")
  FILEPATH="${OUTPUT_DIR}/${TIMESTAMP}.jpg"
  
  # Capture image
  libcamera-jpeg -o "$FILEPATH" --width "$WIDTH" --height "$HEIGHT" --signal
  
  # Print a dot to indicate an image has been captured
  echo -n "."
  
  # Wait for the specified interval
  sleep "$INTERVAL"
done
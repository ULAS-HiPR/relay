#!/bin/bash

# Name of the service
EXECUTABLE="sat_camera"
echo "Deploying $EXECUTABLE service..."

SOURCE_DIR="/home/agrisat/relay/camera"
OUTPUT_DIR="/home/agrisat/relay/results/camera"

chmod +x $SOURCE_DIR/"camera.sh"
echo "Service executable permissions set"

# wipe the contents of the output directory
rm -rf $OUTPUT_DIR/*
echo "Output directory cleaned"

# Copy the script as a bin file
cp $SOURCE_DIR/$EXECUTABLE /usr/local/bin/$EXECUTABLE

echo "Starting $EXECUTABLE service..."
$EXECUTABLE
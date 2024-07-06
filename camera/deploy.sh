#!/bin/bash

# Name of the service
EXECUTABLE="sat_camera"
echo "Deploying $EXECUTABLE service..."

SOURCE_DIR="/home/agrisat/relay/camera"
OUTPUT_DIR="/home/agrisat/relay/results/camera"

chmod +x $SOURCE_DIR/$EXECUTABLE.sh
echo "Service executable permissions set"

# wipe the contents of the output directory
rm -rf $OUTPUT_DIR/*
echo "Output directory cleaned"

echo "Starting $EXECUTABLE service..."
./"$SOURCE_DIR/$EXECUTABLE".sh
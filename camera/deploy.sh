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

# Symlink the bash script camera.sh to /usr/bin/sat_camera
ln -s $SOURCE_DIR/"camera.sh" /usr/bin/$EXECUTABLE
echo "Service symlinked to /usr/bin/$EXECUTABLE"

echo "Starting $EXECUTABLE service..."
$EXECUTABLE
#!/bin/bash

# Name of the service
EXECUTABLE="sat_agrinet"
echo "Deploying $EXECUTABLE service..."

# Service location (where this script is located)
SOURCE_DIR="/home/agrisat/relay/agrinet"
OUTPUT_DIR="/home/agrisat/relay/results/agrinet"

# create and move so it can be run from anywhere
pyinstaller --onefile --name $EXECUTABLE $SOURCE_DIR/main.py
echo $EXECUTABLE "built"

cp $SOURCE_DIR/dist/$EXECUTABLE /usr/bin/$EXECUTABLE
echo "Service deployed to /usr/bin/$EXECUTABLE"

# wipe the contents of the output directory
rm -rf $OUTPUT_DIR/*
echo "Output directory cleaned"

echo "Starting $EXECUTABLE service..."
$EXECUTABLE
#!/bin/bash

# Name of the service
EXECUTABLE="sat_agrinet"
echo "Deploying $EXECUTABLE service..."

# Service location (where this script is located)
SOURCE_DIR="/Users/adambyrne/Code/relay/agrinet"
OUTPUT_DIR="/Users/adambyrne/Code/relay/results/agrinet"

# create and move so it can be run from anywhere
pyinstaller --onefile --name $EXECUTABLE --paths=. --add-data "utils:utils" $SOURCE_DIR/main.py
echo $EXECUTABLE "built"

# wipe the contents of the output directory
rm -rf $OUTPUT_DIR/*

echo "Starting $EXECUTABLE service..."
./"$EXECUTABLE"


#!/bin/bash

folders=("camera" "agrinet")

for folder in "${folders[@]}"
do 
    chmod +x "$folder/deploy.sh"
    bash "$folder/deploy.sh"
done
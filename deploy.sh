#!/bin/bash

folders=("camera" "agrinet")

for folder in "${folders[@]}"
do 
    bash "$folder/deploy.sh"
done
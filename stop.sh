#!/bin/bash

services=(
    "camera"
    "agrinet"
)

for service in "${services[@]}"; do
    echo "Stopping service: $service"
    sudo systemctl stop "$service"
done

echo "All services stopped."

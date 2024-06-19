#!/bin/bash

services=(
    "ASCamera"
    "ASAgriNet"
    "ASTelemetry"
    "ASOdometry"
)

for service in "${services[@]}"; do
    echo "Stopping service: $service"
    sudo systemctl stop "$service"
done

echo "All services stopped."

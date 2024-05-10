# relay

A lightweight approach to a distributed processing chain for a CanSat mission using the [Raspberry Pi 4B](https://www.raspberrypi.org/) and an array of sensors for data collection and agricultural monitoring.

## Services

Exploiting the built-in systemd service manager, relay uses `.service` files to orchestrate the execution of them, handle dependencies and restart them in case of failure. The services are:

| Service | Description |
| --- | --- |
| `agrinet/` | Inference on camera images using Pix2Pix to generate NIR from RGB |
| `camera/` | Script to take and save still images on a schedule |
| `telemetry/` | Communication with the ground station using a LoRa module |
| `odometry/` | Data collection (altitude, temperature, pressure, gps) |

## Data

All recorded data is persisted in the `data/` directory. The data is structured as follows:

```sh
captures/   # Still images taken by the camera
inference/  # Inference results from the agrinet service
telemetry/  # Telemetry data from the LoRa module
odometry/   # Data collected by the odometry service
```

## Deployment

To deploy the services, run the following commands:

```sh
chmod +x deploy.sh
./deploy.sh # runs each services' ./deploy.sh script
```

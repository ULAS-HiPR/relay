# relay

A lightweight approach to a distributed processing chain for a CanSat mission using the [Raspberry Pi 4B](https://www.raspberrypi.org/) and an array of sensors for data collection and agricultural monitoring.

## Services

Exploiting the built-in systemd service manager, relay uses `.service` files to orchestrate the execution of them, handle dependencies and restart them in case of failure. The services are:

| Service | Description | Stack |
| --- | --- | --- |
| `agrinet/` | Inference on camera images using Pix2Pix to generate NIR from RGB | Python, TensorFlow |
| `camera/` | Script to take and save still images on a ticker | Go, libcamera |
| `telemetry/` | Communication with the ground station using a LoRa module | Python, RPi.GPIO |
| `odometry/` | Data collection (altitude, temperature, pressure, gps) | Python, RPi.GPIO |

## Data

All recorded data is persisted in the `results` directory under a subdir named after the service that generated it. 
If using File IO like in camera, spin off those in goroutines so pictures can be taken every second regardless.

## Deployment

To deploy the services, run the following commands:

```sh
chmod +x deploy.sh
./deploy.sh # runs each services' ./deploy.sh script
```

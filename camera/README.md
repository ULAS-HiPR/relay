# camera

Camera is a service running the libcamera library to capture images from a HQ Raspberry Pi FishEye camera and save them to disk. These images are watched by the agrinet service to generate synthetic NIR images from them. Its a bash script symlinked to `/usr/bin/sat_camera` for easy access.

## Installation

```bash
sudo apt install libcamera libcamera-apps

chmod +x ./deploy.sh 
./deploy.sh # Deploy the service (auto start on deploy)
```

*Note: deploy the service on code change as it will not auto-reload.*

## Usage

```bash
sat_camera # Start the service
killall sat_camera # Stop the service
```

## Debugging

Use `gdb` to attach to the running process and debug it - can be done live or after the fact.

```bash
gdb -p $(pgrep -f sat_camera) # Attach to the running process (named)
```
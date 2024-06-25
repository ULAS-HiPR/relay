# agrinet

AgriNet is a service running the NVIDIA Pix2Pix model against the nirscene0x dataset to generate via a CGAN a synthetic NIR image from a RGB image with the end goal of a more scalable solution to agricultural monitoring.

## Installation

AgriNet is a python package and runs as a PyInstaller executable. The service is deployed via a shell script that will copy the necessary dependencies (single binary) and start the service.

*Note: deploy the service on code change as it will not auto-reload.*

```bash
pip install -r requirements.txt

chmod +x ./deploy.sh
./deploy.sh # Deploy the service (auto start on deploy)
```

## Usage

```bash
sat_agrinet # Start the service
killall sat_agrinet # Stop the service
```

## Debugging

Use `gdb` to attach to the running process and debug it - can be done live or after the fact.

```bash
gdb -p $(pgrep -f sat_agrinet) # Attach to the running process (named)
```

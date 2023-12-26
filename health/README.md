# health

A service for monitoring device vitals and sending them to a RabbitMQ queue `health` written in Rust.

## Payload

An example running on Raspberry Pi Zero W:

```txt
Time: 1703557465
CPU Speed: 997 MHz
Load Average: 1.03, 1.07, 0.87
Memory Info: Total: 439592 KB, Free: 113532 KB, Available: 274484 KB, Buffers: 9692 KB, Cached: 177580 KB
Total Processes: 293
```

## Compilation

Due to hardware limitations, cross compilation is required.
Use the bash script `release.sh` to compile release binaries for the target architecture and `scp` them to the target. You will need to authenticate with the target for `scp` (normal terminal password prompt).

**Requires:** Docker daemon running on the host machine.

*Note: You may need to update the ip address or user in `release.sh` to match your target.*

## Deployment to systemd

Run `deploy.sh` to copy the systemd service file to the target and enable it.

```service
[Unit]
Description=Health Service
After=network.target

[Service]
Type=simple
User=bxrne
WorkingDirectory=/home/bxrne/relay/health
ExecStart=/home/bxrne/relay/health/health
Restart=on-failure

[Install]
WantedBy=multi-user.target
```

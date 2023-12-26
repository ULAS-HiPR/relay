# relay

A processing chain for CanSats to be ran on limited hardware (Raspberry Pi Zero W) in real time for data collection, transmission and CanSat control.

## Getting Started

```bash
git clone https://github.com/theadambyrne/relay.git
cd relay

# See each service for more information
```

## Services

| Service | Description | Language | Docs |
| ------- | ----------- | -------- | ------ |
| health | Device vitals monitoring | Rust | [README](./health/README.md) |

## Communication

Services communicate via RabbitMQ. Which is configured on the target with permissions for remote access via IP address.

## Deployment

Services are deployed to `systemd` via scp. See each service for more information.

## Configuration

Environment variables for URLs, IPs, ports, etc. are stored in `config.toml` and are loaded into the environment at runtime ([see here](config.toml)).

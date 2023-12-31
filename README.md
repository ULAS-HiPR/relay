# relay

Real time processing chain for AgriSat running as microservices via systemd.
AgriSat is a CanSat aboard the Sionna rocket which will conduct visual evaluation via machine learning of land for agricultural use.
Relay adopts a full agnostic approach using RabbitMQ to communicate active data so services can be written in any language.

## Getting Started
```bash
git clone https://github.com/theadambyrne/relay.git
cd relay

# See each service's README for more information
```

## Testing
All services must be suitably unit tested to verify correct behaviour and correct exception handling.
Make sure to create a CI pipeline to run your tests for feature branches. Ensure these are ran on PR and on merge so we can verify a valid system.

## Services

| Service | Description | Language | Details |
| ------- | ----------- | -------- | ------ |
| health | Device vitals monitoring | Rust | [README](./health/README.md) |

> Note: IPs, ports, MQ server details, etc. are stored in `config.toml` and are loaded into the environment at runtime ([see here](config.toml)).


### Releasing to target

For compiled languages `scp` the binary over into its correct location (keep in mind we will clone this repo onto targt device to ensure paths remain valid especially for configuration. In the case of interpreted languages no action other than calling the interpreter in the service execution step is required.

> `release.sh` - remote push the binary to the target device having cross compiled it


### Orchestration (deployment)

Writing a `.service` file is more straight forward than you'd think. You specificy where the program is, what is running it and when to run it. See `health` for more details.

> `deploy.sh` - informing `systemd` of the new service via registration.

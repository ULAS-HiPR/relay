# health

A service for monitoring device vitals and sending them to a RabbitMQ queue `health` written in Rust.

## Payload

```txt
Time: 1703552453
CPU Speed: 2400 MHz
Load Average: 3.42, 7.32, 5.99
Memory Info: Total: 33554432 KB, Free: 1907264 KB, Available: 15391628 KB, Buffers: 0 KB, Cached: 0 KB
Total Processes: 605
```

## Usage

```bash
# Install dependencies
cargo install 

# Build for release
cargo build --release

# Run
cargo run --release
```
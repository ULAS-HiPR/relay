#!/bin/bash -e

TARGET=arm-unknown-linux-gnueabihf
USER=bxrne
PI_IP=192.168.68.121
RUST_VERSION=1.72.0  # Replace with the version you want to use

# Install the Rust toolchain directly
rustup toolchain install $RUST_VERSION --profile minimal

# Set the default toolchain
rustup default $RUST_VERSION

# Install cross if not already installed
cargo install -f cross

# Add to path
export PATH=$HOME/.cargo/bin:$PATH

# Build with the specified target
cross build --release --target $TARGET

# Copy the binary to the target device (Raspberry Pi)
scp -r ./target/$TARGET/release/health $USER@$PI_IP:/home/$USER/relay/health/

scp -r ./target/$TARGET/release/health $USER@$PI_IP:/home/bxrne/relay/health/
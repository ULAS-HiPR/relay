#!/bin/bash -e

TARGET=arm-unknown-linux-gnueabihf # Raspberry Pi Zero W
USER=bxrne
TARGET_IP=192.168.68.121
RUST_VERSION=1.72.0  # Required for cross compilation (cross-rs)

# Install the Rust toolchain directly
rustup toolchain install $RUST_VERSION --profile minimal

# Set the default toolchain
rustup default $RUST_VERSION

# Install cross if not already installed
cargo install -f cross

# Add to path
export PATH=$HOME/.cargo/bin:$PATH

# Build with the specified target for release
cross build --release --target $TARGET

# Copy the binary to the target device
scp -r ./target/$TARGET/release/health $USER@$TARGET_IP:/home/bxrne/relay/health/
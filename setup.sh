#!/bin/bash

# Setup script for CloneMine
echo "Setting up CloneMine development environment..."

# Check if git is installed
if ! command -v git &> /dev/null; then
    echo "Error: git is not installed"
    exit 1
fi

# Initialize submodules
echo "Initializing git submodules..."
git submodule update --init --recursive

# Check if CMake is installed
if ! command -v cmake &> /dev/null; then
    echo "Error: CMake is not installed"
    echo "Please install CMake 3.20 or higher"
    exit 1
fi

# Check if Vulkan SDK is installed
if [ -z "$VULKAN_SDK" ]; then
    echo "Warning: VULKAN_SDK environment variable is not set"
    echo "Please install the Vulkan SDK from https://vulkan.lunarg.com/"
    echo "Or on Linux, install vulkan-sdk package"
fi

echo "Setup complete!"
echo "To build:"
echo "  mkdir build && cd build"
echo "  cmake .."
echo "  cmake --build ."

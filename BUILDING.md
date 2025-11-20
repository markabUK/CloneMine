# Building CloneMine

This document provides detailed build instructions for CloneMine across different platforms.

## Prerequisites

### All Platforms

1. **CMake 3.20+**: Download from https://cmake.org/download/
2. **Git**: For cloning and managing submodules
3. **Vulkan SDK 1.2+**: Download from https://vulkan.lunarg.com/

### Linux

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install cmake g++ git libvulkan-dev vulkan-tools

# Fedora/RHEL
sudo dnf install cmake gcc-c++ git vulkan-devel vulkan-tools

# Arch Linux
sudo pacman -S cmake gcc git vulkan-devel vulkan-tools
```

### Windows

1. Install Visual Studio 2019 or later with C++ support
2. Install CMake from https://cmake.org/download/
3. Install Vulkan SDK from https://vulkan.lunarg.com/
4. Install Git from https://git-scm.com/

### macOS

```bash
# Install Xcode Command Line Tools
xcode-select --install

# Install CMake and other tools via Homebrew
brew install cmake git

# Install Vulkan SDK
# Download MoltenVK from https://vulkan.lunarg.com/
```

## Build Steps

### 1. Clone Repository

```bash
git clone https://github.com/markabUK/CloneMine.git
cd CloneMine
```

### 2. Initialize Dependencies

```bash
# Linux/macOS
chmod +x setup.sh
./setup.sh

# Windows
setup.bat
```

Or manually:

```bash
git submodule update --init --recursive
```

### 3. Configure and Build

#### Linux/macOS

```bash
mkdir build
cd build
cmake ..
cmake --build . -j$(nproc)
```

#### Windows (Visual Studio)

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

Or open the generated `.sln` file in Visual Studio.

## Build Options

You can customize the build with CMake options:

```bash
# Debug build
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build
cmake -DCMAKE_BUILD_TYPE=Release ..

# Specify compiler
cmake -DCMAKE_CXX_COMPILER=clang++ ..

# Specify Vulkan SDK path (if not in default location)
cmake -DVULKAN_SDK=/path/to/vulkan/sdk ..
```

## Troubleshooting

### Vulkan SDK Not Found

**Linux**: Install the Vulkan development packages:
```bash
sudo apt-get install libvulkan-dev vulkan-tools
```

**Windows/macOS**: Ensure the `VULKAN_SDK` environment variable is set:
```bash
# Check if set
echo $VULKAN_SDK  # Linux/macOS
echo %VULKAN_SDK%  # Windows
```

### Submodule Issues

If dependencies are missing:
```bash
git submodule update --init --recursive --force
```

### Shader Compilation Fails

Ensure `glslc` (SPIR-V compiler) is in your PATH. It comes with the Vulkan SDK:

```bash
# Check if glslc is available
glslc --version

# If not found, add Vulkan SDK bin to PATH
export PATH=$VULKAN_SDK/bin:$PATH  # Linux/macOS
set PATH=%VULKAN_SDK%\Bin;%PATH%  # Windows
```

### C++20 Not Supported

Ensure you have a recent compiler:
- GCC 10+
- Clang 11+
- MSVC 2019 16.8+

Check your compiler version:
```bash
g++ --version
clang++ --version
```

## Running

After building successfully:

```bash
# From build directory
./CloneMine  # Linux/macOS
.\Release\CloneMine.exe  # Windows
```

## Development Build

For development with debug symbols and validation layers:

```bash
mkdir build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build .
```

Debug builds enable:
- Vulkan validation layers
- Debug symbols
- Additional runtime checks

## Clean Build

To start fresh:

```bash
rm -rf build
mkdir build
cd build
cmake ..
cmake --build .
```

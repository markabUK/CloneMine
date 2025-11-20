# Quick Start Guide

Get CloneMine up and running in minutes!

## Prerequisites

Before you begin, ensure you have:

1. **Vulkan SDK** - Download from https://vulkan.lunarg.com/
2. **CMake 3.20+** - Download from https://cmake.org/
3. **C++20 Compiler**:
   - Linux: GCC 10+ or Clang 11+
   - Windows: Visual Studio 2019+
   - macOS: Xcode 12+
4. **Git** - For cloning and submodules

## Installation Steps

### 1. Clone the Repository

```bash
git clone https://github.com/markabUK/CloneMine.git
cd CloneMine
```

### 2. Run Setup Script

This initializes the external dependencies:

```bash
# Linux/macOS
./setup.sh

# Windows
setup.bat
```

### 3. Build the Project

```bash
# Create build directory
mkdir build
cd build

# Configure
cmake ..

# Build
cmake --build .
```

On Windows with Visual Studio, you can also open the generated `.sln` file.

### 4. Run CloneMine

```bash
# From the build directory
./CloneMine         # Linux/macOS
.\CloneMine.exe     # Windows (Debug)
.\Release\CloneMine.exe  # Windows (Release)
```

## Troubleshooting

### "Vulkan SDK not found"

**Solution**: Install the Vulkan SDK and ensure `VULKAN_SDK` environment variable is set.

```bash
# Linux
export VULKAN_SDK=/path/to/vulkan/sdk

# Windows
set VULKAN_SDK=C:\VulkanSDK\1.x.x.x
```

### "Submodules not initialized"

**Solution**: Initialize git submodules:

```bash
git submodule update --init --recursive
```

### "Shader compilation failed"

**Solution**: Ensure `glslc` is in your PATH (comes with Vulkan SDK):

```bash
# Test if glslc is available
glslc --version
```

## Creating Your First Plugin

1. Create a new `.lua` file in the `plugins/` directory:

```lua
-- plugins/my_plugin.lua

function onLoad()
    log("My plugin loaded successfully!")
end

function onUpdate(deltaTime)
    -- Called every frame
    -- deltaTime is in seconds
end
```

2. Restart CloneMine - your plugin will be loaded automatically!

## Next Steps

- Read [README.md](README.md) for detailed information
- Check [BUILDING.md](BUILDING.md) for advanced build options
- Review [SECURITY.md](SECURITY.md) to understand plugin security
- Explore [IMPLEMENTATION.md](IMPLEMENTATION.md) for architecture details
- Look at `plugins/example.lua` for API examples

## Platform-Specific Notes

### Linux

Install development packages:
```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake libvulkan-dev vulkan-tools

# Fedora
sudo dnf install cmake gcc-c++ vulkan-devel vulkan-tools
```

### Windows

- Use Visual Studio 2019 or later
- Install Vulkan SDK with all components
- Run CMake from Visual Studio Developer Command Prompt

### macOS

- Install Xcode Command Line Tools
- Download MoltenVK (Vulkan for macOS) from Vulkan SDK
- Use Homebrew for CMake: `brew install cmake`

## Getting Help

If you encounter issues:

1. Check the troubleshooting section above
2. Review the detailed build guide in [BUILDING.md](BUILDING.md)
3. Ensure all prerequisites are installed correctly
4. Verify your system meets the requirements

## System Requirements

**Minimum:**
- Vulkan-capable GPU
- 4 GB RAM
- 500 MB disk space

**Recommended:**
- Modern GPU with Vulkan 1.2+ support
- 8 GB RAM
- 1 GB disk space (for build artifacts)

## Success!

Once running, you should see a window with a colored triangle. This confirms:
- ✅ Vulkan is working
- ✅ Shaders compiled successfully
- ✅ Window system initialized
- ✅ Plugins loaded

Happy voxel gaming! 🎮

# CI/CD Pipeline

CloneMine uses GitHub Actions for continuous integration and deployment.

## Workflows

### Build Pipeline

**File**: `.github/workflows/build.yml`

The build pipeline automatically builds the project on:
- Every push to `main` and `copilot/**` branches
- Every pull request to `main`

#### Platforms

**Linux (Ubuntu Latest)**
- Installs system dependencies:
  - Vulkan SDK and development libraries
  - Wayland and X11 support for GLFW
  - CMake and build tools
- Configures and builds with CMake
- Uploads executable as artifact

**Windows (Latest)**
- Uses `humbletim/setup-vulkan-sdk` GitHub Action for reliable Vulkan SDK installation
- Configures and builds with CMake (Release configuration)
- Uploads executable as artifact

#### Build Status

Check the build status badge at the top of the README:

[![Build](https://github.com/markabUK/CloneMine/actions/workflows/build.yml/badge.svg)](https://github.com/markabUK/CloneMine/actions/workflows/build.yml)

#### Artifacts

After each successful build, compiled executables are available as artifacts:
- `clonemine-linux` - Linux executable
- `clonemine-windows` - Windows executable

To download artifacts:
1. Go to the [Actions tab](https://github.com/markabUK/CloneMine/actions)
2. Click on a successful workflow run
3. Scroll down to "Artifacts" section
4. Download the artifact for your platform

## Local Testing

To test the build locally before pushing:

### Linux
```bash
# Install dependencies
sudo apt-get install cmake g++ libvulkan-dev vulkan-tools

# Build
./setup.sh
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . -j$(nproc)
```

### Windows
```powershell
# Install Vulkan SDK from https://vulkan.lunarg.com/
# Install Visual Studio with C++ support

# Build
.\setup.bat
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Troubleshooting

### Linux Build Failures

**Missing Vulkan SDK**
```bash
sudo apt-get install libvulkan-dev vulkan-tools
```

**Missing X11 dependencies**
```bash
sudo apt-get install libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev
```

### Windows Build Failures

**Vulkan SDK not found**
- Ensure `VULKAN_SDK` environment variable is set
- Download SDK from https://vulkan.lunarg.com/

**CMake configuration fails**
- Use Visual Studio 2019 or later
- Install "Desktop development with C++" workload

## Future Enhancements

Planned CI/CD improvements:
- [ ] Automated testing
- [ ] Code coverage reports
- [ ] Release automation
- [ ] Multi-architecture builds (ARM64)
- [ ] macOS build support
- [ ] Shader validation step
- [ ] Static analysis integration

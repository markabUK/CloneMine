# CloneMine

[![Build](https://github.com/markabUK/CloneMine/actions/workflows/build.yml/badge.svg)](https://github.com/markabUK/CloneMine/actions/workflows/build.yml)

A Minecraft-inspired voxel game engine built with modern C++20, featuring Vulkan rendering, secure Lua plugin support, and cross-platform compatibility.

## Features

- **Modern C++20**: Utilizes the latest C++ features with memory-safe practices
- **Vulkan Rendering**: Cross-platform graphics using Vulkan API
- **Secure Plugin System**: Sandboxed Lua plugins with restricted API access
- **Voxel-Based World**: Chunk-based terrain generation and management
- **CMake Build System**: Easy to build and configure

## Requirements

### Build Dependencies
- CMake 3.20 or higher
- C++20 compatible compiler (GCC 10+, Clang 11+, MSVC 2019+)
- Vulkan SDK 1.2 or higher
- Git (for submodule dependencies)

### Runtime Dependencies
- Vulkan-capable GPU and drivers

## Building

### Clone the Repository

```bash
git clone --recursive https://github.com/markabUK/CloneMine.git
cd CloneMine
```

If you already cloned without `--recursive`, initialize submodules:

```bash
git submodule update --init --recursive
```

### Build on Linux/macOS

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Build on Windows

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Running

After building, run the executable from the build directory:

```bash
./CloneMine  # Linux/macOS
.\CloneMine.exe  # Windows
```

## Plugin System

CloneMine features a secure plugin system using sandboxed Lua scripts. Plugins cannot access the file system, execute system commands, or perform other dangerous operations.

### Creating a Plugin

Create a `.lua` file in the `plugins/` directory:

```lua
-- myplugin.lua

function onLoad()
    log("My plugin loaded!")
end

function onUpdate(deltaTime)
    -- Called every frame
end

function onInput()
    -- Handle input events
end

function onRender()
    -- Called during rendering
end
```

### Safe Plugin API

Plugins have access to the following safe functions:

- `log(message)` - Print a message to the console
- `getBlockType(x, y, z)` - Get the block type at coordinates
- `setBlockType(x, y, z, type)` - Set the block type at coordinates

### Security Features

The plugin system implements the following security measures:

1. **Sandboxed Execution**: Each plugin runs in its own Lua state
2. **Restricted API**: Dangerous Lua functions are removed:
   - File I/O operations (`io`, `dofile`, `loadfile`)
   - System commands (`os.execute`, `os.exit`)
   - Debug library access
   - Package loading capabilities
3. **Controlled Access**: Plugins can only interact with the game through the provided API

## Architecture

### Project Structure

```
CloneMine/
├── src/
│   ├── core/           # Application and window management
│   ├── rendering/      # Vulkan rendering system
│   ├── world/          # Voxel world and chunk management
│   └── plugin/         # Lua plugin system
├── shaders/            # GLSL shader files
├── plugins/            # Lua plugin scripts
└── external/           # Third-party dependencies
    ├── glfw/           # Window and input handling
    ├── glm/            # Mathematics library
    └── lua/            # Lua scripting engine
```

### Memory Safety

The codebase follows modern C++ memory safety practices:

- **Smart Pointers**: Extensive use of `std::unique_ptr` and `std::shared_ptr`
- **RAII**: Resource management through constructors/destructors
- **No Raw Pointers**: Ownership is always clear
- **Move Semantics**: Efficient resource transfer without copying
- **Delete Copy Constructors**: Prevention of accidental copies where inappropriate

## Development

### Code Style

- C++20 standard
- RAII for resource management
- `[[nodiscard]]` for functions returning important values
- `noexcept` for non-throwing functions
- `const` correctness throughout

### Adding Features

1. Follow the existing project structure
2. Use smart pointers for memory management
3. Ensure all resources are properly cleaned up
4. Test with validation layers enabled (Debug builds)

## License

This project is open source. See LICENSE file for details.

## Contributing

Contributions are welcome! Please ensure:

1. Code follows C++20 standards
2. Memory safety practices are maintained
3. Plugin API security is not compromised
4. Changes are tested on multiple platforms

## Acknowledgments

- GLFW - Window and input handling
- GLM - Mathematics library
- Lua - Scripting language
- Vulkan - Graphics API
# External Dependencies

This directory contains third-party libraries used by CloneMine.

## Libraries

### GLFW (3.x)
- **Purpose**: Window creation and input handling
- **License**: Zlib
- **Repository**: https://github.com/glfw/glfw
- **Website**: https://www.glfw.org/

GLFW provides cross-platform window management, input handling, and OpenGL/Vulkan context creation.

### GLM (0.9.x)
- **Purpose**: Mathematics library for graphics
- **License**: MIT
- **Repository**: https://github.com/g-truc/glm
- **Website**: https://glm.g-truc.net/

GLM is a header-only C++ mathematics library based on the OpenGL Shading Language (GLSL) specifications.

### Lua (5.4.x)
- **Purpose**: Scripting language for plugins
- **License**: MIT
- **Repository**: https://github.com/lua/lua
- **Website**: https://www.lua.org/

Lua is a powerful, efficient, lightweight, embeddable scripting language. It's used for the plugin system with security sandboxing.

## Initialization

These dependencies are managed as Git submodules. To initialize them:

```bash
git submodule update --init --recursive
```

Or use the provided setup scripts:
```bash
./setup.sh      # Linux/macOS
setup.bat       # Windows
```

## Building

The dependencies are automatically built as part of the main CMake build process. No manual building is required.

## Updating Dependencies

To update all submodules to their latest versions:

```bash
git submodule update --remote --merge
```

To update a specific dependency:

```bash
cd external/glfw
git pull origin master
cd ../..
git add external/glfw
git commit -m "Update GLFW to latest version"
```

## License Compliance

All dependencies use permissive licenses (MIT/Zlib) that allow:
- Commercial use
- Modification
- Distribution
- Private use

Please review each library's LICENSE file for specific terms.

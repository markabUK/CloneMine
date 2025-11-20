@echo off
REM Setup script for CloneMine on Windows

echo Setting up CloneMine development environment...

REM Check if git is installed
git --version >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: git is not installed
    exit /b 1
)

REM Initialize submodules
echo Initializing git submodules...
git submodule update --init --recursive

REM Check if CMake is installed
cmake --version >nul 2>&1
if %errorlevel% neq 0 (
    echo Error: CMake is not installed
    echo Please install CMake 3.20 or higher
    exit /b 1
)

REM Check if Vulkan SDK is installed
if "%VULKAN_SDK%"=="" (
    echo Warning: VULKAN_SDK environment variable is not set
    echo Please install the Vulkan SDK from https://vulkan.lunarg.com/
)

echo Setup complete!
echo To build:
echo   mkdir build
echo   cd build
echo   cmake ..
echo   cmake --build . --config Release

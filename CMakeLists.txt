cmake_minimum_required(VERSION 3.20)
project(CloneMine VERSION 1.0.0 LANGUAGES C CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable strict warnings for memory safety (only for our code)
if(MSVC)
    set(CLONEMINE_COMPILE_OPTIONS /W4 /WX)
else()
    set(CLONEMINE_COMPILE_OPTIONS -Wall -Wextra -Wpedantic -Werror)
endif()

# Find required packages
find_package(Vulkan REQUIRED)

# Add subdirectories (external libraries build without strict warnings)
add_subdirectory(external)
add_subdirectory(src)

# Install targets
install(TARGETS CloneMine CloneMineClient CloneMineServer
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install plugin examples
install(DIRECTORY plugins/
    DESTINATION share/clonemine/plugins
    FILES_MATCHING PATTERN "*.lua"
)

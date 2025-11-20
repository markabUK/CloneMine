cmake_minimum_required(VERSION 3.20)
project(CloneMine VERSION 1.0.0 LANGUAGES CXX)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable strict warnings for memory safety
if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Find required packages
find_package(Vulkan REQUIRED)

# Add subdirectories
add_subdirectory(external)
add_subdirectory(src)

# Install targets
install(TARGETS CloneMine
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install plugin examples
install(DIRECTORY plugins/
    DESTINATION share/clonemine/plugins
    FILES_MATCHING PATTERN "*.lua"
)

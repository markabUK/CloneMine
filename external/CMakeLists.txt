# External dependencies

# Check if dependencies exist
if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glfw/CMakeLists.txt)
    message(WARNING "GLFW not found. Client build will be disabled. Please run: git submodule update --init --recursive")
    set(BUILD_CLIENT OFF PARENT_SCOPE)
endif()

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glm/CMakeLists.txt)
    message(FATAL_ERROR "GLM not found. Please run: git submodule update --init --recursive")
endif()

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lua)
    message(FATAL_ERROR "Lua not found. Please run: git submodule update --init --recursive")
endif()

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/asio)
    message(FATAL_ERROR "ASIO not found. Please run: git submodule update --init --recursive")
endif()

# GLM for math operations (header-only)
add_library(glm INTERFACE)
target_include_directories(glm INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/glm)

# ASIO for networking (header-only)
add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/asio/asio/include)
target_compile_definitions(asio INTERFACE ASIO_STANDALONE)

# GLFW for window management (optional)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/glfw/CMakeLists.txt AND Vulkan_FOUND)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    add_subdirectory(glfw)
endif()

# Lua library
# Build Lua as a static library
set(LUA_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lua)
file(GLOB LUA_SOURCES
    ${LUA_SRC_DIR}/*.c
)
# Remove lua.c, luac.c, and onelua.c (contains duplicate symbols)
list(REMOVE_ITEM LUA_SOURCES 
    ${LUA_SRC_DIR}/lua.c 
    ${LUA_SRC_DIR}/luac.c
    ${LUA_SRC_DIR}/onelua.c
)

add_library(lua_static STATIC ${LUA_SOURCES})
target_include_directories(lua_static PUBLIC ${LUA_SRC_DIR})
set_target_properties(lua_static PROPERTIES 
    COMPILE_FLAGS "-DLUA_USE_POSIX"
    POSITION_INDEPENDENT_CODE ON
)

# Export targets
add_library(external_libs INTERFACE)
if(Vulkan_FOUND AND TARGET glfw)
    target_link_libraries(external_libs INTERFACE
        glm
        glfw
        lua_static
        Vulkan::Vulkan
    )
else()
    target_link_libraries(external_libs INTERFACE
        glm
        lua_static
    )
endif()

# Export networking-only libs for server
add_library(external_libs_server INTERFACE)
target_link_libraries(external_libs_server INTERFACE
    glm
    lua_static
    asio
)

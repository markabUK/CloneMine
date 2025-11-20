# Source files
set(SOURCES
    main.cpp
    core/Application.cpp
    core/Window.cpp
    rendering/VulkanContext.cpp
    rendering/VulkanDevice.cpp
    rendering/VulkanSwapchain.cpp
    rendering/VulkanPipeline.cpp
    rendering/VulkanBuffer.cpp
    rendering/Renderer.cpp
    world/Chunk.cpp
    world/World.cpp
    world/Block.cpp
    plugin/PluginManager.cpp
    plugin/LuaSandbox.cpp
)

set(HEADERS
    core/Application.h
    core/Window.h
    rendering/VulkanContext.h
    rendering/VulkanDevice.h
    rendering/VulkanSwapchain.h
    rendering/VulkanPipeline.h
    rendering/VulkanBuffer.h
    rendering/Renderer.h
    world/Chunk.h
    world/World.h
    world/Block.h
    plugin/PluginManager.h
    plugin/LuaSandbox.h
    plugin/PluginAPI.h
)

# Create executable
add_executable(CloneMine ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(CloneMine PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/lua/src
)

# Link libraries
target_link_libraries(CloneMine PRIVATE external_libs)

# Compile shader files
find_program(GLSLC glslc HINTS $ENV{VULKAN_SDK}/bin)
if(GLSLC)
    set(SHADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../shaders)
    set(SHADER_OUTPUT_DIR ${CMAKE_BINARY_DIR}/shaders)
    
    file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})
    
    file(GLOB VERTEX_SHADERS ${SHADER_DIR}/*.vert)
    file(GLOB FRAGMENT_SHADERS ${SHADER_DIR}/*.frag)
    
    foreach(SHADER ${VERTEX_SHADERS} ${FRAGMENT_SHADERS})
        get_filename_component(SHADER_NAME ${SHADER} NAME)
        set(SPIRV ${SHADER_OUTPUT_DIR}/${SHADER_NAME}.spv)
        add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${GLSLC} ${SHADER} -o ${SPIRV}
            DEPENDS ${SHADER}
        )
        list(APPEND SPIRV_SHADERS ${SPIRV})
    endforeach()
    
    add_custom_target(shaders ALL DEPENDS ${SPIRV_SHADERS})
    add_dependencies(CloneMine shaders)
endif()

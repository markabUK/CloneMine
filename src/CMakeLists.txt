# Common source files shared between client and server
set(COMMON_SOURCES
    world/Chunk.cpp
    world/World.cpp
    world/Block.cpp
    world/Player.cpp
    plugin/PluginManager.cpp
    plugin/LuaSandbox.cpp
    plugin/PluginAPI.cpp
    network/NetworkMessage.cpp
    network/PacketEncryption.cpp
    combat/DamageCalculation.cpp
)

set(COMMON_HEADERS
    world/Chunk.h
    world/World.h
    world/Block.h
    world/Player.h
    plugin/PluginManager.h
    plugin/LuaSandbox.h
    plugin/PluginAPI.h
    network/NetworkMessage.h
    network/PacketEncryption.h
    combat/DamageCalculation.h
    combat/DamageTypes.h
)

# Client-specific source files
set(CLIENT_SOURCES
    client_main.cpp
    client/ClientApplication.cpp
    client/NetworkClient.cpp
    core/Application.cpp
    core/Window.cpp
    core/InputManager.cpp
    rendering/VulkanContext.cpp
    rendering/VulkanDevice.cpp
    rendering/VulkanSwapchain.cpp
    rendering/VulkanPipeline.cpp
    rendering/VulkanBuffer.cpp
    rendering/Renderer.cpp
)

set(CLIENT_HEADERS
    client/ClientApplication.h
    client/NetworkClient.h
    core/Application.h
    core/Window.h
    core/InputManager.h
    rendering/VulkanContext.h
    rendering/VulkanDevice.h
    rendering/VulkanSwapchain.h
    rendering/VulkanPipeline.h
    rendering/VulkanBuffer.h
    rendering/Renderer.h
)

# Server-specific source files
set(SERVER_SOURCES
    server_main.cpp
    server/GameServer.cpp
    server/ServerPlayer.cpp
)

set(SERVER_HEADERS
    server/GameServer.h
    server/ServerPlayer.h
)

# Always create server executable (no Vulkan dependency)
add_executable(CloneMineServer ${SERVER_SOURCES} ${SERVER_HEADERS} ${COMMON_SOURCES} ${COMMON_HEADERS})

# Apply strict warnings to server
target_compile_options(CloneMineServer PRIVATE ${CLONEMINE_COMPILE_OPTIONS})

# Include directories for server
target_include_directories(CloneMineServer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/lua/src
)

# Link libraries for server (no Vulkan or GLFW needed)
target_link_libraries(CloneMineServer PRIVATE external_libs_server)

# Only create client executable if Vulkan is available
if(Vulkan_FOUND)
    # Create client executable
    add_executable(CloneMineClient ${CLIENT_SOURCES} ${CLIENT_HEADERS} ${COMMON_SOURCES} ${COMMON_HEADERS})

    # Apply strict warnings to client
    target_compile_options(CloneMineClient PRIVATE ${CLONEMINE_COMPILE_OPTIONS})

    # Include directories for client
    target_include_directories(CloneMineClient PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/lua/src
    )

    # Link libraries for client
    target_link_libraries(CloneMineClient PRIVATE external_libs asio)
    
    # Keep the original CloneMine executable for backward compatibility (single-player mode)
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

    # Create original executable
    add_executable(CloneMine ${SOURCES} ${HEADERS})

    # Apply strict warnings to our code
    target_compile_options(CloneMine PRIVATE ${CLONEMINE_COMPILE_OPTIONS})

    # Include directories
    target_include_directories(CloneMine PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../external/lua/src
    )

    # Link libraries
    target_link_libraries(CloneMine PRIVATE external_libs)
endif()

# Compile shader files
if(Vulkan_FOUND)
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
        add_dependencies(CloneMineClient shaders)
    endif()
endif()

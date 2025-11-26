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
    network/PacketValidator.cpp
    combat/DamageCalculation.cpp
    audio/AudioManager.cpp
    scripting/ScriptedScene.cpp
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
    network/PacketValidator.h
    combat/DamageCalculation.h
    combat/DamageTypes.h
    quest/QuestData.h
    character/CharacterData.h
    audio/AudioManager.h
    scripting/ScriptedScene.h
    config/ServerConfig.h
)

# Client-specific source files
set(CLIENT_SOURCES
    client_main.cpp
    client/ClientApplication.cpp
    client/NetworkClient.cpp
    client/LoginScreen.cpp
    client/CharacterSelectScreen.cpp
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
    client/LoginScreen.h
    client/CharacterSelectScreen.h
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

# Chat server source files
set(CHAT_SERVER_SOURCES
    chat_server_main.cpp
    server/ChatServer.cpp
)

set(CHAT_SERVER_HEADERS
    server/ChatServer.h
)

# Quest server source files
set(QUEST_SERVER_SOURCES
    quest_server_main.cpp
    server/QuestServer.cpp
)

set(QUEST_SERVER_HEADERS
    server/QuestServer.h
)

# Login server source files
set(LOGIN_SERVER_SOURCES
    login_server_main.cpp
    server/LoginServer.cpp
)

set(LOGIN_SERVER_HEADERS
    server/LoginServer.h
)

# Character server source files
set(CHARACTER_SERVER_SOURCES
    character_server_main.cpp
    server/CharacterServer.cpp
)

set(CHARACTER_SERVER_HEADERS
    server/CharacterServer.h
)

# Always create server executables (no Vulkan dependency)
add_executable(CloneMineServer ${SERVER_SOURCES} ${SERVER_HEADERS} ${COMMON_SOURCES} ${COMMON_HEADERS})
add_executable(CloneMineChatServer ${CHAT_SERVER_SOURCES} ${CHAT_SERVER_HEADERS} ${COMMON_SOURCES} ${COMMON_HEADERS})
add_executable(CloneMineQuestServer ${QUEST_SERVER_SOURCES} ${QUEST_SERVER_HEADERS} ${COMMON_SOURCES} ${COMMON_HEADERS})
add_executable(CloneMineLoginServer ${LOGIN_SERVER_SOURCES} ${LOGIN_SERVER_HEADERS} ${COMMON_SOURCES} ${COMMON_HEADERS})
add_executable(CloneMineCharacterServer ${CHARACTER_SERVER_SOURCES} ${CHARACTER_SERVER_HEADERS} ${COMMON_SOURCES} ${COMMON_HEADERS})

# Apply strict warnings to servers
target_compile_options(CloneMineServer PRIVATE ${CLONEMINE_COMPILE_OPTIONS})
target_compile_options(CloneMineChatServer PRIVATE ${CLONEMINE_COMPILE_OPTIONS})
target_compile_options(CloneMineQuestServer PRIVATE ${CLONEMINE_COMPILE_OPTIONS})
target_compile_options(CloneMineLoginServer PRIVATE ${CLONEMINE_COMPILE_OPTIONS})
target_compile_options(CloneMineCharacterServer PRIVATE ${CLONEMINE_COMPILE_OPTIONS})

# Include directories for servers
target_include_directories(CloneMineServer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/lua/src
)
target_include_directories(CloneMineChatServer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/lua/src
)
target_include_directories(CloneMineQuestServer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/lua/src
)
target_include_directories(CloneMineLoginServer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/lua/src
)
target_include_directories(CloneMineCharacterServer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../external/lua/src
)

# Link libraries for servers (no Vulkan or GLFW needed)
target_link_libraries(CloneMineServer PRIVATE external_libs_server)
target_link_libraries(CloneMineChatServer PRIVATE external_libs_server)
target_link_libraries(CloneMineQuestServer PRIVATE external_libs_server)
target_link_libraries(CloneMineLoginServer PRIVATE external_libs_server)
target_link_libraries(CloneMineCharacterServer PRIVATE external_libs_server)

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

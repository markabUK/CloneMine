#include "ClientApplication.h"
#include "../network/NetworkMessage.h"
#include <chrono>
#include <iostream>

namespace clonemine {
namespace client {

ClientApplication::ClientApplication(std::string_view title, uint32_t width, uint32_t height)
    : m_window(std::make_unique<Window>(title, width, height))
    , m_renderer(std::make_unique<Renderer>(*m_window))
    , m_world(std::make_unique<World>())
    , m_pluginManager(std::make_unique<PluginManager>())
    , m_networkClient(std::make_unique<NetworkClient>())
{
    // Load plugins from the plugins directory
    m_pluginManager->loadPluginsFromDirectory("plugins");
    m_pluginManager->initializePlugins();
    
    // Set network message callback
    m_networkClient->setMessageCallback([this](const std::vector<uint8_t>& data) {
        handleNetworkMessage(data);
    });
}

ClientApplication::~ClientApplication() = default;

bool ClientApplication::connectToServer(const std::string& host, uint16_t port, const std::string& playerName) {
    return m_networkClient->connect(host, port, playerName);
}

void ClientApplication::run() {
    using clock = std::chrono::high_resolution_clock;
    auto lastTime = clock::now();

    while (m_running && !m_window->shouldClose()) {
        auto currentTime = clock::now();
        auto deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        processInput();
        update(deltaTime);
        render();
        
        m_window->pollEvents();
    }

    m_renderer->waitIdle();
    m_networkClient->disconnect();
}

void ClientApplication::shutdown() {
    m_running = false;
}

void ClientApplication::processInput() {
    // Input processing will be handled here
    // Plugin callbacks for input
    m_pluginManager->callHook("onInput");
    
    // TODO: Capture actual input from window
    // For now, just update local player prediction
}

void ClientApplication::update(float deltaTime) {
    // Process network messages
    m_networkClient->processMessages();
    
    // Update local prediction
    m_localPlayer.update(deltaTime);
    
    // Update remote players
    for (auto& [id, player] : m_remotePlayers) {
        player.update(deltaTime);
    }
    
    // Update world (client-side prediction)
    m_world->update(deltaTime);
    
    // Send player input to server
    m_inputSendTimer += deltaTime;
    if (m_inputSendTimer >= INPUT_SEND_RATE) {
        m_inputSendTimer = 0.0f;
        sendPlayerInput();
    }
    
    // Plugin update callbacks
    m_pluginManager->callHook("onUpdate", deltaTime);
}

void ClientApplication::render() {
    if (m_window->wasResized()) {
        m_renderer->handleResize();
        m_window->resetResizedFlag();
    }

    m_renderer->beginFrame();
    m_renderer->renderWorld(*m_world);
    m_renderer->endFrame();
    
    // Plugin render callbacks
    m_pluginManager->callHook("onRender");
}

void ClientApplication::handleNetworkMessage(const std::vector<uint8_t>& data) {
    if (data.empty()) {
        return;
    }
    
    network::MessageType type = static_cast<network::MessageType>(data[0]);
    
    switch (type) {
        case network::MessageType::PLAYER_STATE_UPDATE: {
            if (data.size() < 50) break;
            
            // Parse player state update
            uint32_t playerId = data[1] | (data[2] << 8) | (data[3] << 16) | (data[4] << 24);
            
            // Read position (3 floats)
            float px, py, pz;
            std::memcpy(&px, &data[5], sizeof(float));
            std::memcpy(&py, &data[9], sizeof(float));
            std::memcpy(&pz, &data[13], sizeof(float));
            
            // Read velocity (3 floats)
            float vx, vy, vz;
            std::memcpy(&vx, &data[17], sizeof(float));
            std::memcpy(&vy, &data[21], sizeof(float));
            std::memcpy(&vz, &data[25], sizeof(float));
            
            // Read rotation (2 floats)
            float yaw, pitch;
            std::memcpy(&yaw, &data[29], sizeof(float));
            std::memcpy(&pitch, &data[33], sizeof(float));
            
            // Read health and resource (2 floats)
            float health, resource;
            std::memcpy(&health, &data[37], sizeof(float));
            std::memcpy(&resource, &data[41], sizeof(float));
            
            if (playerId == m_networkClient->getPlayerId()) {
                // Update local player with authoritative state
                m_localPlayer.setPosition(glm::vec3(px, py, pz));
                m_localPlayer.setRotation(yaw, pitch);
                m_localPlayer.setHealth(health);
                m_localPlayer.setResource(resource);
            } else {
                // Update or create remote player
                auto it = m_remotePlayers.find(playerId);
                if (it == m_remotePlayers.end()) {
                    m_remotePlayers[playerId] = Player();
                }
                
                m_remotePlayers[playerId].setPosition(glm::vec3(px, py, pz));
                m_remotePlayers[playerId].setRotation(yaw, pitch);
                m_remotePlayers[playerId].setHealth(health);
                m_remotePlayers[playerId].setResource(resource);
            }
            break;
        }
        
        case network::MessageType::PLAYER_SPAWN: {
            if (data.size() < 20) break;
            
            uint32_t playerId = data[1] | (data[2] << 8) | (data[3] << 16) | (data[4] << 24);
            
            // Parse spawn position
            float px, py, pz;
            std::memcpy(&px, &data[5], sizeof(float));
            std::memcpy(&py, &data[9], sizeof(float));
            std::memcpy(&pz, &data[13], sizeof(float));
            
            if (playerId != m_networkClient->getPlayerId()) {
                // Add remote player
                m_remotePlayers[playerId] = Player();
                m_remotePlayers[playerId].setPosition(glm::vec3(px, py, pz));
                
                std::cout << "Remote player " << playerId << " spawned" << std::endl;
            }
            break;
        }
        
        case network::MessageType::PLAYER_DESPAWN: {
            if (data.size() < 5) break;
            
            uint32_t playerId = data[1] | (data[2] << 8) | (data[3] << 16) | (data[4] << 24);
            
            auto it = m_remotePlayers.find(playerId);
            if (it != m_remotePlayers.end()) {
                m_remotePlayers.erase(it);
                std::cout << "Remote player " << playerId << " despawned" << std::endl;
            }
            break;
        }
        
        default:
            // Unknown message type
            break;
    }
}

void ClientApplication::sendPlayerInput() {
    if (!m_networkClient->isConnected()) {
        return;
    }
    
    // Create input message
    network::PlayerInput input;
    input.playerId = m_networkClient->getPlayerId();
    
    // TODO: Get actual input from window/input system
    // For now, send default values
    input.movement = glm::vec3(0.0f);
    input.yaw = m_localPlayer.getYaw();
    input.pitch = m_localPlayer.getPitch();
    input.jump = false;
    input.crouch = false;
    input.timestamp = static_cast<uint32_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count()
    );
    
    m_networkClient->sendMessage(input);
}

} // namespace client
} // namespace clonemine

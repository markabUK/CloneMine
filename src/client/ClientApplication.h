#pragma once

#include "NetworkClient.h"
#include "../core/Window.h"
#include "../rendering/Renderer.h"
#include "../world/World.h"
#include "../world/Player.h"
#include "../plugin/PluginManager.h"
#include <memory>
#include <string_view>
#include <unordered_map>

namespace clonemine {
namespace client {

class ClientApplication {
public:
    ClientApplication(std::string_view title, uint32_t width, uint32_t height);
    ~ClientApplication();

    // Delete copy operations for memory safety
    ClientApplication(const ClientApplication&) = delete;
    ClientApplication& operator=(const ClientApplication&) = delete;
    
    // Allow move operations
    ClientApplication(ClientApplication&&) noexcept = default;
    ClientApplication& operator=(ClientApplication&&) noexcept = default;

    bool connectToServer(const std::string& host, uint16_t port, const std::string& playerName);
    void run();
    void shutdown();

    [[nodiscard]] bool isRunning() const noexcept { return m_running; }

private:
    void processInput();
    void update(float deltaTime);
    void render();
    void handleNetworkMessage(const std::vector<uint8_t>& data);
    void sendPlayerInput();
    
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<World> m_world;
    std::unique_ptr<PluginManager> m_pluginManager;
    std::unique_ptr<NetworkClient> m_networkClient;
    
    // Client-side players (for rendering other players)
    std::unordered_map<uint32_t, Player> m_remotePlayers;
    Player m_localPlayer; // Local player for input prediction
    
    bool m_running{true};
    float m_inputSendTimer{0.0f};
    const float INPUT_SEND_RATE = 1.0f / 30.0f; // Send input 30 times per second
};

} // namespace client
} // namespace clonemine

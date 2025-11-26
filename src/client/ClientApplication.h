#pragma once

#include "NetworkClient.h"
#include "LoginScreen.h"
#include "CharacterSelectScreen.h"
#include "../core/Window.h"
#include "../core/InputManager.h"
#include "../rendering/Renderer.h"
#include "../world/World.h"
#include "../world/Player.h"
#include "../plugin/PluginManager.h"
#include <memory>
#include <string_view>
#include <unordered_map>
#include <deque>
#include <chrono>

namespace clonemine {
namespace client {

enum class ClientScreen {
    LOGIN,
    CHARACTER_SELECT,
    GAME
};

struct ChatMessage {
    std::string sender;
    std::string message;
    float timeRemaining{10.0f}; // Display for 10 seconds
};

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
    void setCharacterName(const std::string& characterName) { m_characterName = characterName; }
    [[nodiscard]] const std::string& getCharacterName() const { return m_characterName; }
    void run();
    void shutdown();

    [[nodiscard]] bool isRunning() const noexcept { return m_running; }

private:
    void processInput();
    void update(float deltaTime);
    void render();
    void handleNetworkMessage(const std::vector<uint8_t>& data);
    void sendPlayerInput();
    void sendChatMessage(const std::string& message);
    void updateChat(float deltaTime);
    
    // Screen management
    void switchToScreen(ClientScreen screen);
    void renderLoginScreen();
    void renderCharacterSelectScreen();
    void renderGameScreen();
    
    // AFK detection
    void recordActivity();
    void checkAFKTimeout(float deltaTime);
    void checkCharacterSelectTimeout(float deltaTime);
    float getCurrentTime() const;
    
    std::unique_ptr<Window> m_window;
    std::unique_ptr<InputManager> m_inputManager;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<World> m_world;
    std::unique_ptr<PluginManager> m_pluginManager;
    std::unique_ptr<NetworkClient> m_networkClient;
    
    // Screen management
    ClientScreen m_currentScreen{ClientScreen::LOGIN};
    std::unique_ptr<LoginScreen> m_loginScreen;
    std::unique_ptr<CharacterSelectScreen> m_characterSelectScreen;
    
    // Client-side players (for rendering other players)
    std::unordered_map<uint32_t, Player> m_remotePlayers;
    Player m_localPlayer; // Local player for input prediction
    
    // Camera
    float m_cameraYaw{0.0f};
    float m_cameraPitch{0.0f};
    const float m_cameraSensitivity{0.1f};
    
    // Character
    std::string m_characterName{"Player"}; // Character name from character server
    
    // Chat
    std::deque<ChatMessage> m_chatMessages;
    const size_t MAX_CHAT_MESSAGES = 10;
    
    // AFK timeout system
    float m_lastActivityTime{0.0f};
    bool m_afkWarningActive{false};
    float m_afkCountdown{0.0f};
    
    // Timeout constants
    static constexpr float AFK_TIMEOUT = 3600.0f;        // 1 hour in seconds
    static constexpr float AFK_WARNING_TIME = 30.0f;     // 30 second warning
    static constexpr float CHARACTER_SELECT_TIMEOUT = 7200.0f; // 2 hours in seconds
    
    bool m_running{true};
    float m_inputSendTimer{0.0f};
    // Input send rate: 60 Hz for highly responsive gameplay
    // Balances network bandwidth (~2KB/s) with input responsiveness
    // Higher rates (120Hz+) provide minimal benefit for human reaction times
    const float INPUT_SEND_RATE = 1.0f / 60.0f; // Send input 60 times per second
};

} // namespace client
} // namespace clonemine

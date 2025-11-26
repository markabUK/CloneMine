#include "ClientApplication.h"
#include "../network/NetworkMessage.h"
#include <chrono>
#include <iostream>

namespace clonemine {
namespace client {

ClientApplication::ClientApplication(std::string_view title, uint32_t width, uint32_t height)
    : m_window(std::make_unique<Window>(title, width, height))
    , m_inputManager(std::make_unique<InputManager>(m_window->getNativeWindow()))
    , m_renderer(std::make_unique<Renderer>(*m_window))
    , m_world(std::make_unique<World>())
    , m_pluginManager(std::make_unique<PluginManager>())
    , m_networkClient(std::make_unique<NetworkClient>())
    , m_loginScreen(std::make_unique<LoginScreen>())
    , m_characterSelectScreen(std::make_unique<CharacterSelectScreen>())
    , m_currentScreen(ClientScreen::LOGIN) // Ensure we start at LOGIN screen
{
    // Load plugins from the plugins directory
    m_pluginManager->loadPluginsFromDirectory("plugins");
    m_pluginManager->initializePlugins();
    
    // Set network message callback
    m_networkClient->setMessageCallback([this](const std::vector<uint8_t>& data) {
        handleNetworkMessage(data);
    });
    
    // Set up login screen callback
    m_loginScreen->setLoginCallback([this](const std::string& username, const std::string& sessionToken) {
        onLoginSuccess(username, sessionToken);
    });
    
    // Set up character select callbacks
    m_characterSelectScreen->setSelectCallback([this](int index) {
        onCharacterSelected(index);
    });
    
    m_characterSelectScreen->setCancelCallback([this]() {
        onCancelCharacterSelect();
    });
    
    // Initialize activity time
    recordActivity();
    
    std::cout << "Client started at Login Screen" << std::endl;
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

void ClientApplication::logout() {
    std::cout << "Logging out - returning to login screen" << std::endl;
    
    // Disconnect from all servers
    m_networkClient->disconnect();
    
    // Clear session data
    m_sessionToken.clear();
    m_characterName = "Player";
    
    // Return to login screen
    switchToScreen(ClientScreen::LOGIN);
}

void ClientApplication::exitGame() {
    std::cout << "Exiting game" << std::endl;
    
    // Disconnect from all servers
    m_networkClient->disconnect();
    
    // Stop the application
    m_running = false;
}

void ClientApplication::returnToCharacterSelect() {
    std::cout << "Returning to character select" << std::endl;
    
    // Disconnect from game server
    m_networkClient->disconnect();
    
    // Go to character select screen
    switchToScreen(ClientScreen::CHARACTER_SELECT);
}

void ClientApplication::processInput() {
    // Update input state
    m_inputManager->update();
    
    // Record activity on any input (resets AFK timer)
    if (m_inputManager->hasAnyInput()) {
        recordActivity();
    }
    
    // Screen-specific input handling
    switch (m_currentScreen) {
        case ClientScreen::LOGIN:
            m_loginScreen->processInput();
            break;
            
        case ClientScreen::CHARACTER_SELECT:
            m_characterSelectScreen->processInput();
            break;
            
        case ClientScreen::GAME:
            // Check for ESC key - logout options
            if (m_inputManager->isKeyJustPressed(Key::ESCAPE)) {
                // In a full implementation, this would open a menu
                // For now, ESC returns to character select
                std::cout << "ESC pressed - returning to character select" << std::endl;
                returnToCharacterSelect();
                return;
            }
            
            // Check for chat message send
            if (m_inputManager->isKeyJustPressed(Key::ENTER) && 
                !m_inputManager->isChatMode() &&
                !m_inputManager->getTypedText().empty()) {
                sendChatMessage(m_inputManager->getTypedText());
                m_inputManager->clearTypedText();
            }
            
            // Update camera with mouse
            if (!m_inputManager->isChatMode()) {
                glm::vec2 mouseDelta = m_inputManager->getMouseDelta();
                m_cameraYaw += mouseDelta.x * m_cameraSensitivity;
                m_cameraPitch -= mouseDelta.y * m_cameraSensitivity;
                
                // Clamp pitch
                m_cameraPitch = std::clamp(m_cameraPitch, -89.0f, 89.0f);
                
                // Update local player rotation
                m_localPlayer.setRotation(m_cameraYaw, m_cameraPitch);
            }
            break;
    }
    
    // Plugin callbacks for input
    m_pluginManager->callHook("onInput");
}

void ClientApplication::update(float deltaTime) {
    // Process network messages
    m_networkClient->processMessages();
    
    // Screen-specific updates
    switch (m_currentScreen) {
        case ClientScreen::LOGIN:
            m_loginScreen->update(deltaTime);
            break;
            
        case ClientScreen::CHARACTER_SELECT:
            m_characterSelectScreen->update(deltaTime);
            checkCharacterSelectTimeout(deltaTime);
            break;
            
        case ClientScreen::GAME:
            // Update chat messages
            updateChat(deltaTime);
            
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
            
            // Check AFK timeout
            checkAFKTimeout(deltaTime);
            break;
    }
    
    // Plugin update callbacks
    m_pluginManager->callHook("onUpdate", deltaTime);
}

void ClientApplication::render() {
    if (m_window->wasResized()) {
        m_renderer->handleResize();
        m_window->resetResizedFlag();
    }

    switch (m_currentScreen) {
        case ClientScreen::LOGIN:
            renderLoginScreen();
            break;
            
        case ClientScreen::CHARACTER_SELECT:
            renderCharacterSelectScreen();
            break;
            
        case ClientScreen::GAME:
            renderGameScreen();
            break;
    }
    
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
        
        case network::MessageType::CHAT_MESSAGE: {
            if (data.size() < 10) break;
            
            // Parse chat message
            size_t offset = 1;
            
            // Read sender length
            uint32_t senderLen = data[offset] | (data[offset+1] << 8) | 
                                (data[offset+2] << 16) | (data[offset+3] << 24);
            offset += 4;
            
            if (offset + senderLen + 4 > data.size()) break;
            
            std::string sender(data.begin() + offset, data.begin() + offset + senderLen);
            offset += senderLen;
            
            // Read message length
            uint32_t msgLen = data[offset] | (data[offset+1] << 8) | 
                             (data[offset+2] << 16) | (data[offset+3] << 24);
            offset += 4;
            
            if (offset + msgLen > data.size()) break;
            
            std::string message(data.begin() + offset, data.begin() + offset + msgLen);
            
            // Add to chat messages
            ChatMessage chatMsg;
            chatMsg.sender = sender;
            chatMsg.message = message;
            chatMsg.timeRemaining = 10.0f;
            
            m_chatMessages.push_back(chatMsg);
            if (m_chatMessages.size() > MAX_CHAT_MESSAGES) {
                m_chatMessages.pop_front();
            }
            
            std::cout << "[CHAT] " << sender << ": " << message << std::endl;
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
    
    // Get actual input from input manager
    glm::vec3 movement = m_inputManager->getMovementInput();
    input.movement = movement;
    input.yaw = m_cameraYaw;
    input.pitch = m_cameraPitch;
    input.jump = m_inputManager->isKeyPressed(Key::SPACE);
    input.crouch = m_inputManager->isKeyPressed(Key::LEFT_SHIFT);
    input.timestamp = static_cast<uint32_t>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count()
    );
    
    m_networkClient->sendMessage(input);
}

void ClientApplication::sendChatMessage(const std::string& message) {
    if (!m_networkClient->isConnected() || message.empty()) {
        return;
    }
    
    network::ChatMessage chatMsg;
    chatMsg.sender = m_characterName; // Use actual character name
    chatMsg.message = message;
    
    m_networkClient->sendMessage(chatMsg);
}

void ClientApplication::updateChat(float deltaTime) {
    // Update chat message timers
    for (auto it = m_chatMessages.begin(); it != m_chatMessages.end(); ) {
        it->timeRemaining -= deltaTime;
        if (it->timeRemaining <= 0.0f) {
            it = m_chatMessages.erase(it);
        } else {
            ++it;
        }
    }
}

// Screen management methods
void ClientApplication::switchToScreen(ClientScreen screen) {
    m_currentScreen = screen;
    recordActivity(); // Reset activity timer on screen change
    
    // Reset screen states
    switch (screen) {
        case ClientScreen::LOGIN:
            m_loginScreen->reset();
            break;
        case ClientScreen::CHARACTER_SELECT:
            m_characterSelectScreen->reset();
            break;
        case ClientScreen::GAME:
            m_afkWarningActive = false;
            m_afkCountdown = 0.0f;
            break;
    }
}

void ClientApplication::renderLoginScreen() {
    m_renderer->beginFrame();
    // TODO: Render login UI
    m_loginScreen->render();
    m_renderer->endFrame();
}

void ClientApplication::renderCharacterSelectScreen() {
    m_renderer->beginFrame();
    // TODO: Render character select UI
    m_characterSelectScreen->render();
    m_renderer->endFrame();
}

void ClientApplication::renderGameScreen() {
    m_renderer->beginFrame();
    m_renderer->renderWorld(*m_world);
    
    // Render AFK warning if active
    if (m_afkWarningActive) {
        // TODO: Integrate with UI system
        // For now, this is a placeholder
        // In a real implementation, this would render large red text at screen center:
        // "AFK Warning: You will be logged out in XX seconds"
        std::cout << "=== AFK WARNING ===" << std::endl;
        std::cout << "You will be logged out in " << static_cast<int>(m_afkCountdown) << " seconds" << std::endl;
        std::cout << "===================" << std::endl;
    }
    
    m_renderer->endFrame();
    
    // TODO: Render chat messages (would need UI system)
}

// AFK detection methods
void ClientApplication::recordActivity() {
    m_lastActivityTime = getCurrentTime();
    
    // Cancel AFK warning if active
    if (m_afkWarningActive) {
        m_afkWarningActive = false;
        m_afkCountdown = 0.0f;
        std::cout << "AFK warning cancelled - activity detected" << std::endl;
    }
}

void ClientApplication::checkAFKTimeout(float deltaTime) {
    float currentTime = getCurrentTime();
    float timeSinceActivity = currentTime - m_lastActivityTime;
    
    // Check if player has been AFK for 1 hour
    if (timeSinceActivity >= AFK_TIMEOUT) {
        if (!m_afkWarningActive) {
            // Start AFK warning countdown
            m_afkWarningActive = true;
            m_afkCountdown = AFK_WARNING_TIME;
            std::cout << "AFK detected - starting 30 second countdown" << std::endl;
        } else {
            // Update countdown
            m_afkCountdown -= deltaTime;
            
            // Check if countdown reached zero
            if (m_afkCountdown <= 0.0f) {
                std::cout << "AFK timeout - returning to character select" << std::endl;
                
                // Disconnect from game server
                m_networkClient->disconnect();
                
                // Return to character select screen
                switchToScreen(ClientScreen::CHARACTER_SELECT);
            }
        }
    }
}

void ClientApplication::checkCharacterSelectTimeout(float deltaTime) {
    float currentTime = getCurrentTime();
    float timeSinceActivity = m_characterSelectScreen->getLastActivityTime();
    float inactiveTime = currentTime - timeSinceActivity;
    
    // Check if no actions for 2 hours
    if (inactiveTime >= CHARACTER_SELECT_TIMEOUT) {
        std::cout << "Character select timeout - returning to login" << std::endl;
        
        // Disconnect from character server
        m_networkClient->disconnect();
        
        // Return to login screen
        switchToScreen(ClientScreen::LOGIN);
    }
}

float ClientApplication::getCurrentTime() const {
    auto now = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
    return static_cast<float>(duration.count());
}

// Screen callback implementations
void ClientApplication::onLoginSuccess(const std::string& username, const std::string& sessionToken) {
    std::cout << "Login successful for user: " << username << std::endl;
    
    // Store session token
    m_sessionToken = sessionToken;
    
    // TODO: Connect to character server with session token
    // For now, just switch to character select screen
    
    // Switch to character select screen
    switchToScreen(ClientScreen::CHARACTER_SELECT);
}

void ClientApplication::onCharacterSelected(int characterIndex) {
    std::cout << "Character selected: index " << characterIndex << std::endl;
    
    // TODO: Get character name from character list
    // TODO: Connect to game server with character data
    
    // For now, switch to game screen
    switchToScreen(ClientScreen::GAME);
}

void ClientApplication::onCancelCharacterSelect() {
    std::cout << "Character selection cancelled - returning to login" << std::endl;
    
    // Disconnect from character server
    m_networkClient->disconnect();
    
    // Clear session
    m_sessionToken.clear();
    
    // Return to login screen
    switchToScreen(ClientScreen::LOGIN);
}

} // namespace client
} // namespace clonemine

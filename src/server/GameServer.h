#pragma once

#include "ServerPlayer.h"
#include "../world/World.h"
#include "../world/Chunk.h"
#include "../save/SaveSystem.h"
#include "common/RateLimiter.h"
#include <asio.hpp>
#include <memory>
#include <unordered_map>
#include <thread>
#include <atomic>

namespace clonemine {
namespace server {

class GameServer {
public:
    explicit GameServer(uint16_t port);
    ~GameServer();
    
    // Delete copy operations
    GameServer(const GameServer&) = delete;
    GameServer& operator=(const GameServer&) = delete;
    
    void start();
    void stop();
    void run();
    
    [[nodiscard]] bool isRunning() const { return m_running; }
    
private:
    void acceptConnections();
    void handleNewConnection(std::shared_ptr<asio::ip::tcp::socket> socket);
    void handlePlayerInput(uint32_t playerId, const std::vector<uint8_t>& data);
    void handleChatMessage(uint32_t playerId, const std::vector<uint8_t>& data);
    void broadcastPlayerStates();
    void updateGame(float deltaTime);
    void savePlayerData(const ServerPlayer& player);
    bool loadPlayerData(ServerPlayer& player, const std::string& playerName);
    
    // Network
    asio::io_context m_ioContext;
    std::unique_ptr<asio::ip::tcp::acceptor> m_acceptor;
    std::thread m_networkThread;
    
    // Game state
    std::unique_ptr<World> m_world;
    std::unordered_map<uint32_t, std::unique_ptr<ServerPlayer>> m_players;
    uint32_t m_nextPlayerId{1};
    
    // Threading
    std::atomic<bool> m_running{false};
    uint16_t m_port;
    
    // Rate limiter to prevent DOS attacks
    std::unique_ptr<RateLimiter> m_rateLimiter;
};

} // namespace server
} // namespace clonemine

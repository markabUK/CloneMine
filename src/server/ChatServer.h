#pragma once

#include "../network/NetworkMessage.h"
#include "../network/PacketEncryption.h"
#include "common/RateLimiter.h"
#include <asio.hpp>
#include <memory>
#include <unordered_map>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <deque>

namespace clonemine {
namespace server {

// Chat client session
struct ChatClient {
    uint32_t playerId;
    std::string playerName;
    std::shared_ptr<asio::ip::tcp::socket> socket;
    std::unique_ptr<network::PacketEncryption> encryption;
    bool connected{true};
};

// Chat server handles all chat communication separately from game logic
class ChatServer {
public:
    explicit ChatServer(uint16_t port);
    ~ChatServer();
    
    // Delete copy operations
    ChatServer(const ChatServer&) = delete;
    ChatServer& operator=(const ChatServer&) = delete;
    
    void start();
    void stop();
    void run();
    
    [[nodiscard]] bool isRunning() const { return m_running; }
    
private:
    void acceptConnections();
    void handleNewConnection(std::shared_ptr<asio::ip::tcp::socket> socket);
    void handleChatMessage(uint32_t playerId, const std::vector<uint8_t>& data);
    void broadcastMessage(const std::string& sender, const std::string& message);
    void disconnectClient(uint32_t playerId);
    
    // Network
    asio::io_context m_ioContext;
    std::unique_ptr<asio::ip::tcp::acceptor> m_acceptor;
    std::thread m_networkThread;
    
    // Connected clients
    std::unordered_map<uint32_t, std::unique_ptr<ChatClient>> m_clients;
    std::mutex m_clientsMutex;
    uint32_t m_nextClientId{1};
    
    // Chat history (last 100 messages)
    std::deque<std::pair<std::string, std::string>> m_chatHistory;
    std::mutex m_historyMutex;
    static constexpr size_t MAX_HISTORY = 100;
    
    // Threading
    std::atomic<bool> m_running{false};
    uint16_t m_port;
    
    // Rate limiter to prevent DOS attacks
    std::unique_ptr<RateLimiter> m_rateLimiter;
};

} // namespace server
} // namespace clonemine

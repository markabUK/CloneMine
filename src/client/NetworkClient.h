#pragma once

#include "../network/NetworkMessage.h"
#include "../network/PacketEncryption.h"
#include <asio.hpp>
#include <memory>
#include <string>
#include <functional>
#include <queue>
#include <mutex>

namespace clonemine {
namespace client {

class NetworkClient {
public:
    NetworkClient();
    ~NetworkClient();
    
    // Delete copy operations
    NetworkClient(const NetworkClient&) = delete;
    NetworkClient& operator=(const NetworkClient&) = delete;
    
    // Connection
    bool connect(const std::string& host, uint16_t port, const std::string& playerName);
    void disconnect();
    bool isConnected() const { return m_connected; }
    
    // Messaging
    void sendMessage(const network::NetworkMessage& message);
    void processMessages();
    
    // Callbacks
    using MessageCallback = std::function<void(const std::vector<uint8_t>&)>;
    void setMessageCallback(MessageCallback callback) { m_messageCallback = callback; }
    
    uint32_t getPlayerId() const { return m_playerId; }
    
private:
    void receiveMessages();
    void handleMessage(const std::vector<uint8_t>& data);
    
    asio::io_context m_ioContext;
    std::unique_ptr<asio::ip::tcp::socket> m_socket;
    std::thread m_networkThread;
    
    bool m_connected{false};
    uint32_t m_playerId{0};
    
    MessageCallback m_messageCallback;
    std::queue<std::vector<uint8_t>> m_messageQueue;
    std::mutex m_queueMutex;
    
    // Encryption
    std::unique_ptr<network::PacketEncryption> m_encryption;
};

} // namespace client
} // namespace clonemine

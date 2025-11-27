#pragma once

#include "../network/NetworkMessage.h"
#include "../network/PacketEncryption.h"
#include <asio.hpp>
#include <memory>
#include <unordered_map>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>
#include <random>

namespace clonemine {
namespace server {

// Login session data
struct LoginSession {
    uint32_t sessionId;
    std::string username;
    std::shared_ptr<asio::ip::tcp::socket> socket;
    std::unique_ptr<network::PacketEncryption> encryption;
    bool authenticated{false};
    bool connected{true};
    std::string sessionToken; // Token for character server authentication
};

// Login server handles authentication with handshaking
class LoginServer {
public:
    explicit LoginServer(uint16_t port, uint32_t maxCharactersPerAccount = 5);
    ~LoginServer();
    
    // Delete copy operations
    LoginServer(const LoginServer&) = delete;
    LoginServer& operator=(const LoginServer&) = delete;
    
    void start();
    void stop();
    void run();
    
    [[nodiscard]] bool isRunning() const { return m_running; }
    [[nodiscard]] uint32_t getMaxCharactersPerAccount() const { return m_maxCharactersPerAccount; }
    
private:
    void acceptConnections();
    void handleNewConnection(std::shared_ptr<asio::ip::tcp::socket> socket);
    void handleHandshake(uint32_t sessionId, const std::vector<uint8_t>& data);
    void handleLoginRequest(uint32_t sessionId, const std::vector<uint8_t>& data);
    bool authenticateUser(const std::string& username, const std::string& password);
    std::string generateSessionToken();
    
    // Network
    asio::io_context m_ioContext;
    std::unique_ptr<asio::ip::tcp::acceptor> m_acceptor;
    std::thread m_networkThread;
    
    // Connected sessions
    std::unordered_map<uint32_t, std::unique_ptr<LoginSession>> m_sessions;
    std::mutex m_sessionsMutex;
    uint32_t m_nextSessionId{1};
    
    // Authenticated sessions (sessionToken -> username)
    std::unordered_map<std::string, std::string> m_authenticatedSessions;
    std::mutex m_authMutex;
    
    // User database (simplified - in production use real DB)
    // username -> password hash
    std::unordered_map<std::string, std::string> m_userDatabase;
    std::mutex m_dbMutex;
    
    // Threading
    std::atomic<bool> m_running{false};
    uint16_t m_port;
    
    // Configuration
    uint32_t m_maxCharactersPerAccount;
    
    // Random number generator for session tokens
    std::random_device m_rd;
    std::mt19937 m_rng;
};

} // namespace server
} // namespace clonemine

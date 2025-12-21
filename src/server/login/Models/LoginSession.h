#pragma once

#include "../../../network/PacketEncryption.h"
#include <asio.hpp>
#include <string>
#include <memory>
#include <cstdint>

namespace clonemine {
namespace server {
namespace login {

/// <summary>
/// Represents an active login session
/// Single Responsibility: Session state management only
/// </summary>
class LoginSession {
public:
    LoginSession() = default;
    LoginSession(uint32_t id, std::shared_ptr<asio::ip::tcp::socket> socket)
        : m_sessionId(id)
        , m_socket(std::move(socket))
        , m_authenticated(false)
        , m_connected(true) {}
    
    // Getters
    [[nodiscard]] uint32_t getSessionId() const { return m_sessionId; }
    [[nodiscard]] const std::string& getUsername() const { return m_username; }
    [[nodiscard]] const std::string& getSessionToken() const { return m_sessionToken; }
    [[nodiscard]] bool isAuthenticated() const { return m_authenticated; }
    [[nodiscard]] bool isConnected() const { return m_connected; }
    [[nodiscard]] std::shared_ptr<asio::ip::tcp::socket> getSocket() { return m_socket; }
    [[nodiscard]] network::PacketEncryption* getEncryption() { return m_encryption.get(); }
    
    // Setters
    void setSessionId(uint32_t id) { m_sessionId = id; }
    void setUsername(const std::string& username) { m_username = username; }
    void setSessionToken(const std::string& token) { m_sessionToken = token; }
    void setAuthenticated(bool auth) { m_authenticated = auth; }
    void setConnected(bool conn) { m_connected = conn; }
    void setSocket(std::shared_ptr<asio::ip::tcp::socket> socket) { m_socket = std::move(socket); }
    void setEncryption(std::unique_ptr<network::PacketEncryption> encryption) { 
        m_encryption = std::move(encryption); 
    }
    
private:
    uint32_t m_sessionId{0};
    std::string m_username;
    std::string m_sessionToken;
    std::shared_ptr<asio::ip::tcp::socket> m_socket;
    std::unique_ptr<network::PacketEncryption> m_encryption;
    bool m_authenticated{false};
    bool m_connected{true};
};

} // namespace login
} // namespace server
} // namespace clonemine

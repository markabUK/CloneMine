#pragma once

#include <cstdint>
#include <string>

namespace clonemine {
namespace server {
namespace login {

/// <summary>
/// Server configuration settings
/// Single Responsibility: Configuration data only
/// </summary>
class ServerConfiguration {
public:
    ServerConfiguration()
        : m_port(25564)
        , m_maxCharactersPerAccount(5)
        , m_maxFailedLoginAttempts(5)
        , m_sessionTokenLength(32)
        , m_encryptionKey("CloneMineSharedSecret2024")
        , m_maxUsernameLength(16)
        , m_minUsernameLength(3)
        , m_maxPasswordLength(64)
        , m_minPasswordLength(8) {}
    
    // Getters
    [[nodiscard]] uint16_t getPort() const { return m_port; }
    [[nodiscard]] uint32_t getMaxCharactersPerAccount() const { return m_maxCharactersPerAccount; }
    [[nodiscard]] uint32_t getMaxFailedLoginAttempts() const { return m_maxFailedLoginAttempts; }
    [[nodiscard]] uint32_t getSessionTokenLength() const { return m_sessionTokenLength; }
    [[nodiscard]] const std::string& getEncryptionKey() const { return m_encryptionKey; }
    [[nodiscard]] uint32_t getMaxUsernameLength() const { return m_maxUsernameLength; }
    [[nodiscard]] uint32_t getMinUsernameLength() const { return m_minUsernameLength; }
    [[nodiscard]] uint32_t getMaxPasswordLength() const { return m_maxPasswordLength; }
    [[nodiscard]] uint32_t getMinPasswordLength() const { return m_minPasswordLength; }
    
    // Setters
    void setPort(uint16_t port) { m_port = port; }
    void setMaxCharactersPerAccount(uint32_t max) { m_maxCharactersPerAccount = max; }
    void setMaxFailedLoginAttempts(uint32_t max) { m_maxFailedLoginAttempts = max; }
    void setEncryptionKey(const std::string& key) { m_encryptionKey = key; }
    
private:
    uint16_t m_port;
    uint32_t m_maxCharactersPerAccount;
    uint32_t m_maxFailedLoginAttempts;
    uint32_t m_sessionTokenLength;
    std::string m_encryptionKey;
    uint32_t m_maxUsernameLength;
    uint32_t m_minUsernameLength;
    uint32_t m_maxPasswordLength;
    uint32_t m_minPasswordLength;
};

} // namespace login
} // namespace server
} // namespace clonemine

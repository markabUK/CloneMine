#pragma once

#include <string>
#include <cstdint>

namespace clonemine {
namespace server {
namespace login {

/// <summary>
/// Represents a user account with authentication credentials
/// Single Responsibility: Data model only, no business logic
/// </summary>
class Account {
public:
    Account() = default;
    Account(uint32_t id, const std::string& username, const std::string& passwordHash, const std::string& salt)
        : m_id(id)
        , m_username(username)
        , m_passwordHash(passwordHash)
        , m_salt(salt)
        , m_isActive(true)
        , m_failedLoginAttempts(0) {}
    
    // Getters
    [[nodiscard]] uint32_t getId() const { return m_id; }
    [[nodiscard]] const std::string& getUsername() const { return m_username; }
    [[nodiscard]] const std::string& getPasswordHash() const { return m_passwordHash; }
    [[nodiscard]] const std::string& getSalt() const { return m_salt; }
    [[nodiscard]] bool isActive() const { return m_isActive; }
    [[nodiscard]] uint32_t getFailedLoginAttempts() const { return m_failedLoginAttempts; }
    
    // Setters
    void setId(uint32_t id) { m_id = id; }
    void setUsername(const std::string& username) { m_username = username; }
    void setPasswordHash(const std::string& hash) { m_passwordHash = hash; }
    void setSalt(const std::string& salt) { m_salt = salt; }
    void setActive(bool active) { m_isActive = active; }
    void setFailedLoginAttempts(uint32_t attempts) { m_failedLoginAttempts = attempts; }
    
    void incrementFailedAttempts() { ++m_failedLoginAttempts; }
    void resetFailedAttempts() { m_failedLoginAttempts = 0; }
    
private:
    uint32_t m_id{0};
    std::string m_username;
    std::string m_passwordHash;
    std::string m_salt;
    bool m_isActive{true};
    uint32_t m_failedLoginAttempts{0};
};

} // namespace login
} // namespace server
} // namespace clonemine

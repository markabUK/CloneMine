#pragma once

#include <string>
#include <cstdint>

namespace clonemine {
namespace server {
namespace login {

/// <summary>
/// Result of a login attempt
/// Single Responsibility: Encapsulates login operation result
/// </summary>
class LoginResult {
public:
    LoginResult(bool success, const std::string& message = "", const std::string& token = "")
        : m_success(success)
        , m_message(message)
        , m_sessionToken(token)
        , m_accountId(0) {}
    
    [[nodiscard]] bool isSuccess() const { return m_success; }
    [[nodiscard]] const std::string& getMessage() const { return m_message; }
    [[nodiscard]] const std::string& getSessionToken() const { return m_sessionToken; }
    [[nodiscard]] uint32_t getAccountId() const { return m_accountId; }
    
    void setAccountId(uint32_t id) { m_accountId = id; }
    
    static LoginResult Success(const std::string& token, uint32_t accountId) {
        LoginResult result(true, "Login successful", token);
        result.setAccountId(accountId);
        return result;
    }
    
    static LoginResult Failure(const std::string& message) {
        return LoginResult(false, message);
    }
    
private:
    bool m_success;
    std::string m_message;
    std::string m_sessionToken;
    uint32_t m_accountId;
};

} // namespace login
} // namespace server
} // namespace clonemine

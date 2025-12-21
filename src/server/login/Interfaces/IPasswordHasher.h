#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace clonemine {
namespace server {
namespace login {

/// <summary>
/// Interface for password hashing operations
/// Interface Segregation: Focused on password security only
/// </summary>
class IPasswordHasher {
public:
    virtual ~IPasswordHasher() = default;
    
    /// <summary>Hash password with salt using PBKDF2</summary>
    virtual std::string hashPassword(const std::string& password, const std::string& salt) = 0;
    
    /// <summary>Generate random salt</summary>
    virtual std::string generateSalt() = 0;
    
    /// <summary>Verify password against hash (constant-time comparison)</summary>
    virtual bool verifyPassword(const std::string& password, const std::string& hash, const std::string& salt) = 0;
};

} // namespace login
} // namespace server
} // namespace clonemine

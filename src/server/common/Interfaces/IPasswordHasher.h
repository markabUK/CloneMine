#pragma once
#include <string>

namespace CloneMine {
namespace Common {

/// <summary>
/// Interface for password hashing services
/// Interface Segregation Principle: Focused on password hashing only
/// </summary>
class IPasswordHasher {
public:
    virtual ~IPasswordHasher() = default;

    /// <summary>
    /// Hashes a password using PBKDF2-SHA256 with 100,000 iterations
    /// </summary>
    /// <param name="password">Plain text password</param>
    /// <returns>Hex-encoded salt and hash</returns>
    virtual std::string HashPassword(const std::string& password) = 0;

    /// <summary>
    /// Verifies a password against a hash using constant-time comparison
    /// </summary>
    /// <param name="password">Plain text password to verify</param>
    /// <param name="hash">Hex-encoded salt and hash to verify against</param>
    /// <returns>True if password matches, false otherwise</returns>
    virtual bool VerifyPassword(const std::string& password, const std::string& hash) = 0;
};

} // namespace Common
} // namespace CloneMine

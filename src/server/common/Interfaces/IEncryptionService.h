#pragma once
#include <vector>
#include <string>

namespace CloneMine {
namespace Common {

/// <summary>
/// Interface for encryption/decryption services
/// Interface Segregation Principle: Focused on encryption operations only
/// </summary>
class IEncryptionService {
public:
    virtual ~IEncryptionService() = default;

    /// <summary>
    /// Encrypts data using AES-256-CBC
    /// </summary>
    /// <param name="data">Data to encrypt</param>
    /// <returns>Encrypted data</returns>
    virtual std::vector<uint8_t> Encrypt(const std::vector<uint8_t>& data) = 0;

    /// <summary>
    /// Decrypts data using AES-256-CBC
    /// </summary>
    /// <param name="data">Encrypted data</param>
    /// <returns>Decrypted data</returns>
    virtual std::vector<uint8_t> Decrypt(const std::vector<uint8_t>& data) = 0;

    /// <summary>
    /// Encrypts a string message
    /// </summary>
    /// <param name="message">Plain text message</param>
    /// <returns>Encrypted message</returns>
    virtual std::string EncryptString(const std::string& message) = 0;

    /// <summary>
    /// Decrypts a string message
    /// </summary>
    /// <param name="encryptedMessage">Encrypted message</param>
    /// <returns>Decrypted plain text message</returns>
    virtual std::string DecryptString(const std::string& encryptedMessage) = 0;
};

} // namespace Common
} // namespace CloneMine

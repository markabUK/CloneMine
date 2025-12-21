#pragma once

#include <vector>
#include <cstdint>

namespace clonemine {
namespace server {
namespace login {

/// <summary>
/// Interface for encryption/decryption operations
/// Interface Segregation: Focused on encryption only
/// </summary>
class IEncryptionService {
public:
    virtual ~IEncryptionService() = default;
    
    /// <summary>Encrypt data using AES-256</summary>
    virtual void encrypt(std::vector<uint8_t>& data) = 0;
    
    /// <summary>Decrypt data using AES-256</summary>
    virtual void decrypt(std::vector<uint8_t>& data) = 0;
    
    /// <summary>Generate random IV for encryption</summary>
    virtual std::vector<uint8_t> generateIV() = 0;
};

} // namespace login
} // namespace server
} // namespace clonemine

#pragma once

#include <string>
#include <vector>
#include <memory>

namespace CloneMine {
namespace Chat {

/**
 * @brief Interface for encryption/decryption operations
 * 
 * Follows Interface Segregation Principle - focused contract for encryption
 */
class IEncryptionService {
public:
    virtual ~IEncryptionService() = default;

    virtual std::vector<unsigned char> Encrypt(const std::string& plaintext) = 0;
    virtual std::string Decrypt(const std::vector<unsigned char>& ciphertext) = 0;
    virtual void SetKey(const std::string& key) = 0;
};

} // namespace Chat
} // namespace CloneMine

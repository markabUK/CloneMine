#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <array>

namespace clonemine {
namespace network {

/**
 * Simple packet encryption using XOR cipher with a rotating key.
 * This provides basic obfuscation to make packet sniffing harder.
 * 
 * For production use, consider upgrading to AES-256 or similar.
 */
class PacketEncryption {
public:
    // Initialize with a shared secret key
    explicit PacketEncryption(const std::string& secretKey);
    
    // Encrypt data in-place
    void encrypt(std::vector<uint8_t>& data);
    
    // Decrypt data in-place
    void decrypt(std::vector<uint8_t>& data);
    
    // Generate a random session key
    static std::string generateSessionKey();
    
private:
    std::array<uint8_t, 32> m_key;  // 256-bit key
    uint32_t m_counter{0};           // Counter for additional randomness
    
    void deriveKey(const std::string& secretKey);
    uint8_t getKeyByte(size_t index);
};

} // namespace network
} // namespace clonemine

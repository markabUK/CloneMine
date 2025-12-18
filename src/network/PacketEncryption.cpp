#include "PacketEncryption.h"
#include <random>
#include <cstring>
#include <algorithm>

namespace clonemine {
namespace network {

PacketEncryption::PacketEncryption(const std::string& secretKey) {
    deriveKey(secretKey);
}

void PacketEncryption::encrypt(std::vector<uint8_t>& data) {
    if (data.empty()) return;
    
    // XOR encryption with rotating key
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] ^= getKeyByte(i);
    }
    
    // Increment counter for next encryption
    m_counter++;
}

void PacketEncryption::decrypt(std::vector<uint8_t>& data) {
    // XOR is symmetric, so decrypt is the same as encrypt
    encrypt(data);
}

std::string PacketEncryption::generateSessionKey() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(33, 126); // Printable ASCII
    
    std::string key;
    key.reserve(32);
    for (int i = 0; i < 32; ++i) {
        key += static_cast<char>(dis(gen));
    }
    
    return key;
}

void PacketEncryption::deriveKey(const std::string& secretKey) {
    // Simple key derivation: hash the secret multiple times
    m_key.fill(0);
    
    // Copy secret key into key array (with wrapping if needed)
    for (size_t i = 0; i < secretKey.length(); ++i) {
        m_key[i % m_key.size()] ^= static_cast<uint8_t>(secretKey[i]);
    }
    
    // Mix the key with a simple permutation
    for (size_t round = 0; round < 4; ++round) {
        for (size_t i = 0; i < m_key.size(); ++i) {
            // Simple mixing function
            uint8_t temp = m_key[i];
            temp = (temp << 3) | (temp >> 5);  // Rotate left by 3
            temp ^= m_key[(i + 7) % m_key.size()];
            m_key[i] = temp;
        }
    }
}

uint8_t PacketEncryption::getKeyByte(size_t index) {
    // Combine key with counter for additional randomness
    uint32_t mixed = m_counter ^ (static_cast<uint32_t>(index) * 0x9E3779B9); // Golden ratio
    return m_key[index % m_key.size()] ^ static_cast<uint8_t>(mixed & 0xFF);
}

} // namespace network
} // namespace clonemine

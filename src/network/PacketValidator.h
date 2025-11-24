#pragma once

#include "NetworkMessage.h"
#include <vector>
#include <cstdint>

namespace clonemine {
namespace network {

/**
 * Validates network packets for security and correctness.
 * Rejects malformed, oversized, or suspicious packets.
 */
class PacketValidator {
public:
    // Validation result
    enum class ValidationResult {
        VALID,
        INVALID_SIZE,
        INVALID_TYPE,
        INVALID_DATA,
        NULL_DATA,
        SUSPICIOUS
    };
    
    // Validate a raw packet before processing
    static ValidationResult validatePacket(const std::vector<uint8_t>& data, MessageType expectedType);
    
    // Validate packet size against expected size for message type
    static bool validateSize(MessageType type, size_t actualSize);
    
    // Check for null bytes in critical data sections
    static bool hasNullInCriticalData(const std::vector<uint8_t>& data, MessageType type);
    
    // Get minimum expected size for a message type
    static size_t getMinimumSize(MessageType type);
    
    // Get maximum allowed size for a message type
    static size_t getMaximumSize(MessageType type);
    
private:
    static constexpr size_t MAX_PACKET_SIZE = 1024 * 1024; // 1MB max
    static constexpr size_t MAX_STRING_LENGTH = 512;
};

} // namespace network
} // namespace clonemine

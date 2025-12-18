#include "PacketValidator.h"
#include <algorithm>

namespace clonemine {
namespace network {

PacketValidator::ValidationResult PacketValidator::validatePacket(
    const std::vector<uint8_t>& data, 
    MessageType expectedType) {
    
    // Check for empty or null data
    if (data.empty()) {
        return ValidationResult::NULL_DATA;
    }
    
    // Check overall size
    if (data.size() > MAX_PACKET_SIZE) {
        return ValidationResult::INVALID_SIZE;
    }
    
    // Validate message type
    MessageType actualType = static_cast<MessageType>(data[0]);
    if (actualType != expectedType) {
        return ValidationResult::INVALID_TYPE;
    }
    
    // Validate size against expected range
    if (!validateSize(actualType, data.size())) {
        return ValidationResult::INVALID_SIZE;
    }
    
    // Check for nulls in critical data
    if (hasNullInCriticalData(data, actualType)) {
        return ValidationResult::NULL_DATA;
    }
    
    return ValidationResult::VALID;
}

bool PacketValidator::validateSize(MessageType type, size_t actualSize) {
    size_t minSize = getMinimumSize(type);
    size_t maxSize = getMaximumSize(type);
    
    return actualSize >= minSize && actualSize <= maxSize;
}

bool PacketValidator::hasNullInCriticalData(const std::vector<uint8_t>& data, MessageType type) {
    // Check based on message type
    switch (type) {
        case MessageType::CONNECT_REQUEST:
        case MessageType::CONNECT_RESPONSE:
        case MessageType::CHAT_MESSAGE: {
            // These have string data - check for proper string formatting
            // Strings should have length prefix, not null-terminated
            if (data.size() < 6) return true;
            
            size_t offset = 5; // Skip type + playerId
            while (offset + 4 <= data.size()) {
                uint32_t strLen = data[offset] | (data[offset+1] << 8) | 
                                 (data[offset+2] << 16) | (data[offset+3] << 24);
                offset += 4;
                
                // Check if string length is reasonable
                if (strLen > MAX_STRING_LENGTH) {
                    return true; // Suspicious
                }
                
                // Skip the string data
                offset += strLen;
                if (offset > data.size()) {
                    return true; // String extends beyond packet
                }
            }
            break;
        }
        
        case MessageType::PLAYER_INPUT:
        case MessageType::PLAYER_STATE_UPDATE: {
            // These should have fixed sizes with float data
            // No nulls expected in float data
            break;
        }
        
        default:
            break;
    }
    
    return false;
}

size_t PacketValidator::getMinimumSize(MessageType type) {
    switch (type) {
        case MessageType::CONNECT_REQUEST:
            return 10; // type + playerId + minimum name length field + 1 char
        case MessageType::CONNECT_RESPONSE:
            return 10; // type + accepted + playerId + message length
        case MessageType::DISCONNECT:
            return 1;  // Just type
        case MessageType::PLAYER_INPUT:
            return 50; // type + playerId + movement + rotation + flags + timestamp
        case MessageType::PLAYER_STATE_UPDATE:
            return 50; // type + playerId + position + velocity + rotation + health + resource + timestamp
        case MessageType::PLAYER_SPAWN:
            return 20; // type + playerId + position + minimum strings
        case MessageType::PLAYER_DESPAWN:
            return 5;  // type + playerId
        case MessageType::CHAT_MESSAGE:
            return 10; // type + minimum string data
        default:
            return 1;
    }
}

size_t PacketValidator::getMaximumSize(MessageType type) {
    switch (type) {
        case MessageType::CONNECT_REQUEST:
        case MessageType::CONNECT_RESPONSE:
        case MessageType::CHAT_MESSAGE:
            return 1024; // Allow reasonable text data
        case MessageType::PLAYER_INPUT:
            return 100;
        case MessageType::PLAYER_STATE_UPDATE:
            return 100;
        case MessageType::PLAYER_SPAWN:
            return 512;
        case MessageType::CHUNK_DATA:
            return 64 * 1024; // Chunks can be larger
        default:
            return 1024;
    }
}

} // namespace network
} // namespace clonemine

#include "PacketValidator.h"
#include <algorithm>
#include <iostream>

namespace clonemine {
namespace network {

PacketValidator::ValidationResult PacketValidator::validatePacket(
    const std::vector<uint8_t>& data, 
    MessageType expectedType) {
    
    // Check for empty or null data
    if (data.empty()) {
        std::cout << "[PacketValidator] Error: NULL_DATA (empty vector)" << std::endl;
        return ValidationResult::NULL_DATA;
    }
    
    // Check overall size
    if (data.size() > MAX_PACKET_SIZE) {
        std::cout << "[PacketValidator] Error: INVALID_SIZE (exceeds MAX_PACKET_SIZE: " << data.size() << ")" << std::endl;
        return ValidationResult::INVALID_SIZE;
    }
    
    // Validate message type
    MessageType actualType = static_cast<MessageType>(data[0]);
    if (actualType != expectedType) {
        std::cout << "[PacketValidator] Error: INVALID_TYPE (Expected " << static_cast<int>(expectedType) << ", got " << static_cast<int>(actualType) << ")" << std::endl;
        return ValidationResult::INVALID_TYPE;
    }
    
    // Validate size against expected range
    if (!validateSize(actualType, data.size())) {
        std::cout << "[PacketValidator] Error: INVALID_SIZE (Size " << data.size() << " out of bounds for type " << static_cast<int>(actualType) << ")" << std::endl;
        return ValidationResult::INVALID_SIZE;
    }
    
    // Check for nulls in critical data
    if (hasNullInCriticalData(data, actualType)) {
        std::cout << "[PacketValidator] Error: NULL_DATA / Validation failed in hasNullInCriticalData for type " << static_cast<int>(actualType) << std::endl;
        return ValidationResult::NULL_DATA;
    }
    
    return ValidationResult::VALID;
}

bool PacketValidator::validateSize(MessageType type, size_t actualSize) {
    size_t minSize = getMinimumSize(type);
    size_t maxSize = getMaximumSize(type);
    
    bool valid = actualSize >= minSize && actualSize <= maxSize;
    if (!valid) {
        std::cout << "[validateSize] Failed for type " << static_cast<int>(type) << ": actual size = " << actualSize 
                  << " (min: " << minSize << ", max: " << maxSize << ")" << std::endl;
    }
    return valid;
}

bool PacketValidator::hasNullInCriticalData(const std::vector<uint8_t>& data, MessageType type) {
    // Check based on message type
    switch (type) {
        case MessageType::CONNECT_REQUEST:
        case MessageType::CONNECT_RESPONSE:
        case MessageType::CHAT_MESSAGE: {
            // These have string data - check for proper string formatting
            // Strings should have length prefix, not null-terminated
            
            // CHAT_MESSAGE starts right after type (offset 1), whereas CONNECT_REQUEST has playerId at offset 1-4.
            size_t offset = (type == MessageType::CHAT_MESSAGE) ? 1 : 5; 
            
            if (data.size() < offset + 4) {
                std::cout << "[hasNullInCriticalData] Packet too small for string headers. Size: " << data.size() << std::endl;
                return true;
            }
            
            while (offset + 4 <= data.size()) {
                uint32_t strLen = data[offset] | (data[offset+1] << 8) | 
                                 (data[offset+2] << 16) | (data[offset+3] << 24);
                offset += 4;
                
                // Check if string length is reasonable
                if (strLen > MAX_STRING_LENGTH) {
                    std::cout << "[hasNullInCriticalData] Suspicious string length: " << strLen << std::endl;
                    return true; // Suspicious
                }
                
                // Skip the string data
                offset += strLen;
                if (offset > data.size()) {
                    std::cout << "[hasNullInCriticalData] String bounds exceed packet size. Offset: " << offset << ", Packet Size: " << data.size() << std::endl;
                    return true; // String extends beyond packet
                }
            }
            break;
        }
        
        case MessageType::PLAYER_INPUT:
        case MessageType::PLAYER_STATE_UPDATE: {
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
            return 10; 
        case MessageType::CONNECT_RESPONSE:
            return 10; 
        case MessageType::DISCONNECT:
            return 1;  
        case MessageType::PLAYER_INPUT:
            return 50; 
        case MessageType::PLAYER_STATE_UPDATE:
            return 50; 
        case MessageType::PLAYER_SPAWN:
            return 20; 
        case MessageType::PLAYER_DESPAWN:
            return 5;  
        case MessageType::CHAT_MESSAGE:
            return 9;  // type(1) + senderLen(4) + msgLen(4)
        default:
            return 1;
    }
}

size_t PacketValidator::getMaximumSize(MessageType type) {
    switch (type) {
        case MessageType::CONNECT_REQUEST:
        case MessageType::CONNECT_RESPONSE:
        case MessageType::CHAT_MESSAGE:
            return 1024; 
        case MessageType::PLAYER_INPUT:
            return 100;
        case MessageType::PLAYER_STATE_UPDATE:
            return 100;
        case MessageType::PLAYER_SPAWN:
            return 512;
        case MessageType::CHUNK_DATA:
            return 64 * 1024; 
        default:
            return 1024;
    }
}

} // namespace network
} // namespace clonemine
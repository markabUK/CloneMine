#include "NetworkMessage.h"
#include <cstring>

namespace clonemine {
namespace network {

// Helper functions for serialization
static void writeUint32(std::vector<uint8_t>& buffer, uint32_t value) {
    buffer.push_back(static_cast<uint8_t>(value & 0xFF));
    buffer.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
    buffer.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
    buffer.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
}

static void writeFloat(std::vector<uint8_t>& buffer, float value) {
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(float));
    writeUint32(buffer, bits);
}

static void writeVec3(std::vector<uint8_t>& buffer, const glm::vec3& vec) {
    writeFloat(buffer, vec.x);
    writeFloat(buffer, vec.y);
    writeFloat(buffer, vec.z);
}

static void writeString(std::vector<uint8_t>& buffer, const std::string& str) {
    writeUint32(buffer, static_cast<uint32_t>(str.size()));
    buffer.insert(buffer.end(), str.begin(), str.end());
}

// ConnectRequest implementation
std::vector<uint8_t> ConnectRequest::serialize() const {
    std::vector<uint8_t> buffer;
    buffer.reserve(getSize());
    
    buffer.push_back(static_cast<uint8_t>(type));
    writeUint32(buffer, playerId);
    writeString(buffer, playerName);
    
    return buffer;
}

// ConnectResponse implementation
std::vector<uint8_t> ConnectResponse::serialize() const {
    std::vector<uint8_t> buffer;
    buffer.reserve(getSize());
    
    buffer.push_back(static_cast<uint8_t>(type));
    buffer.push_back(accepted ? 1 : 0);
    writeUint32(buffer, assignedPlayerId);
    writeString(buffer, message);
    
    return buffer;
}

// PlayerInput implementation
std::vector<uint8_t> PlayerInput::serialize() const {
    std::vector<uint8_t> buffer;
    buffer.reserve(getSize());
    
    buffer.push_back(static_cast<uint8_t>(type));
    writeUint32(buffer, playerId);
    writeVec3(buffer, movement);
    writeFloat(buffer, yaw);
    writeFloat(buffer, pitch);
    buffer.push_back(jump ? 1 : 0);
    buffer.push_back(crouch ? 1 : 0);
    writeUint32(buffer, timestamp);
    
    return buffer;
}

// PlayerStateUpdate implementation
std::vector<uint8_t> PlayerStateUpdate::serialize() const {
    std::vector<uint8_t> buffer;
    buffer.reserve(getSize());
    
    buffer.push_back(static_cast<uint8_t>(type));
    writeUint32(buffer, playerId);
    writeVec3(buffer, position);
    writeVec3(buffer, velocity);
    writeFloat(buffer, yaw);
    writeFloat(buffer, pitch);
    writeFloat(buffer, health);
    writeFloat(buffer, resource);
    writeUint32(buffer, timestamp);
    
    return buffer;
}

// PlayerSpawn implementation
std::vector<uint8_t> PlayerSpawn::serialize() const {
    std::vector<uint8_t> buffer;
    buffer.reserve(getSize());
    
    buffer.push_back(static_cast<uint8_t>(type));
    writeUint32(buffer, playerId);
    writeVec3(buffer, position);
    writeString(buffer, playerName);
    writeString(buffer, className);
    
    return buffer;
}

// ChatMessage implementation
std::vector<uint8_t> ChatMessage::serialize() const {
    std::vector<uint8_t> buffer;
    buffer.reserve(getSize());
    
    buffer.push_back(static_cast<uint8_t>(type));
    writeString(buffer, sender);
    writeString(buffer, message);
    
    return buffer;
}

} // namespace network
} // namespace clonemine

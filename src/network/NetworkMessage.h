#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace clonemine {
namespace network {

// Message types for client-server communication
enum class MessageType : uint8_t {
    // Connection
    CONNECT_REQUEST = 0,
    CONNECT_RESPONSE = 1,
    DISCONNECT = 2,
    
    // Player state
    PLAYER_INPUT = 10,
    PLAYER_STATE_UPDATE = 11,
    PLAYER_SPAWN = 12,
    PLAYER_DESPAWN = 13,
    
    // World updates
    WORLD_STATE = 20,
    BLOCK_UPDATE = 21,
    CHUNK_DATA = 22,
    
    // Game events
    DAMAGE_EVENT = 30,
    ITEM_PICKUP = 31,
    SPELL_CAST = 32,
    
    // Chat
    CHAT_MESSAGE = 40
};

// Base message structure
struct NetworkMessage {
    MessageType type;
    uint32_t playerId{0};
    
    virtual ~NetworkMessage() = default;
    
    // Serialize to byte buffer
    virtual std::vector<uint8_t> serialize() const = 0;
    
    // Get message size for pre-allocation
    virtual size_t getSize() const = 0;
};

// Connect request from client
struct ConnectRequest : NetworkMessage {
    std::string playerName;
    
    ConnectRequest() { type = MessageType::CONNECT_REQUEST; }
    
    std::vector<uint8_t> serialize() const override;
    size_t getSize() const override { return sizeof(MessageType) + sizeof(uint32_t) + playerName.size(); }
};

// Connect response from server
struct ConnectResponse : NetworkMessage {
    bool accepted{false};
    uint32_t assignedPlayerId{0};
    std::string message;
    
    ConnectResponse() { type = MessageType::CONNECT_RESPONSE; }
    
    std::vector<uint8_t> serialize() const override;
    size_t getSize() const override { return sizeof(MessageType) + sizeof(bool) + sizeof(uint32_t) + sizeof(uint32_t) + message.size(); }
};

// Player input from client
struct PlayerInput : NetworkMessage {
    glm::vec3 movement{0.0f};
    float yaw{0.0f};
    float pitch{0.0f};
    bool jump{false};
    bool crouch{false};
    uint32_t timestamp{0};
    
    PlayerInput() { type = MessageType::PLAYER_INPUT; }
    
    std::vector<uint8_t> serialize() const override;
    size_t getSize() const override { return sizeof(MessageType) + sizeof(uint32_t) + sizeof(glm::vec3) + sizeof(float) * 2 + sizeof(bool) * 2 + sizeof(uint32_t); }
};

// Player state update from server
struct PlayerStateUpdate : NetworkMessage {
    glm::vec3 position{0.0f};
    glm::vec3 velocity{0.0f};
    float yaw{0.0f};
    float pitch{0.0f};
    float health{100.0f};
    float resource{100.0f};
    uint32_t timestamp{0};
    
    PlayerStateUpdate() { type = MessageType::PLAYER_STATE_UPDATE; }
    
    std::vector<uint8_t> serialize() const override;
    size_t getSize() const override { return sizeof(MessageType) + sizeof(uint32_t) * 2 + sizeof(glm::vec3) * 2 + sizeof(float) * 4 + sizeof(uint32_t); }
};

// Player spawn notification
struct PlayerSpawn : NetworkMessage {
    glm::vec3 position{0.0f};
    std::string playerName;
    std::string className;
    
    PlayerSpawn() { type = MessageType::PLAYER_SPAWN; }
    
    std::vector<uint8_t> serialize() const override;
    size_t getSize() const override { return sizeof(MessageType) + sizeof(uint32_t) + sizeof(glm::vec3) + sizeof(uint32_t) * 2 + playerName.size() + className.size(); }
};

// Chat message
struct ChatMessage : NetworkMessage {
    std::string sender;
    std::string message;
    
    ChatMessage() { type = MessageType::CHAT_MESSAGE; }
    
    std::vector<uint8_t> serialize() const override;
    size_t getSize() const override { return sizeof(MessageType) + sizeof(uint32_t) * 2 + sender.size() + message.size(); }
};

} // namespace network
} // namespace clonemine

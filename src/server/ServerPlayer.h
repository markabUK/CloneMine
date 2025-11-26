#pragma once

#include "../world/Player.h"
#include "../network/PacketEncryption.h"
#include <memory>
#include <asio.hpp>

namespace clonemine {
namespace server {

// Server-side player session
class ServerPlayer {
public:
    ServerPlayer(uint32_t id, std::shared_ptr<asio::ip::tcp::socket> socket);
    ~ServerPlayer() = default;
    
    // Delete copy operations
    ServerPlayer(const ServerPlayer&) = delete;
    ServerPlayer& operator=(const ServerPlayer&) = delete;
    
    // Getters
    uint32_t getId() const { return m_id; }
    const std::string& getName() const { return m_name; }
    Player& getPlayer() { return m_player; }
    const Player& getPlayer() const { return m_player; }
    
    // Setters
    void setName(const std::string& name) { m_name = name; }
    
    // Network operations
    void sendData(const std::vector<uint8_t>& data);
    bool isConnected() const { return m_connected; }
    void disconnect();
    
    // Disconnect grace period (15 seconds)
    void startDisconnectGracePeriod();
    bool isInGracePeriod() const { return m_inGracePeriod; }
    bool hasGracePeriodExpired() const;
    float getGracePeriodRemaining() const;
    void cancelGracePeriod(); // Called if player reconnects
    bool shouldIgnoreActions() const { return m_inGracePeriod; }
    
    // Update
    void update(float deltaTime);
    
private:
    uint32_t m_id;
    std::string m_name;
    Player m_player;
    std::shared_ptr<asio::ip::tcp::socket> m_socket;
    bool m_connected{true};
    
    // Disconnect grace period (15 seconds)
    bool m_inGracePeriod{false};
    float m_gracePeriodStart{0.0f};
    static constexpr float DISCONNECT_GRACE_PERIOD = 15.0f; // 15 seconds
    
    // Encryption
    std::unique_ptr<network::PacketEncryption> m_encryption;
};

} // namespace server
} // namespace clonemine

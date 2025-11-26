#include "ServerPlayer.h"
#include <iostream>
#include <chrono>

namespace clonemine {
namespace server {

namespace {
    float getCurrentTimeSeconds() {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
        return static_cast<float>(duration.count()) / 1000.0f;
    }
}

ServerPlayer::ServerPlayer(uint32_t id, std::shared_ptr<asio::ip::tcp::socket> socket)
    : m_id(id)
    , m_socket(std::move(socket))
{
    // Initialize player at spawn position
    m_player.setPosition(glm::vec3(0.0f, 100.0f, 0.0f));
    
    // Initialize encryption with the same shared secret
    m_encryption = std::make_unique<network::PacketEncryption>("CloneMineSharedSecret2024");
}

void ServerPlayer::sendData(const std::vector<uint8_t>& data) {
    if (!m_connected || !m_socket || !m_socket->is_open()) {
        return;
    }
    
    try {
        // Make a copy to encrypt (don't modify original)
        std::vector<uint8_t> encryptedData = data;
        m_encryption->encrypt(encryptedData);
        
        // Send data size first (4 bytes)
        uint32_t size = static_cast<uint32_t>(encryptedData.size());
        std::vector<uint8_t> sizeBuffer(4);
        sizeBuffer[0] = static_cast<uint8_t>(size & 0xFF);
        sizeBuffer[1] = static_cast<uint8_t>((size >> 8) & 0xFF);
        sizeBuffer[2] = static_cast<uint8_t>((size >> 16) & 0xFF);
        sizeBuffer[3] = static_cast<uint8_t>((size >> 24) & 0xFF);
        
        asio::write(*m_socket, asio::buffer(sizeBuffer));
        asio::write(*m_socket, asio::buffer(encryptedData));
    } catch (const std::exception& e) {
        std::cerr << "Error sending data to player " << m_id << ": " << e.what() << std::endl;
        m_connected = false;
    }
}

void ServerPlayer::disconnect() {
    m_connected = false;
    if (m_socket && m_socket->is_open()) {
        try {
            m_socket->shutdown(asio::ip::tcp::socket::shutdown_both);
            m_socket->close();
        } catch (const std::exception& e) {
            std::cerr << "Error disconnecting player " << m_id << ": " << e.what() << std::endl;
        }
    }
}

void ServerPlayer::startDisconnectGracePeriod() {
    if (!m_inGracePeriod) {
        m_inGracePeriod = true;
        m_gracePeriodStart = getCurrentTimeSeconds();
        std::cout << "Player " << m_id << " (" << m_name << ") starting 15-second disconnect grace period" << std::endl;
    }
}

bool ServerPlayer::hasGracePeriodExpired() const {
    if (!m_inGracePeriod) {
        return false;
    }
    return (getCurrentTimeSeconds() - m_gracePeriodStart) >= DISCONNECT_GRACE_PERIOD;
}

float ServerPlayer::getGracePeriodRemaining() const {
    if (!m_inGracePeriod) {
        return DISCONNECT_GRACE_PERIOD;
    }
    float elapsed = getCurrentTimeSeconds() - m_gracePeriodStart;
    float remaining = DISCONNECT_GRACE_PERIOD - elapsed;
    return remaining > 0 ? remaining : 0;
}

void ServerPlayer::cancelGracePeriod() {
    if (m_inGracePeriod) {
        m_inGracePeriod = false;
        m_gracePeriodStart = 0.0f;
        std::cout << "Player " << m_id << " (" << m_name << ") grace period cancelled - reconnected" << std::endl;
    }
}

void ServerPlayer::update(float deltaTime) {
    // Update player logic
    m_player.update(deltaTime);
}

} // namespace server
} // namespace clonemine

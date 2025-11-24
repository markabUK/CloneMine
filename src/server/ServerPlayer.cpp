#include "ServerPlayer.h"
#include <iostream>

namespace clonemine {
namespace server {

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

void ServerPlayer::update(float deltaTime) {
    // Update player logic
    m_player.update(deltaTime);
}

} // namespace server
} // namespace clonemine

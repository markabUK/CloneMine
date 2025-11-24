#include "NetworkClient.h"
#include <iostream>

namespace clonemine {
namespace client {

NetworkClient::NetworkClient() {
    // Initialize encryption with a shared secret key
    // TODO: In production, use proper key exchange mechanism
    m_encryption = std::make_unique<network::PacketEncryption>("CloneMineSharedSecret2024");
}

NetworkClient::~NetworkClient() {
    disconnect();
}

bool NetworkClient::connect(const std::string& host, uint16_t port, const std::string& playerName) {
    try {
        std::cout << "Connecting to " << host << ":" << port << "..." << std::endl;
        
        // Create socket
        m_socket = std::make_unique<asio::ip::tcp::socket>(m_ioContext);
        
        // Resolve and connect
        asio::ip::tcp::resolver resolver(m_ioContext);
        auto endpoints = resolver.resolve(host, std::to_string(port));
        asio::connect(*m_socket, endpoints);
        
        std::cout << "Connected to server (encrypted channel)" << std::endl;
        
        // Send connect request
        network::ConnectRequest request;
        request.playerName = playerName;
        auto data = request.serialize();
        
        // Encrypt the data
        m_encryption->encrypt(data);
        
        // Send data size first
        uint32_t size = static_cast<uint32_t>(data.size());
        std::vector<uint8_t> sizeBuffer(4);
        sizeBuffer[0] = static_cast<uint8_t>(size & 0xFF);
        sizeBuffer[1] = static_cast<uint8_t>((size >> 8) & 0xFF);
        sizeBuffer[2] = static_cast<uint8_t>((size >> 16) & 0xFF);
        sizeBuffer[3] = static_cast<uint8_t>((size >> 24) & 0xFF);
        
        asio::write(*m_socket, asio::buffer(sizeBuffer));
        asio::write(*m_socket, asio::buffer(data));
        
        // Wait for response
        std::vector<uint8_t> responseSizeBuffer(4);
        asio::read(*m_socket, asio::buffer(responseSizeBuffer));
        
        uint32_t responseSize = responseSizeBuffer[0] |
                               (responseSizeBuffer[1] << 8) |
                               (responseSizeBuffer[2] << 16) |
                               (responseSizeBuffer[3] << 24);
        
        std::vector<uint8_t> responseData(responseSize);
        asio::read(*m_socket, asio::buffer(responseData));
        
        // Decrypt the response
        m_encryption->decrypt(responseData);
        
        // Parse response
        if (responseData.size() >= 6 && 
            responseData[0] == static_cast<uint8_t>(network::MessageType::CONNECT_RESPONSE)) {
            
            bool accepted = responseData[1] != 0;
            m_playerId = responseData[2] |
                        (responseData[3] << 8) |
                        (responseData[4] << 16) |
                        (responseData[5] << 24);
            
            if (accepted) {
                m_connected = true;
                std::cout << "Connection accepted! Player ID: " << m_playerId << std::endl;
                
                // Start receiving messages in background thread
                m_networkThread = std::thread([this]() {
                    receiveMessages();
                });
                
                return true;
            } else {
                std::cerr << "Connection rejected by server" << std::endl;
                m_socket->close();
                return false;
            }
        }
        
        std::cerr << "Invalid server response" << std::endl;
        m_socket->close();
        return false;
        
    } catch (const std::exception& e) {
        std::cerr << "Connection failed: " << e.what() << std::endl;
        m_connected = false;
        return false;
    }
}

void NetworkClient::disconnect() {
    if (!m_connected) {
        return;
    }
    
    std::cout << "Disconnecting from server..." << std::endl;
    m_connected = false;
    
    if (m_socket && m_socket->is_open()) {
        try {
            // Send disconnect message
            std::vector<uint8_t> disconnectMsg = {
                static_cast<uint8_t>(network::MessageType::DISCONNECT)
            };
            
            uint32_t size = static_cast<uint32_t>(disconnectMsg.size());
            std::vector<uint8_t> sizeBuffer(4);
            sizeBuffer[0] = static_cast<uint8_t>(size & 0xFF);
            sizeBuffer[1] = static_cast<uint8_t>((size >> 8) & 0xFF);
            sizeBuffer[2] = static_cast<uint8_t>((size >> 16) & 0xFF);
            sizeBuffer[3] = static_cast<uint8_t>((size >> 24) & 0xFF);
            
            asio::write(*m_socket, asio::buffer(sizeBuffer));
            asio::write(*m_socket, asio::buffer(disconnectMsg));
            
            m_socket->shutdown(asio::ip::tcp::socket::shutdown_both);
            m_socket->close();
        } catch (const std::exception& e) {
            std::cerr << "Error during disconnect: " << e.what() << std::endl;
        }
    }
    
    if (m_networkThread.joinable()) {
        m_networkThread.join();
    }
}

void NetworkClient::sendMessage(const network::NetworkMessage& message) {
    if (!m_connected || !m_socket || !m_socket->is_open()) {
        return;
    }
    
    try {
        auto data = message.serialize();
        
        // Encrypt the data before sending
        m_encryption->encrypt(data);
        
        // Send data size first
        uint32_t size = static_cast<uint32_t>(data.size());
        std::vector<uint8_t> sizeBuffer(4);
        sizeBuffer[0] = static_cast<uint8_t>(size & 0xFF);
        sizeBuffer[1] = static_cast<uint8_t>((size >> 8) & 0xFF);
        sizeBuffer[2] = static_cast<uint8_t>((size >> 16) & 0xFF);
        sizeBuffer[3] = static_cast<uint8_t>((size >> 24) & 0xFF);
        
        asio::write(*m_socket, asio::buffer(sizeBuffer));
        asio::write(*m_socket, asio::buffer(data));
        
    } catch (const std::exception& e) {
        std::cerr << "Error sending message: " << e.what() << std::endl;
        m_connected = false;
    }
}

void NetworkClient::receiveMessages() {
    while (m_connected) {
        try {
            // Read message size
            std::vector<uint8_t> sizeBuffer(4);
            asio::read(*m_socket, asio::buffer(sizeBuffer));
            
            uint32_t messageSize = sizeBuffer[0] |
                                  (sizeBuffer[1] << 8) |
                                  (sizeBuffer[2] << 16) |
                                  (sizeBuffer[3] << 24);
            
            if (messageSize == 0 || messageSize > 1024 * 1024) { // Max 1MB
                std::cerr << "Invalid message size: " << messageSize << std::endl;
                m_connected = false;
                break;
            }
            
            // Read message data
            std::vector<uint8_t> messageData(messageSize);
            asio::read(*m_socket, asio::buffer(messageData));
            
            // Decrypt the received data
            m_encryption->decrypt(messageData);
            
            // Queue message for processing
            {
                std::lock_guard<std::mutex> lock(m_queueMutex);
                m_messageQueue.push(std::move(messageData));
            }
            
        } catch (const std::exception& e) {
            if (m_connected) {
                std::cerr << "Error receiving message: " << e.what() << std::endl;
                m_connected = false;
            }
            break;
        }
    }
}

void NetworkClient::processMessages() {
    std::lock_guard<std::mutex> lock(m_queueMutex);
    
    while (!m_messageQueue.empty()) {
        auto& data = m_messageQueue.front();
        handleMessage(data);
        m_messageQueue.pop();
    }
}

void NetworkClient::handleMessage(const std::vector<uint8_t>& data) {
    if (m_messageCallback) {
        m_messageCallback(data);
    }
}

} // namespace client
} // namespace clonemine

#include "ChatServer.h"
#include "../network/PacketValidator.h"
#include <iostream>
#include <chrono>

namespace clonemine {
namespace server {

ChatServer::ChatServer(uint16_t port)
    : m_port(port)
{
    std::cout << "Initializing chat server on port " << port << "..." << std::endl;
}

ChatServer::~ChatServer() {
    stop();
}

void ChatServer::start() {
    if (m_running) {
        return;
    }
    
    m_running = true;
    
    try {
        // Setup acceptor
        asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), m_port);
        m_acceptor = std::make_unique<asio::ip::tcp::acceptor>(m_ioContext, endpoint);
        
        std::cout << "Chat server listening on port " << m_port << std::endl;
        
        // Start accepting connections in a separate thread
        m_networkThread = std::thread([this]() {
            acceptConnections();
            m_ioContext.run();
        });
        
    } catch (const std::exception& e) {
        std::cerr << "Failed to start chat server: " << e.what() << std::endl;
        m_running = false;
        throw;
    }
}

void ChatServer::stop() {
    if (!m_running) {
        return;
    }
    
    std::cout << "Stopping chat server..." << std::endl;
    m_running = false;
    
    // Explicitly close all client sockets to unblock any blocking read threads
    {
        std::lock_guard<std::mutex> lock(m_clientsMutex);
        for (auto& [id, client] : m_clients) {
            if (client->socket && client->socket->is_open()) {
                asio::error_code ignored;
                client->socket->close(ignored);
            }
        }
        m_clients.clear();
    }
    
    // Stop network acceptor
    if (m_acceptor) {
        asio::error_code ignored;
        m_acceptor->close(ignored);
    }
    m_ioContext.stop();
    
    if (m_networkThread.joinable()) {
        m_networkThread.join();
    }
    
    std::cout << "Chat server stopped." << std::endl;
}

void ChatServer::run() {
    std::cout << "Chat server main loop started." << std::endl;
    
    while (m_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        std::lock_guard<std::mutex> lock(m_clientsMutex);
        for (auto it = m_clients.begin(); it != m_clients.end(); ) {
            if (!it->second->connected) {
                std::cout << "Removing disconnected client " << it->first << std::endl;
                it = m_clients.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void ChatServer::acceptConnections() {
    auto socket = std::make_shared<asio::ip::tcp::socket>(m_ioContext);
    
    m_acceptor->async_accept(*socket, [this, socket](const asio::error_code& error) {
        if (!error) {
            std::cout << "New chat connection from " << socket->remote_endpoint() << std::endl;
            handleNewConnection(socket);
        } else {
            if (m_running) {
                std::cerr << "Chat accept error: " << error.message() << std::endl;
            }
        }
        
        if (m_running) {
            acceptConnections();
        }
    });
}

void ChatServer::handleNewConnection(std::shared_ptr<asio::ip::tcp::socket> socket) {
    try {
        auto encryption = std::make_unique<network::PacketEncryption>("CloneMineSharedSecret2024");
        
        std::vector<uint8_t> sizeBuffer(4);
        asio::read(*socket, asio::buffer(sizeBuffer));
        
        uint32_t messageSize = sizeBuffer[0] | (sizeBuffer[1] << 8) |
                              (sizeBuffer[2] << 16) | (sizeBuffer[3] << 24);
        
        if (messageSize == 0 || messageSize > 1024) {
            socket->close();
            return;
        }
        
        std::vector<uint8_t> buffer(messageSize);
        asio::read(*socket, asio::buffer(buffer));
        encryption->decrypt(buffer);
        
        auto validationResult = network::PacketValidator::validatePacket(
            buffer, network::MessageType::CONNECT_REQUEST);
        
        if (validationResult != network::PacketValidator::ValidationResult::VALID) {
            socket->close();
            return;
        }
        
        if (buffer.size() < 9) {
            socket->close();
            return;
        }
        
        uint32_t nameLen = buffer[5] | (buffer[6] << 8) | (buffer[7] << 16) | (buffer[8] << 24);
        if (9 + nameLen > buffer.size()) {
            socket->close();
            return;
        }
        
        std::string playerName(buffer.begin() + 9, buffer.begin() + 9 + nameLen);
        
        uint32_t clientId = m_nextClientId++;
        auto client = std::make_unique<ChatClient>();
        client->playerId = clientId;
        client->playerName = playerName;
        client->socket = socket;
        client->encryption = std::move(encryption);
        
        network::ConnectResponse response;
        response.accepted = true;
        response.assignedPlayerId = clientId;
        response.message = "Connected to chat server";
        
        auto responseData = response.serialize();
        client->encryption->encrypt(responseData);
        
        uint32_t size = static_cast<uint32_t>(responseData.size());
        std::vector<uint8_t> sizeBytes(4);
        sizeBytes[0] = static_cast<uint8_t>(size & 0xFF);
        sizeBytes[1] = static_cast<uint8_t>((size >> 8) & 0xFF);
        sizeBytes[2] = static_cast<uint8_t>((size >> 16) & 0xFF);
        sizeBytes[3] = static_cast<uint8_t>((size >> 24) & 0xFF);
        
        asio::write(*socket, asio::buffer(sizeBytes));
        asio::write(*socket, asio::buffer(responseData));
        
        {
            std::lock_guard<std::mutex> historyLock(m_historyMutex);
            for (const auto& [sender, msg] : m_chatHistory) {
                network::ChatMessage chatMsg;
                chatMsg.sender = sender;
                chatMsg.message = msg;
                auto data = chatMsg.serialize();
                client->encryption->encrypt(data);
                
                size = static_cast<uint32_t>(data.size());
                sizeBytes[0] = static_cast<uint8_t>(size & 0xFF);
                sizeBytes[1] = static_cast<uint8_t>((size >> 8) & 0xFF);
                sizeBytes[2] = static_cast<uint8_t>((size >> 16) & 0xFF);
                sizeBytes[3] = static_cast<uint8_t>((size >> 24) & 0xFF);
                
                try {
                    asio::write(*socket, asio::buffer(sizeBytes));
                    asio::write(*socket, asio::buffer(data));
                } catch (...) {
                    break;
                }
            }
        }
        
        {
            std::lock_guard<std::mutex> lock(m_clientsMutex);
            m_clients[clientId] = std::move(client);
        }
        
        std::cout << "Chat client " << clientId << " (" << playerName << ") connected" << std::endl;
        
        std::thread([this, clientId, socket]() {
            while (m_running) {
                try {
                    std::vector<uint8_t> sizeBuffer(4);
                    asio::read(*socket, asio::buffer(sizeBuffer));
                    
                    uint32_t msgSize = sizeBuffer[0] | (sizeBuffer[1] << 8) |
                                      (sizeBuffer[2] << 16) | (sizeBuffer[3] << 24);
                    
                    if (msgSize == 0 || msgSize > 1024) {
                        break;
                    }
                    
                    std::vector<uint8_t> data(msgSize);
                    asio::read(*socket, asio::buffer(data));
                    
                    bool isValid = false;
                    {
                        std::lock_guard<std::mutex> lock(m_clientsMutex);
                        auto it = m_clients.find(clientId);
                        if (it != m_clients.end()) {
                            it->second->encryption->decrypt(data);
                            auto validation = network::PacketValidator::validatePacket(
                                data, network::MessageType::CHAT_MESSAGE);
                            isValid = (validation == network::PacketValidator::ValidationResult::VALID);
                        }
                    }
                    
                    if (isValid) {
                        handleChatMessage(clientId, data);
                    } else {
                        std::cerr << "[ERROR] Chat validation failed for client " << clientId << std::endl;
                    }
                } catch (const std::exception& e) {
                    if (m_running) {
                        std::cout << "[DEBUG] Reader thread exception for client " << clientId << ": " << e.what() << std::endl;
                    }
                    break;
                } catch (...) {
                    break;
                }
            }
            
            std::lock_guard<std::mutex> lock(m_clientsMutex);
            auto it = m_clients.find(clientId);
            if (it != m_clients.end()) {
                it->second->connected = false;
            }
        }).detach();
        
    } catch (const std::exception& e) {
        if (m_running) {
            std::cerr << "Error handling chat connection: " << e.what() << std::endl;
        }
    }
}

void ChatServer::handleChatMessage(uint32_t playerId, const std::vector<uint8_t>& data) {
    if (data.size() < 10) return;
    
    size_t offset = 1;
    uint32_t senderLen = data[offset] | (data[offset+1] << 8) |
                        (data[offset+2] << 16) | (data[offset+3] << 24);
    offset += 4;
    
    if (offset + senderLen + 4 > data.size()) return;
    std::string sender(data.begin() + offset, data.begin() + offset + senderLen);
    offset += senderLen;
    
    uint32_t msgLen = data[offset] | (data[offset+1] << 8) |
                     (data[offset+2] << 16) | (data[offset+3] << 24);
    offset += 4;
    
    if (offset + msgLen > data.size()) return;
    std::string message(data.begin() + offset, data.begin() + offset + msgLen);
    
    std::string actualSender;
    {
        std::lock_guard<std::mutex> lock(m_clientsMutex);
        auto it = m_clients.find(playerId);
        if (it != m_clients.end()) {
            actualSender = it->second->playerName;
        }
    }
    
    if (!actualSender.empty()) {
        broadcastMessage(actualSender, message);
    }
}

void ChatServer::broadcastMessage(const std::string& sender, const std::string& message) {
    std::cout << "[CHAT] " << sender << ": " << message << std::endl;
    
    {
        std::lock_guard<std::mutex> lock(m_historyMutex);
        m_chatHistory.push_back({sender, message});
        if (m_chatHistory.size() > MAX_HISTORY) {
            m_chatHistory.pop_front();
        }
    }
    
    network::ChatMessage chatMsg;
    chatMsg.sender = sender;
    chatMsg.message = message;
    auto data = chatMsg.serialize();
    
    std::lock_guard<std::mutex> lock(m_clientsMutex);
    for (auto& [id, client] : m_clients) {
        if (!client->connected) continue;
        
        try {
            auto encryptedData = data;
            client->encryption->encrypt(encryptedData);
            
            uint32_t size = static_cast<uint32_t>(encryptedData.size());
            std::vector<uint8_t> sizeBytes(4);
            sizeBytes[0] = static_cast<uint8_t>(size & 0xFF);
            sizeBytes[1] = static_cast<uint8_t>((size >> 8) & 0xFF);
            sizeBytes[2] = static_cast<uint8_t>((size >> 16) & 0xFF);
            sizeBytes[3] = static_cast<uint8_t>((size >> 24) & 0xFF);
            
            asio::write(*client->socket, asio::buffer(sizeBytes));
            asio::write(*client->socket, asio::buffer(encryptedData));
        } catch (...) {
            client->connected = false;
        }
    }
}

void ChatServer::disconnectClient(uint32_t playerId) {
    std::lock_guard<std::mutex> lock(m_clientsMutex);
    m_clients.erase(playerId);
}

} // namespace server
} // namespace clonemine
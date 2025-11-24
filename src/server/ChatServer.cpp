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
    
    // Disconnect all clients
    {
        std::lock_guard<std::mutex> lock(m_clientsMutex);
        m_clients.clear();
    }
    
    // Stop network
    if (m_acceptor) {
        m_acceptor->close();
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
        // Chat server is event-driven, just keep alive
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Clean up disconnected clients
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
            std::cerr << "Chat accept error: " << error.message() << std::endl;
        }
        
        // Continue accepting
        if (m_running) {
            acceptConnections();
        }
    });
}

void ChatServer::handleNewConnection(std::shared_ptr<asio::ip::tcp::socket> socket) {
    try {
        auto encryption = std::make_unique<network::PacketEncryption>("CloneMineSharedSecret2024");
        
        // Read connect message (with size prefix)
        std::vector<uint8_t> sizeBuffer(4);
        asio::read(*socket, asio::buffer(sizeBuffer));
        
        uint32_t messageSize = sizeBuffer[0] | (sizeBuffer[1] << 8) |
                              (sizeBuffer[2] << 16) | (sizeBuffer[3] << 24);
        
        if (messageSize == 0 || messageSize > 1024) {
            std::cerr << "Invalid chat connect message size" << std::endl;
            socket->close();
            return;
        }
        
        std::vector<uint8_t> buffer(messageSize);
        asio::read(*socket, asio::buffer(buffer));
        encryption->decrypt(buffer);
        
        // Validate packet
        auto validationResult = network::PacketValidator::validatePacket(
            buffer, network::MessageType::CONNECT_REQUEST);
        
        if (validationResult != network::PacketValidator::ValidationResult::VALID) {
            std::cerr << "Invalid chat connect packet" << std::endl;
            socket->close();
            return;
        }
        
        // Parse player name
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
        
        // Create chat client
        uint32_t clientId = m_nextClientId++;
        auto client = std::make_unique<ChatClient>();
        client->playerId = clientId;
        client->playerName = playerName;
        client->socket = socket;
        client->encryption = std::move(encryption);
        
        // Send acceptance response
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
        
        // Send chat history
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
                    break; // Client disconnected
                }
            }
        }
        
        // Add to clients
        {
            std::lock_guard<std::mutex> lock(m_clientsMutex);
            m_clients[clientId] = std::move(client);
        }
        
        std::cout << "Chat client " << clientId << " (" << playerName << ") connected" << std::endl;
        
        // Start receiving messages from this client
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
                    
                    // Decrypt and handle
                    std::lock_guard<std::mutex> lock(m_clientsMutex);
                    auto it = m_clients.find(clientId);
                    if (it != m_clients.end()) {
                        it->second->encryption->decrypt(data);
                        
                        // Validate
                        auto validation = network::PacketValidator::validatePacket(
                            data, network::MessageType::CHAT_MESSAGE);
                        
                        if (validation == network::PacketValidator::ValidationResult::VALID) {
                            handleChatMessage(clientId, data);
                        }
                    }
                } catch (...) {
                    break;
                }
            }
            
            // Mark as disconnected
            std::lock_guard<std::mutex> lock(m_clientsMutex);
            auto it = m_clients.find(clientId);
            if (it != m_clients.end()) {
                it->second->connected = false;
            }
        }).detach();
        
    } catch (const std::exception& e) {
        std::cerr << "Error handling chat connection: " << e.what() << std::endl;
    }
}

void ChatServer::handleChatMessage(uint32_t playerId, const std::vector<uint8_t>& data) {
    if (data.size() < 10) return;
    
    size_t offset = 1;
    
    // Parse sender
    uint32_t senderLen = data[offset] | (data[offset+1] << 8) |
                        (data[offset+2] << 16) | (data[offset+3] << 24);
    offset += 4;
    
    if (offset + senderLen + 4 > data.size()) return;
    
    std::string sender(data.begin() + offset, data.begin() + offset + senderLen);
    offset += senderLen;
    
    // Parse message
    uint32_t msgLen = data[offset] | (data[offset+1] << 8) |
                     (data[offset+2] << 16) | (data[offset+3] << 24);
    offset += 4;
    
    if (offset + msgLen > data.size()) return;
    
    std::string message(data.begin() + offset, data.begin() + offset + msgLen);
    
    // Get actual sender name from client
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
    
    // Add to history
    {
        std::lock_guard<std::mutex> lock(m_historyMutex);
        m_chatHistory.push_back({sender, message});
        if (m_chatHistory.size() > MAX_HISTORY) {
            m_chatHistory.pop_front();
        }
    }
    
    // Broadcast to all clients
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

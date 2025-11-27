#include "LoginServer.h"
#include "../network/PacketValidator.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>

namespace clonemine {
namespace server {

LoginServer::LoginServer(uint16_t port, uint32_t maxCharactersPerAccount)
    : m_port(port)
    , m_maxCharactersPerAccount(maxCharactersPerAccount)
    , m_rng(m_rd())
{
    std::cout << "Initializing login server on port " << port << "..." << std::endl;
    std::cout << "Max characters per account: " << maxCharactersPerAccount << std::endl;
    
    // Add some test users (in production, load from database)
    m_userDatabase["test"] = "test123"; // username:password (should be hashed!)
    m_userDatabase["admin"] = "admin123";
    m_userDatabase["player1"] = "password1";
}

LoginServer::~LoginServer() {
    stop();
}

void LoginServer::start() {
    if (m_running) {
        return;
    }
    
    m_running = true;
    
    try {
        // Setup acceptor
        asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), m_port);
        m_acceptor = std::make_unique<asio::ip::tcp::acceptor>(m_ioContext, endpoint);
        
        std::cout << "Login server listening on port " << m_port << std::endl;
        
        // Start accepting connections in a separate thread
        m_networkThread = std::thread([this]() {
            acceptConnections();
            m_ioContext.run();
        });
        
    } catch (const std::exception& e) {
        std::cerr << "Failed to start login server: " << e.what() << std::endl;
        m_running = false;
        throw;
    }
}

void LoginServer::stop() {
    if (!m_running) {
        return;
    }
    
    std::cout << "Stopping login server..." << std::endl;
    m_running = false;
    
    // Disconnect all sessions
    {
        std::lock_guard<std::mutex> lock(m_sessionsMutex);
        m_sessions.clear();
    }
    
    // Stop network
    if (m_acceptor) {
        m_acceptor->close();
    }
    m_ioContext.stop();
    
    if (m_networkThread.joinable()) {
        m_networkThread.join();
    }
    
    std::cout << "Login server stopped." << std::endl;
}

void LoginServer::run() {
    std::cout << "Login server main loop started." << std::endl;
    
    while (m_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Clean up disconnected sessions
        std::lock_guard<std::mutex> lock(m_sessionsMutex);
        for (auto it = m_sessions.begin(); it != m_sessions.end(); ) {
            if (!it->second->connected) {
                std::cout << "Removing disconnected session " << it->first << std::endl;
                it = m_sessions.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void LoginServer::acceptConnections() {
    auto socket = std::make_shared<asio::ip::tcp::socket>(m_ioContext);
    
    m_acceptor->async_accept(*socket, [this, socket](const asio::error_code& error) {
        if (!error) {
            std::cout << "New login connection from " << socket->remote_endpoint() << std::endl;
            handleNewConnection(socket);
        } else {
            std::cerr << "Login accept error: " << error.message() << std::endl;
        }
        
        // Continue accepting
        if (m_running) {
            acceptConnections();
        }
    });
}

void LoginServer::handleNewConnection(std::shared_ptr<asio::ip::tcp::socket> socket) {
    try {
        auto encryption = std::make_unique<network::PacketEncryption>("CloneMineSharedSecret2024");
        
        // Create session
        uint32_t sessionId = m_nextSessionId++;
        auto session = std::make_unique<LoginSession>();
        session->sessionId = sessionId;
        session->socket = socket;
        session->encryption = std::move(encryption);
        
        // Add to sessions
        {
            std::lock_guard<std::mutex> lock(m_sessionsMutex);
            m_sessions[sessionId] = std::move(session);
        }
        
        std::cout << "Login session " << sessionId << " created" << std::endl;
        
        // Start message handling thread
        std::thread([this, sessionId]() {
            std::lock_guard<std::mutex> lock(m_sessionsMutex);
            auto it = m_sessions.find(sessionId);
            if (it == m_sessions.end()) return;
            
            auto& session = it->second;
            
            // Send handshake challenge
            std::vector<uint8_t> challenge(32);
            for (size_t i = 0; i < challenge.size(); ++i) {
                challenge[i] = static_cast<uint8_t>(m_rng() & 0xFF);
            }
            
            try {
                session->encryption->encrypt(challenge);
                
                uint32_t size = static_cast<uint32_t>(challenge.size());
                std::vector<uint8_t> sizeBytes(4);
                sizeBytes[0] = static_cast<uint8_t>(size & 0xFF);
                sizeBytes[1] = static_cast<uint8_t>((size >> 8) & 0xFF);
                sizeBytes[2] = static_cast<uint8_t>((size >> 16) & 0xFF);
                sizeBytes[3] = static_cast<uint8_t>((size >> 24) & 0xFF);
                
                asio::write(*session->socket, asio::buffer(sizeBytes));
                asio::write(*session->socket, asio::buffer(challenge));
                
                std::cout << "Sent handshake challenge to session " << sessionId << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Failed to send handshake: " << e.what() << std::endl;
                session->connected = false;
                return;
            }
            
            // Receive messages
            while (m_running && session->connected) {
                try {
                    std::vector<uint8_t> sizeBuffer(4);
                    asio::read(*session->socket, asio::buffer(sizeBuffer));
                    
                    uint32_t msgSize = sizeBuffer[0] | (sizeBuffer[1] << 8) |
                                      (sizeBuffer[2] << 16) | (sizeBuffer[3] << 24);
                    
                    if (msgSize == 0 || msgSize > 1024) break;
                    
                    std::vector<uint8_t> data(msgSize);
                    asio::read(*session->socket, asio::buffer(data));
                    session->encryption->decrypt(data);
                    
                    // Handle based on authentication state
                    if (!session->authenticated) {
                        handleLoginRequest(sessionId, data);
                    }
                } catch (...) {
                    break;
                }
            }
            
            session->connected = false;
        }).detach();
        
    } catch (const std::exception& e) {
        std::cerr << "Error handling login connection: " << e.what() << std::endl;
    }
}

void LoginServer::handleHandshake(uint32_t sessionId, const std::vector<uint8_t>& data) {
    (void)sessionId;
    (void)data;
    // Handshake verification (simplified)
}

void LoginServer::handleLoginRequest(uint32_t sessionId, const std::vector<uint8_t>& data) {
    std::lock_guard<std::mutex> lock(m_sessionsMutex);
    auto it = m_sessions.find(sessionId);
    if (it == m_sessions.end()) return;
    
    auto& session = it->second;
    
    // Parse login request (simplified - should use proper message format)
    if (data.size() < 10) {
        std::cerr << "Invalid login request" << std::endl;
        session->connected = false;
        return;
    }
    
    // Parse username
    size_t offset = 1; // Skip message type
    uint32_t usernameLen = data[offset] | (data[offset+1] << 8) |
                          (data[offset+2] << 16) | (data[offset+3] << 24);
    offset += 4;
    
    if (offset + usernameLen + 4 > data.size()) {
        session->connected = false;
        return;
    }
    
    std::string username(data.begin() + offset, data.begin() + offset + usernameLen);
    offset += usernameLen;
    
    // Parse password
    uint32_t passwordLen = data[offset] | (data[offset+1] << 8) |
                          (data[offset+2] << 16) | (data[offset+3] << 24);
    offset += 4;
    
    if (offset + passwordLen > data.size()) {
        session->connected = false;
        return;
    }
    
    std::string password(data.begin() + offset, data.begin() + offset + passwordLen);
    
    std::cout << "Login attempt: " << username << std::endl;
    
    // Authenticate
    if (authenticateUser(username, password)) {
        session->authenticated = true;
        session->username = username;
        session->sessionToken = generateSessionToken();
        
        // Store authenticated session
        {
            std::lock_guard<std::mutex> authLock(m_authMutex);
            m_authenticatedSessions[session->sessionToken] = username;
        }
        
        std::cout << "Login successful: " << username << " (token: " << session->sessionToken << ")" << std::endl;
        
        // Send success response with session token
        network::ConnectResponse response;
        response.accepted = true;
        response.assignedPlayerId = sessionId;
        response.message = session->sessionToken; // Send token in message field
        
        auto responseData = response.serialize();
        session->encryption->encrypt(responseData);
        
        uint32_t size = static_cast<uint32_t>(responseData.size());
        std::vector<uint8_t> sizeBytes(4);
        sizeBytes[0] = static_cast<uint8_t>(size & 0xFF);
        sizeBytes[1] = static_cast<uint8_t>((size >> 8) & 0xFF);
        sizeBytes[2] = static_cast<uint8_t>((size >> 16) & 0xFF);
        sizeBytes[3] = static_cast<uint8_t>((size >> 24) & 0xFF);
        
        try {
            asio::write(*session->socket, asio::buffer(sizeBytes));
            asio::write(*session->socket, asio::buffer(responseData));
        } catch (...) {
            session->connected = false;
        }
    } else {
        std::cout << "Login failed: " << username << std::endl;
        
        // Send failure response
        network::ConnectResponse response;
        response.accepted = false;
        response.assignedPlayerId = 0;
        response.message = "Invalid username or password";
        
        auto responseData = response.serialize();
        session->encryption->encrypt(responseData);
        
        uint32_t size = static_cast<uint32_t>(responseData.size());
        std::vector<uint8_t> sizeBytes(4);
        sizeBytes[0] = static_cast<uint8_t>(size & 0xFF);
        sizeBytes[1] = static_cast<uint8_t>((size >> 8) & 0xFF);
        sizeBytes[2] = static_cast<uint8_t>((size >> 16) & 0xFF);
        sizeBytes[3] = static_cast<uint8_t>((size >> 24) & 0xFF);
        
        try {
            asio::write(*session->socket, asio::buffer(sizeBytes));
            asio::write(*session->socket, asio::buffer(responseData));
        } catch (...) {}
        
        session->connected = false;
    }
}

bool LoginServer::authenticateUser(const std::string& username, const std::string& password) {
    std::lock_guard<std::mutex> lock(m_dbMutex);
    auto it = m_userDatabase.find(username);
    if (it == m_userDatabase.end()) {
        return false;
    }
    
    // In production, compare hashed passwords
    return it->second == password;
}

std::string LoginServer::generateSessionToken() {
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    
    // Generate 128-bit token
    for (int i = 0; i < 4; ++i) {
        ss << std::setw(8) << m_rng();
    }
    
    return ss.str();
}

} // namespace server
} // namespace clonemine

#include "CharacterServer.h"
#include "../network/PacketValidator.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <algorithm>

namespace clonemine {
namespace server {

CharacterServer::CharacterServer(uint16_t port, uint32_t maxCharactersPerAccount)
    : m_port(port)
    , m_maxCharactersPerAccount(maxCharactersPerAccount)
    , m_rateLimiter(std::make_unique<RateLimiter>(100, 60)) // 100 requests per 60 seconds
{
    std::cout << "Initializing character server on port " << port << "..." << std::endl;
    std::cout << "Max characters per account: " << maxCharactersPerAccount << std::endl;
    std::cout << "Rate limiting enabled: 100 requests per 60 seconds" << std::endl;
}

CharacterServer::~CharacterServer() {
    stop();
}

void CharacterServer::start() {
    if (m_running) {
        return;
    }
    
    m_running = true;
    
    try {
        // Setup acceptor
        asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), m_port);
        m_acceptor = std::make_unique<asio::ip::tcp::acceptor>(m_ioContext, endpoint);
        
        std::cout << "Character server listening on port " << m_port << std::endl;
        
        // Start accepting connections in a separate thread
        m_networkThread = std::thread([this]() {
            acceptConnections();
            m_ioContext.run();
        });
        
    } catch (const std::exception& e) {
        std::cerr << "Failed to start character server: " << e.what() << std::endl;
        m_running = false;
        throw;
    }
}

void CharacterServer::stop() {
    if (!m_running) {
        return;
    }
    
    std::cout << "Stopping character server..." << std::endl;
    m_running = false;
    
    // Save all account data
    {
        std::lock_guard<std::mutex> lock(m_accountsMutex);
        for (const auto& [username, account] : m_accounts) {
            saveAccountData(account.accountId);
        }
    }
    
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
    
    std::cout << "Character server stopped." << std::endl;
}

void CharacterServer::run() {
    std::cout << "Character server main loop started." << std::endl;
    
    while (m_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Clean up disconnected sessions
        std::lock_guard<std::mutex> lock(m_sessionsMutex);
        for (auto it = m_sessions.begin(); it != m_sessions.end(); ) {
            if (!it->second->connected) {
                std::cout << "Removing disconnected character session " << it->first << std::endl;
                it = m_sessions.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void CharacterServer::acceptConnections() {
    auto socket = std::make_shared<asio::ip::tcp::socket>(m_ioContext);
    
    m_acceptor->async_accept(*socket, [this, socket](const asio::error_code& error) {
        if (!error) {
            std::cout << "New character connection from " << socket->remote_endpoint() << std::endl;
            handleNewConnection(socket);
        } else {
            std::cerr << "Character accept error: " << error.message() << std::endl;
        }
        
        // Continue accepting
        if (m_running) {
            acceptConnections();
        }
    });
}

void CharacterServer::handleNewConnection(std::shared_ptr<asio::ip::tcp::socket> socket) {
    try {
        // Get client IP address for rate limiting
        std::string clientIp = socket->remote_endpoint().address().to_string();
        
        // Check rate limit
        if (!m_rateLimiter->allowRequest(clientIp)) {
            std::cerr << "Rate limit exceeded for IP: " << clientIp << " - connection rejected" << std::endl;
            socket->close();
            return;
        }
        
        auto encryption = std::make_unique<network::PacketEncryption>("CloneMineSharedSecret2024");
        
        // Create session
        uint32_t sessionId = m_nextSessionId++;
        auto session = std::make_unique<CharacterSession>();
        session->sessionId = sessionId;
        session->socket = socket;
        session->encryption = std::move(encryption);
        
        // Add to sessions
        {
            std::lock_guard<std::mutex> lock(m_sessionsMutex);
            m_sessions[sessionId] = std::move(session);
        }
        
        std::cout << "Character session " << sessionId << " created" << std::endl;
        
        // Start message handling thread
        std::thread([this, sessionId]() {
            std::lock_guard<std::mutex> lock(m_sessionsMutex);
            auto it = m_sessions.find(sessionId);
            if (it == m_sessions.end()) return;
            
            auto& session = it->second;
            
            // Receive messages
            while (m_running && session->connected) {
                try {
                    std::vector<uint8_t> sizeBuffer(4);
                    asio::read(*session->socket, asio::buffer(sizeBuffer));
                    
                    uint32_t msgSize = sizeBuffer[0] | (sizeBuffer[1] << 8) |
                                      (sizeBuffer[2] << 16) | (sizeBuffer[3] << 24);
                    
                    if (msgSize == 0 || msgSize > 4096) break;
                    
                    std::vector<uint8_t> data(msgSize);
                    asio::read(*session->socket, asio::buffer(data));
                    session->encryption->decrypt(data);
                    
                    if (data.empty()) break;
                    
                    // Handle based on authentication state and message type
                    if (!session->authenticated) {
                        handleAuthenticationRequest(sessionId, data);
                    } else {
                        // Authenticated - handle character requests
                        uint8_t msgType = data[0];
                        if (msgType == 0x10) { // LIST_CHARACTERS
                            handleCharacterListRequest(sessionId);
                        } else if (msgType == 0x11) { // CREATE_CHARACTER
                            handleCreateCharacterRequest(sessionId, data);
                        } else if (msgType == 0x12) { // SELECT_CHARACTER
                            handleSelectCharacterRequest(sessionId, data);
                        } else if (msgType == 0x13) { // DELETE_CHARACTER
                            handleDeleteCharacterRequest(sessionId, data);
                        } else if (msgType == 0x14) { // LOAD_CHARACTER (from game server)
                            handleLoadCharacterRequest(sessionId, data);
                        } else if (msgType == 0x15) { // SAVE_CHARACTER (from game server)
                            handleSaveCharacterRequest(sessionId, data);
                        } else if (msgType == 0x16) { // UPDATE_CHARACTER (from game server)
                            handleUpdateCharacterRequest(sessionId, data);
                        }
                    }
                } catch (...) {
                    break;
                }
            }
            
            session->connected = false;
        }).detach();
        
    } catch (const std::exception& e) {
        std::cerr << "Error handling character connection: " << e.what() << std::endl;
    }
}

void CharacterServer::handleAuthenticationRequest(uint32_t sessionId, const std::vector<uint8_t>& data) {
    std::lock_guard<std::mutex> lock(m_sessionsMutex);
    auto it = m_sessions.find(sessionId);
    if (it == m_sessions.end()) return;
    
    auto& session = it->second;
    
    // Parse session token (from login server)
    if (data.size() < 5) {
        session->connected = false;
        return;
    }
    
    size_t offset = 1; // Skip message type
    uint32_t tokenLen = data[offset] | (data[offset+1] << 8) |
                       (data[offset+2] << 16) | (data[offset+3] << 24);
    offset += 4;
    
    if (offset + tokenLen > data.size()) {
        session->connected = false;
        return;
    }
    
    std::string token(data.begin() + offset, data.begin() + offset + tokenLen);
    
    std::string username;
    if (validateSessionToken(token, username)) {
        session->authenticated = true;
        session->username = username;
        session->sessionToken = token;
        
        // Load or create account
        loadAccountData(username);
        
        std::lock_guard<std::mutex> accountLock(m_accountsMutex);
        auto accIt = m_accounts.find(username);
        if (accIt != m_accounts.end()) {
            session->accountId = accIt->second.accountId;
        }
        
        std::cout << "Character session authenticated: " << username << std::endl;
        
        // Send success response
        network::ConnectResponse response;
        response.accepted = true;
        response.assignedPlayerId = sessionId;
        response.message = "Authenticated with character server";
        
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
            
            // Automatically send character list
            handleCharacterListRequest(sessionId);
        } catch (...) {
            session->connected = false;
        }
    } else {
        std::cout << "Character session authentication failed: invalid token" << std::endl;
        session->connected = false;
    }
}

void CharacterServer::handleCharacterListRequest(uint32_t sessionId) {
    sendCharacterList(sessionId);
}

void CharacterServer::handleCreateCharacterRequest(uint32_t sessionId, const std::vector<uint8_t>& data) {
    std::lock_guard<std::mutex> sessionLock(m_sessionsMutex);
    auto it = m_sessions.find(sessionId);
    if (it == m_sessions.end() || !it->second->authenticated) return;
    
    auto& session = it->second;
    uint32_t accountId = session->accountId;
    
    // Parse character data (simplified)
    if (data.size() < 10) return;
    
    size_t offset = 1;
    uint32_t nameLen = data[offset] | (data[offset+1] << 8) |
                      (data[offset+2] << 16) | (data[offset+3] << 24);
    offset += 4;
    
    if (offset + nameLen + 4 > data.size()) return;
    
    std::string name(data.begin() + offset, data.begin() + offset + nameLen);
    offset += nameLen;
    
    uint32_t classLen = data[offset] | (data[offset+1] << 8) |
                       (data[offset+2] << 16) | (data[offset+3] << 24);
    offset += 4;
    
    if (offset + classLen > data.size()) return;
    
    std::string className(data.begin() + offset, data.begin() + offset + classLen);
    
    // Create character with full initialization
    character::CharacterData newChar;
    newChar.characterId = m_nextCharacterId++;
    newChar.name = name;
    newChar.className = className;
    newChar.level = 1;
    newChar.experience = 0;
    newChar.experienceToNextLevel = 1000;
    newChar.position = glm::vec3(0.0f, 0.0f, 0.0f);
    newChar.yaw = 0.0f;
    newChar.pitch = 0.0f;
    newChar.health = 100.0f;
    newChar.maxHealth = 100.0f;
    newChar.resource = 100.0f;
    newChar.maxResource = 100.0f;
    newChar.strength = 10;
    newChar.dexterity = 10;
    newChar.constitution = 10;
    newChar.intelligence = 10;
    newChar.wisdom = 10;
    newChar.charisma = 10;
    newChar.armor = 0;
    newChar.magicResist = 0;
    newChar.critChance = 0.05f;
    newChar.critDamage = 1.5f;
    newChar.movementSpeed = 1.0f;
    newChar.gold = 0;
    newChar.gems = 0;
    newChar.maxInventorySize = 40;
    newChar.availableSkillPoints = 0;
    newChar.availableTalentPoints = 0;
    newChar.createdTimestamp = std::chrono::system_clock::now().time_since_epoch().count();
    newChar.lastPlayedTimestamp = newChar.createdTimestamp;
    newChar.lastSavedTimestamp = newChar.createdTimestamp;
    newChar.totalPlayTime = 0;
    newChar.playerKills = 0;
    newChar.deaths = 0;
    newChar.pvpRating = 1000;
    newChar.currentZone = "Starting Zone";
    newChar.lastCity = "Starting City";
    
    if (createCharacter(accountId, newChar)) {
        std::cout << "Created character: " << name << " (ID: " << newChar.characterId << ") for account " << accountId << std::endl;
        // Track character to account mapping
        m_characterIdToAccountId[newChar.characterId] = accountId;
        saveAccountData(accountId);
        sendCharacterList(sessionId);
    }
}

void CharacterServer::handleSelectCharacterRequest(uint32_t sessionId, const std::vector<uint8_t>& data) {
    std::lock_guard<std::mutex> sessionLock(m_sessionsMutex);
    auto it = m_sessions.find(sessionId);
    if (it == m_sessions.end() || !it->second->authenticated) return;
    
    // Parse character ID
    if (data.size() < 5) return;
    
    uint32_t characterId = data[1] | (data[2] << 8) | (data[3] << 16) | (data[4] << 24);
    
    std::cout << "Session " << sessionId << " selected character " << characterId << std::endl;
    
    // TODO: Send character to game server or return character data to client
}

void CharacterServer::handleDeleteCharacterRequest(uint32_t sessionId, const std::vector<uint8_t>& data) {
    std::lock_guard<std::mutex> sessionLock(m_sessionsMutex);
    auto it = m_sessions.find(sessionId);
    if (it == m_sessions.end() || !it->second->authenticated) return;
    
    auto& session = it->second;
    uint32_t accountId = session->accountId;
    
    // Parse character ID
    if (data.size() < 5) return;
    
    uint32_t characterId = data[1] | (data[2] << 8) | (data[3] << 16) | (data[4] << 24);
    
    if (deleteCharacter(accountId, characterId)) {
        std::cout << "Deleted character " << characterId << " from account " << accountId << std::endl;
        saveAccountData(accountId);
        sendCharacterList(sessionId);
    }
}

void CharacterServer::sendCharacterList(uint32_t sessionId) {
    std::lock_guard<std::mutex> sessionLock(m_sessionsMutex);
    auto it = m_sessions.find(sessionId);
    if (it == m_sessions.end() || !it->second->authenticated) return;
    
    auto& session = it->second;
    auto characters = getAccountCharacters(session->accountId);
    
    // Build character list message
    std::vector<uint8_t> message;
    message.push_back(0x20); // CHARACTER_LIST message type
    
    uint32_t count = static_cast<uint32_t>(characters.size());
    message.push_back(static_cast<uint8_t>(count & 0xFF));
    message.push_back(static_cast<uint8_t>((count >> 8) & 0xFF));
    message.push_back(static_cast<uint8_t>((count >> 16) & 0xFF));
    message.push_back(static_cast<uint8_t>((count >> 24) & 0xFF));
    
    for (const auto& character : characters) {
        // Character ID
        uint32_t id = character.characterId;
        message.push_back(static_cast<uint8_t>(id & 0xFF));
        message.push_back(static_cast<uint8_t>((id >> 8) & 0xFF));
        message.push_back(static_cast<uint8_t>((id >> 16) & 0xFF));
        message.push_back(static_cast<uint8_t>((id >> 24) & 0xFF));
        
        // Name
        uint32_t nameLen = static_cast<uint32_t>(character.name.size());
        message.push_back(static_cast<uint8_t>(nameLen & 0xFF));
        message.push_back(static_cast<uint8_t>((nameLen >> 8) & 0xFF));
        message.push_back(static_cast<uint8_t>((nameLen >> 16) & 0xFF));
        message.push_back(static_cast<uint8_t>((nameLen >> 24) & 0xFF));
        message.insert(message.end(), character.name.begin(), character.name.end());
        
        // Class
        uint32_t classLen = static_cast<uint32_t>(character.className.size());
        message.push_back(static_cast<uint8_t>(classLen & 0xFF));
        message.push_back(static_cast<uint8_t>((classLen >> 8) & 0xFF));
        message.push_back(static_cast<uint8_t>((classLen >> 16) & 0xFF));
        message.push_back(static_cast<uint8_t>((classLen >> 24) & 0xFF));
        message.insert(message.end(), character.className.begin(), character.className.end());
        
        // Level
        uint32_t level = character.level;
        message.push_back(static_cast<uint8_t>(level & 0xFF));
        message.push_back(static_cast<uint8_t>((level >> 8) & 0xFF));
        message.push_back(static_cast<uint8_t>((level >> 16) & 0xFF));
        message.push_back(static_cast<uint8_t>((level >> 24) & 0xFF));
    }
    
    session->encryption->encrypt(message);
    
    uint32_t size = static_cast<uint32_t>(message.size());
    std::vector<uint8_t> sizeBytes(4);
    sizeBytes[0] = static_cast<uint8_t>(size & 0xFF);
    sizeBytes[1] = static_cast<uint8_t>((size >> 8) & 0xFF);
    sizeBytes[2] = static_cast<uint8_t>((size >> 16) & 0xFF);
    sizeBytes[3] = static_cast<uint8_t>((size >> 24) & 0xFF);
    
    try {
        asio::write(*session->socket, asio::buffer(sizeBytes));
        asio::write(*session->socket, asio::buffer(message));
        
        std::cout << "Sent character list to session " << sessionId << " (" << characters.size() << " characters)" << std::endl;
    } catch (...) {
        session->connected = false;
    }
}

bool CharacterServer::validateSessionToken(const std::string& token, std::string& outUsername) {
    // In production, validate with login server or shared session storage
    // For now, simplified validation
    (void)token;
    outUsername = "test"; // Placeholder
    return true;
}

void CharacterServer::loadAccountData(const std::string& username) {
    std::lock_guard<std::mutex> lock(m_accountsMutex);
    
    // Check if already loaded
    if (m_accounts.find(username) != m_accounts.end()) {
        return;
    }
    
    // Create new account
    character::Account account;
    account.accountId = m_nextAccountId++;
    account.username = username;
    account.maxCharacters = m_maxCharactersPerAccount;
    account.characterSlots.resize(account.maxCharacters);
    
    // TODO: Load from file/database
    
    m_accounts[username] = account;
    m_accountIdToUsername[account.accountId] = username;
    
    std::cout << "Loaded account: " << username << " (ID: " << account.accountId << ")" << std::endl;
}

void CharacterServer::saveAccountData(uint32_t accountId) {
    std::lock_guard<std::mutex> lock(m_accountsMutex);
    
    auto usernameIt = m_accountIdToUsername.find(accountId);
    if (usernameIt == m_accountIdToUsername.end()) return;
    
    // TODO: Save to file/database
    std::cout << "Saved account data for ID: " << accountId << std::endl;
}

bool CharacterServer::createCharacter(uint32_t accountId, const character::CharacterData& character) {
    std::lock_guard<std::mutex> lock(m_accountsMutex);
    
    auto usernameIt = m_accountIdToUsername.find(accountId);
    if (usernameIt == m_accountIdToUsername.end()) return false;
    
    auto accountIt = m_accounts.find(usernameIt->second);
    if (accountIt == m_accounts.end()) return false;
    
    auto& account = accountIt->second;
    
    // Check if character name is available (global uniqueness check)
    if (!isCharacterNameAvailable(character.name)) {
        std::cerr << "Character name '" << character.name << "' is already taken" << std::endl;
        return false;
    }
    
    // Find available slot
    int slot = account.findAvailableSlot();
    if (slot < 0) {
        std::cerr << "No available character slots for account " << accountId << std::endl;
        return false;
    }
    
    // Reserve the character name
    reserveCharacterName(character.name);
    
    // Add character
    account.characterSlots[slot].occupied = true;
    account.characterSlots[slot].character = character;
    
    return true;
}

bool CharacterServer::deleteCharacter(uint32_t accountId, uint32_t characterId) {
    std::lock_guard<std::mutex> lock(m_accountsMutex);
    
    auto usernameIt = m_accountIdToUsername.find(accountId);
    if (usernameIt == m_accountIdToUsername.end()) return false;
    
    auto accountIt = m_accounts.find(usernameIt->second);
    if (accountIt == m_accounts.end()) return false;
    
    auto& account = accountIt->second;
    
    // Find and delete character
    for (auto& slot : account.characterSlots) {
        if (slot.occupied && slot.character.characterId == characterId) {
            // Release the character name so it can be reused
            releaseCharacterName(slot.character.name);
            
            slot.occupied = false;
            return true;
        }
    }
    
    return false;
}

std::vector<character::CharacterData> CharacterServer::getAccountCharacters(uint32_t accountId) {
    std::lock_guard<std::mutex> lock(m_accountsMutex);
    
    std::vector<character::CharacterData> characters;
    
    auto usernameIt = m_accountIdToUsername.find(accountId);
    if (usernameIt == m_accountIdToUsername.end()) return characters;
    
    auto accountIt = m_accounts.find(usernameIt->second);
    if (accountIt == m_accounts.end()) return characters;
    
    const auto& account = accountIt->second;
    
    for (const auto& slot : account.characterSlots) {
        if (slot.occupied) {
            characters.push_back(slot.character);
        }
    }
    
    return characters;
}

character::CharacterData* CharacterServer::getCharacterState(uint32_t characterId) {
    std::lock_guard<std::mutex> lock(m_accountsMutex);
    
    // Find character by ID
    auto accountIdIt = m_characterIdToAccountId.find(characterId);
    if (accountIdIt == m_characterIdToAccountId.end()) return nullptr;
    
    uint32_t accountId = accountIdIt->second;
    auto usernameIt = m_accountIdToUsername.find(accountId);
    if (usernameIt == m_accountIdToUsername.end()) return nullptr;
    
    auto accountIt = m_accounts.find(usernameIt->second);
    if (accountIt == m_accounts.end()) return nullptr;
    
    auto& account = accountIt->second;
    
    for (auto& slot : account.characterSlots) {
        if (slot.occupied && slot.character.characterId == characterId) {
            return &slot.character;
        }
    }
    
    return nullptr;
}

bool CharacterServer::updateCharacterState(uint32_t characterId, const character::CharacterData& newState) {
    std::lock_guard<std::mutex> lock(m_accountsMutex);
    
    // Find character by ID
    auto accountIdIt = m_characterIdToAccountId.find(characterId);
    if (accountIdIt == m_characterIdToAccountId.end()) return false;
    
    uint32_t accountId = accountIdIt->second;
    auto usernameIt = m_accountIdToUsername.find(accountId);
    if (usernameIt == m_accountIdToUsername.end()) return false;
    
    auto accountIt = m_accounts.find(usernameIt->second);
    if (accountIt == m_accounts.end()) return false;
    
    auto& account = accountIt->second;
    
    for (auto& slot : account.characterSlots) {
        if (slot.occupied && slot.character.characterId == characterId) {
            // Update state
            slot.character = newState;
            slot.character.lastSavedTimestamp = std::chrono::system_clock::now().time_since_epoch().count();
            
            std::cout << "Updated character state for ID " << characterId << std::endl;
            return true;
        }
    }
    
    return false;
}

bool CharacterServer::saveCharacterState(uint32_t characterId) {
    auto accountIdIt = m_characterIdToAccountId.find(characterId);
    if (accountIdIt == m_characterIdToAccountId.end()) return false;
    
    saveAccountData(accountIdIt->second);
    std::cout << "Saved character state for ID " << characterId << std::endl;
    return true;
}

void CharacterServer::setCharacterOnline(uint32_t characterId, bool online) {
    std::lock_guard<std::mutex> lock(m_accountsMutex);
    m_characterOnlineStatus[characterId] = online;
    std::cout << "Character " << characterId << " is now " << (online ? "online" : "offline") << std::endl;
}

void CharacterServer::handleLoadCharacterRequest(uint32_t sessionId, const std::vector<uint8_t>& data) {
    std::lock_guard<std::mutex> sessionLock(m_sessionsMutex);
    auto it = m_sessions.find(sessionId);
    if (it == m_sessions.end() || !it->second->authenticated) return;
    
    // Parse character ID
    if (data.size() < 5) return;
    
    uint32_t characterId = data[1] | (data[2] << 8) | (data[3] << 16) | (data[4] << 24);
    
    std::cout << "Game server requested character " << characterId << std::endl;
    
    sendCharacterData(sessionId, characterId);
}

void CharacterServer::handleSaveCharacterRequest(uint32_t sessionId, const std::vector<uint8_t>& data) {
    (void)sessionId;
    
    // Parse character ID
    if (data.size() < 5) return;
    
    uint32_t characterId = data[1] | (data[2] << 8) | (data[3] << 16) | (data[4] << 24);
    
    saveCharacterState(characterId);
}

void CharacterServer::handleUpdateCharacterRequest(uint32_t sessionId, const std::vector<uint8_t>& data) {
    (void)sessionId;
    
    // Parse character ID and state (simplified - should deserialize full CharacterData)
    if (data.size() < 5) return;
    
    uint32_t characterId = data[1] | (data[2] << 8) | (data[3] << 16) | (data[4] << 24);
    
    // TODO: Deserialize full character state from data
    // For now, just acknowledge
    std::cout << "Received state update for character " << characterId << std::endl;
}

void CharacterServer::sendCharacterData(uint32_t sessionId, uint32_t characterId) {
    auto character = getCharacterState(characterId);
    if (!character) {
        std::cerr << "Character " << characterId << " not found" << std::endl;
        return;
    }
    
    std::lock_guard<std::mutex> sessionLock(m_sessionsMutex);
    auto it = m_sessions.find(sessionId);
    if (it == m_sessions.end()) return;
    
    auto& session = it->second;
    
    // Build character data message (simplified - should serialize full CharacterData)
    std::vector<uint8_t> message;
    message.push_back(0x21); // CHARACTER_DATA message type
    
    // Character ID
    message.push_back(static_cast<uint8_t>(characterId & 0xFF));
    message.push_back(static_cast<uint8_t>((characterId >> 8) & 0xFF));
    message.push_back(static_cast<uint8_t>((characterId >> 16) & 0xFF));
    message.push_back(static_cast<uint8_t>((characterId >> 24) & 0xFF));
    
    // Name
    uint32_t nameLen = static_cast<uint32_t>(character->name.size());
    message.push_back(static_cast<uint8_t>(nameLen & 0xFF));
    message.push_back(static_cast<uint8_t>((nameLen >> 8) & 0xFF));
    message.push_back(static_cast<uint8_t>((nameLen >> 16) & 0xFF));
    message.push_back(static_cast<uint8_t>((nameLen >> 24) & 0xFF));
    message.insert(message.end(), character->name.begin(), character->name.end());
    
    // TODO: Serialize all character data (inventory, equipment, abilities, etc.)
    
    session->encryption->encrypt(message);
    
    uint32_t size = static_cast<uint32_t>(message.size());
    std::vector<uint8_t> sizeBytes(4);
    sizeBytes[0] = static_cast<uint8_t>(size & 0xFF);
    sizeBytes[1] = static_cast<uint8_t>((size >> 8) & 0xFF);
    sizeBytes[2] = static_cast<uint8_t>((size >> 16) & 0xFF);
    sizeBytes[3] = static_cast<uint8_t>((size >> 24) & 0xFF);
    
    try {
        asio::write(*session->socket, asio::buffer(sizeBytes));
        asio::write(*session->socket, asio::buffer(message));
        
        std::cout << "Sent character data for ID " << characterId << " to session " << sessionId << std::endl;
    } catch (...) {
        session->connected = false;
    }
}

// Character name uniqueness methods
bool CharacterServer::isCharacterNameAvailable(const std::string& name) const {
    // Convert to lowercase for case-insensitive check
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    
    return m_usedCharacterNames.find(lowerName) == m_usedCharacterNames.end();
}

void CharacterServer::reserveCharacterName(const std::string& name) {
    // Convert to lowercase for case-insensitive storage
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    
    m_usedCharacterNames.insert(lowerName);
    std::cout << "[CharacterServer] Reserved character name: " << name << std::endl;
}

void CharacterServer::releaseCharacterName(const std::string& name) {
    // Convert to lowercase for case-insensitive removal
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
    
    m_usedCharacterNames.erase(lowerName);
    std::cout << "[CharacterServer] Released character name: " << name << std::endl;
}

} // namespace server
} // namespace clonemine

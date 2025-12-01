#pragma once

#include "../character/CharacterData.h"
#include "../network/NetworkMessage.h"
#include "../network/PacketEncryption.h"
#include <asio.hpp>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

namespace clonemine {
namespace server {

// Character selection session
struct CharacterSession {
    uint32_t sessionId;
    std::string username;
    std::string sessionToken; // From login server
    uint32_t accountId;
    std::shared_ptr<asio::ip::tcp::socket> socket;
    std::unique_ptr<network::PacketEncryption> encryption;
    bool authenticated{false};
    bool connected{true};
};

// Character server allows character selection after login
class CharacterServer {
public:
    explicit CharacterServer(uint16_t port, uint32_t maxCharactersPerAccount = 5);
    ~CharacterServer();
    
    // Delete copy operations
    CharacterServer(const CharacterServer&) = delete;
    CharacterServer& operator=(const CharacterServer&) = delete;
    
    void start();
    void stop();
    void run();
    
    [[nodiscard]] bool isRunning() const { return m_running; }
    
    // Character management
    bool createCharacter(uint32_t accountId, const character::CharacterData& character);
    bool deleteCharacter(uint32_t accountId, uint32_t characterId);
    std::vector<character::CharacterData> getAccountCharacters(uint32_t accountId);
    
    // Character name uniqueness (global across all accounts)
    bool isCharacterNameAvailable(const std::string& name) const;
    void reserveCharacterName(const std::string& name);
    void releaseCharacterName(const std::string& name);
    
    // Character state management (for game server)
    character::CharacterData* getCharacterState(uint32_t characterId);
    bool updateCharacterState(uint32_t characterId, const character::CharacterData& newState);
    bool saveCharacterState(uint32_t characterId);
    void setCharacterOnline(uint32_t characterId, bool online);
    
private:
    void acceptConnections();
    void handleNewConnection(std::shared_ptr<asio::ip::tcp::socket> socket);
    void handleAuthenticationRequest(uint32_t sessionId, const std::vector<uint8_t>& data);
    void handleCharacterListRequest(uint32_t sessionId);
    void handleCreateCharacterRequest(uint32_t sessionId, const std::vector<uint8_t>& data);
    void handleSelectCharacterRequest(uint32_t sessionId, const std::vector<uint8_t>& data);
    void handleDeleteCharacterRequest(uint32_t sessionId, const std::vector<uint8_t>& data);
    void handleLoadCharacterRequest(uint32_t sessionId, const std::vector<uint8_t>& data); // For game server
    void handleSaveCharacterRequest(uint32_t sessionId, const std::vector<uint8_t>& data); // For game server
    void handleUpdateCharacterRequest(uint32_t sessionId, const std::vector<uint8_t>& data); // For game server
    void sendCharacterList(uint32_t sessionId);
    void sendCharacterData(uint32_t sessionId, uint32_t characterId);
    bool validateSessionToken(const std::string& token, std::string& outUsername);
    void loadAccountData(const std::string& username);
    void saveAccountData(uint32_t accountId);
    
    // Network
    asio::io_context m_ioContext;
    std::unique_ptr<asio::ip::tcp::acceptor> m_acceptor;
    std::thread m_networkThread;
    
    // Connected sessions
    std::unordered_map<uint32_t, std::unique_ptr<CharacterSession>> m_sessions;
    std::mutex m_sessionsMutex;
    uint32_t m_nextSessionId{1};
    
    // Account data (username -> Account)
    std::unordered_map<std::string, character::Account> m_accounts;
    std::unordered_map<uint32_t, std::string> m_accountIdToUsername; // accountId -> username
    std::unordered_map<uint32_t, uint32_t> m_characterIdToAccountId; // characterId -> accountId
    std::unordered_map<uint32_t, bool> m_characterOnlineStatus; // characterId -> online
    std::mutex m_accountsMutex;
    uint32_t m_nextAccountId{1};
    uint32_t m_nextCharacterId{1};
    
    // Global character name registry (unique across all accounts)
    std::unordered_set<std::string> m_usedCharacterNames;
    
    // Threading
    std::atomic<bool> m_running{false};
    uint16_t m_port;
    
    // Configuration
    uint32_t m_maxCharactersPerAccount;
};

} // namespace server
} // namespace clonemine

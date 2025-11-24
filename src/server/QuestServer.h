#pragma once

#include "../quest/QuestData.h"
#include "../network/NetworkMessage.h"
#include "../network/PacketEncryption.h"
#include <asio.hpp>
#include <memory>
#include <unordered_map>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

namespace clonemine {
namespace server {

// Quest client session
struct QuestClient {
    uint32_t playerId;
    std::string playerName;
    uint32_t playerLevel;
    std::shared_ptr<asio::ip::tcp::socket> socket;
    std::unique_ptr<network::PacketEncryption> encryption;
    bool connected{true};
};

// Quest server manages all quests and tracks player progress
class QuestServer {
public:
    explicit QuestServer(uint16_t port);
    ~QuestServer();
    
    // Delete copy operations
    QuestServer(const QuestServer&) = delete;
    QuestServer& operator=(const QuestServer&) = delete;
    
    void start();
    void stop();
    void run();
    
    [[nodiscard]] bool isRunning() const { return m_running; }
    
    // Quest management
    void loadQuests();
    void addQuest(const quest::Quest& quest);
    
private:
    void acceptConnections();
    void handleNewConnection(std::shared_ptr<asio::ip::tcp::socket> socket);
    void handleQuestRequest(uint32_t playerId, const std::vector<uint8_t>& data);
    void handleProgressUpdate(uint32_t playerId, const std::vector<uint8_t>& data);
    void sendAvailableQuests(uint32_t playerId);
    void sendQuestProgress(uint32_t playerId, uint32_t questId);
    void updateQuestProgress(uint32_t playerId, uint32_t questId, 
                            const std::string& targetId, uint32_t amount);
    
    // Network
    asio::io_context m_ioContext;
    std::unique_ptr<asio::ip::tcp::acceptor> m_acceptor;
    std::thread m_networkThread;
    
    // Connected clients
    std::unordered_map<uint32_t, std::unique_ptr<QuestClient>> m_clients;
    std::mutex m_clientsMutex;
    uint32_t m_nextClientId{1};
    
    // Quest data
    std::unordered_map<uint32_t, quest::Quest> m_quests;
    std::unordered_map<uint32_t, std::unordered_map<uint32_t, quest::Quest>> m_playerQuests; // playerId -> questId -> Quest
    std::mutex m_questsMutex;
    
    // Threading
    std::atomic<bool> m_running{false};
    uint16_t m_port;
};

} // namespace server
} // namespace clonemine

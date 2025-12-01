#include "QuestServer.h"
#include "../network/PacketValidator.h"
#include <iostream>
#include <fstream>
#include <chrono>

namespace clonemine {
namespace server {

QuestServer::QuestServer(uint16_t port)
    : m_port(port)
{
    std::cout << "Initializing quest server on port " << port << "..." << std::endl;
    loadQuests();
}

QuestServer::~QuestServer() {
    stop();
}

void QuestServer::start() {
    if (m_running) {
        return;
    }
    
    m_running = true;
    
    try {
        // Setup acceptor
        asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), m_port);
        m_acceptor = std::make_unique<asio::ip::tcp::acceptor>(m_ioContext, endpoint);
        
        std::cout << "Quest server listening on port " << m_port << std::endl;
        
        // Start accepting connections in a separate thread
        m_networkThread = std::thread([this]() {
            acceptConnections();
            m_ioContext.run();
        });
        
    } catch (const std::exception& e) {
        std::cerr << "Failed to start quest server: " << e.what() << std::endl;
        m_running = false;
        throw;
    }
}

void QuestServer::stop() {
    if (!m_running) {
        return;
    }
    
    std::cout << "Stopping quest server..." << std::endl;
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
    
    std::cout << "Quest server stopped." << std::endl;
}

void QuestServer::run() {
    std::cout << "Quest server main loop started." << std::endl;
    std::cout << "Loaded " << m_quests.size() << " quests" << std::endl;
    
    while (m_running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Clean up disconnected clients
        std::lock_guard<std::mutex> lock(m_clientsMutex);
        for (auto it = m_clients.begin(); it != m_clients.end(); ) {
            if (!it->second->connected) {
                std::cout << "Removing disconnected quest client " << it->first << std::endl;
                it = m_clients.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void QuestServer::loadQuests() {
    // Load sample quests
    // In production, load from database or JSON file
    
    quest::Quest quest1;
    quest1.questId = 1;
    quest1.title = "Zombie Slayer I";
    quest1.description = "Defeat 10 zombies in the forest";
    quest1.requiredLevel = 1;
    quest1.recommendedLevel = 3;
    
    quest::QuestObjective obj1;
    obj1.description = "Kill zombies";
    obj1.type = quest::QuestType::KILL_MONSTERS;
    obj1.targetId = "zombie";
    obj1.requiredAmount = 10;
    quest1.objectives.push_back(obj1);
    
    quest1.rewards.experience = 100;
    quest1.rewards.gold = 50;
    quest1.rewards.items = {"health_potion"};
    
    addQuest(quest1);
    
    quest::Quest quest2;
    quest2.questId = 2;
    quest2.title = "Gather Resources";
    quest2.description = "Collect 20 wood blocks";
    quest2.requiredLevel = 1;
    quest2.recommendedLevel = 1;
    
    quest::QuestObjective obj2;
    obj2.description = "Collect wood";
    obj2.type = quest::QuestType::COLLECT_ITEMS;
    obj2.targetId = "wood";
    obj2.requiredAmount = 20;
    quest2.objectives.push_back(obj2);
    
    quest2.rewards.experience = 50;
    quest2.rewards.gold = 25;
    
    addQuest(quest2);
    
    quest::Quest quest3;
    quest3.questId = 3;
    quest3.title = "Dungeon Explorer";
    quest3.description = "Explore the dark cavern and defeat the boss";
    quest3.requiredLevel = 5;
    quest3.recommendedLevel = 8;
    
    quest::QuestObjective obj3a;
    obj3a.description = "Explore Dark Cavern";
    obj3a.type = quest::QuestType::EXPLORE_LOCATION;
    obj3a.targetId = "dark_cavern";
    obj3a.requiredAmount = 1;
    quest3.objectives.push_back(obj3a);
    
    quest::QuestObjective obj3b;
    obj3b.description = "Defeat Cave Troll";
    obj3b.type = quest::QuestType::KILL_MONSTERS;
    obj3b.targetId = "cave_troll";
    obj3b.requiredAmount = 1;
    quest3.objectives.push_back(obj3b);
    
    quest3.rewards.experience = 500;
    quest3.rewards.gold = 250;
    quest3.rewards.items = {"rare_sword", "magic_ring"};
    
    addQuest(quest3);
}

void QuestServer::addQuest(const quest::Quest& quest) {
    std::lock_guard<std::mutex> lock(m_questsMutex);
    m_quests[quest.questId] = quest;
}

void QuestServer::acceptConnections() {
    auto socket = std::make_shared<asio::ip::tcp::socket>(m_ioContext);
    
    m_acceptor->async_accept(*socket, [this, socket](const asio::error_code& error) {
        if (!error) {
            std::cout << "New quest connection from " << socket->remote_endpoint() << std::endl;
            handleNewConnection(socket);
        } else {
            std::cerr << "Quest accept error: " << error.message() << std::endl;
        }
        
        // Continue accepting
        if (m_running) {
            acceptConnections();
        }
    });
}

void QuestServer::handleNewConnection(std::shared_ptr<asio::ip::tcp::socket> socket) {
    try {
        auto encryption = std::make_unique<network::PacketEncryption>("CloneMineSharedSecret2024");
        
        // Read connect message
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
        
        // Validate
        auto validation = network::PacketValidator::validatePacket(
            buffer, network::MessageType::CONNECT_REQUEST);
        
        if (validation != network::PacketValidator::ValidationResult::VALID) {
            socket->close();
            return;
        }
        
        // Parse player info
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
        
        // Create quest client
        uint32_t clientId = m_nextClientId++;
        auto client = std::make_unique<QuestClient>();
        client->playerId = clientId;
        client->playerName = playerName;
        client->playerLevel = 1; // TODO: Get from game server
        client->socket = socket;
        client->encryption = std::move(encryption);
        
        // Send acceptance
        network::ConnectResponse response;
        response.accepted = true;
        response.assignedPlayerId = clientId;
        response.message = "Connected to quest server";
        
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
        
        // Send available quests
        sendAvailableQuests(clientId);
        
        // Add to clients
        {
            std::lock_guard<std::mutex> lock(m_clientsMutex);
            m_clients[clientId] = std::move(client);
        }
        
        std::cout << "Quest client " << clientId << " (" << playerName << ") connected" << std::endl;
        
        // Start message handling thread
        std::thread([this, clientId]() {
            // Receive quest requests and progress updates
            std::lock_guard<std::mutex> lock(m_clientsMutex);
            auto it = m_clients.find(clientId);
            if (it == m_clients.end()) return;
            
            auto& client = it->second;
            while (m_running && client->connected) {
                try {
                    std::vector<uint8_t> sizeBuffer(4);
                    asio::read(*client->socket, asio::buffer(sizeBuffer));
                    
                    uint32_t msgSize = sizeBuffer[0] | (sizeBuffer[1] << 8) |
                                      (sizeBuffer[2] << 16) | (sizeBuffer[3] << 24);
                    
                    if (msgSize == 0 || msgSize > 1024) break;
                    
                    std::vector<uint8_t> data(msgSize);
                    asio::read(*client->socket, asio::buffer(data));
                    client->encryption->decrypt(data);
                    
                    // Handle based on message type
                    if (!data.empty()) {
                        // Process quest-related messages
                        // TODO: Add specific quest message types
                    }
                } catch (...) {
                    break;
                }
            }
            
            client->connected = false;
        }).detach();
        
    } catch (const std::exception& e) {
        std::cerr << "Error handling quest connection: " << e.what() << std::endl;
    }
}

void QuestServer::sendAvailableQuests(uint32_t playerId) {
    std::lock_guard<std::mutex> clientLock(m_clientsMutex);
    auto clientIt = m_clients.find(playerId);
    if (clientIt == m_clients.end()) return;
    
    auto& client = clientIt->second;
    
    std::lock_guard<std::mutex> questLock(m_questsMutex);
    for (const auto& [questId, quest] : m_quests) {
        // Check if player meets level requirement
        if (quest.requiredLevel > client->playerLevel) {
            continue;
        }
        
        // TODO: Send quest data as network message
        // For now, just log
        std::cout << "Sending quest " << questId << " (" << quest.title << ") to player " << playerId << std::endl;
    }
}

void QuestServer::handleQuestRequest(uint32_t playerId, const std::vector<uint8_t>& data) {
    // Handle quest accept/complete requests
    (void)playerId;
    (void)data;
}

void QuestServer::handleProgressUpdate(uint32_t playerId, const std::vector<uint8_t>& data) {
    // Handle progress updates from game server
    (void)playerId;
    (void)data;
}

void QuestServer::sendQuestProgress(uint32_t playerId, uint32_t questId) {
    // Send current progress for a quest
    (void)playerId;
    (void)questId;
}

void QuestServer::updateQuestProgress(uint32_t playerId, uint32_t questId,
                                     const std::string& targetId, uint32_t amount) {
    std::lock_guard<std::mutex> lock(m_questsMutex);
    
    auto playerIt = m_playerQuests.find(playerId);
    if (playerIt == m_playerQuests.end()) return;
    
    auto questIt = playerIt->second.find(questId);
    if (questIt == playerIt->second.end()) return;
    
    auto& quest = questIt->second;
    for (auto& objective : quest.objectives) {
        if (objective.targetId == targetId) {
            objective.currentAmount += amount;
            if (objective.currentAmount > objective.requiredAmount) {
                objective.currentAmount = objective.requiredAmount;
            }
            
            std::cout << "Updated quest " << questId << " progress for player " << playerId
                     << ": " << objective.currentAmount << "/" << objective.requiredAmount << std::endl;
            
            // Check if quest is complete
            if (quest.isComplete()) {
                quest.status = quest::QuestStatus::COMPLETED;
                std::cout << "Player " << playerId << " completed quest " << questId << "!" << std::endl;
            }
            break;
        }
    }
}

} // namespace server
} // namespace clonemine

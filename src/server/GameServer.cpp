#include "GameServer.h"
#include "../network/NetworkMessage.h"
#include <iostream>
#include <chrono>
#include <filesystem>

namespace clonemine {
namespace server {

GameServer::GameServer(uint16_t port)
    : m_world(std::make_unique<World>())
    , m_port(port)
{
    std::cout << "Initializing game server on port " << port << "..." << std::endl;
    
    // Create saves directory if it doesn't exist
    std::filesystem::create_directories("server_saves");
}

GameServer::~GameServer() {
    stop();
}

void GameServer::start() {
    if (m_running) {
        return;
    }
    
    m_running = true;
    
    try {
        // Setup acceptor
        asio::ip::tcp::endpoint endpoint(asio::ip::tcp::v4(), m_port);
        m_acceptor = std::make_unique<asio::ip::tcp::acceptor>(m_ioContext, endpoint);
        
        std::cout << "Server listening on port " << m_port << std::endl;
        
        // Start accepting connections in a separate thread
        m_networkThread = std::thread([this]() {
            acceptConnections();
            m_ioContext.run();
        });
        
    } catch (const std::exception& e) {
        std::cerr << "Failed to start server: " << e.what() << std::endl;
        m_running = false;
        throw;
    }
}

void GameServer::stop() {
    if (!m_running) {
        return;
    }
    
    std::cout << "Stopping server..." << std::endl;
    m_running = false;
    
    // Disconnect all players
    for (auto& [id, player] : m_players) {
        savePlayerData(*player);
        player->disconnect();
    }
    m_players.clear();
    
    // Stop network
    if (m_acceptor) {
        m_acceptor->close();
    }
    m_ioContext.stop();
    
    if (m_networkThread.joinable()) {
        m_networkThread.join();
    }
    
    std::cout << "Server stopped." << std::endl;
}

void GameServer::run() {
    using clock = std::chrono::high_resolution_clock;
    auto lastTime = clock::now();
    const float targetFrameTime = 1.0f / 60.0f; // 60 Hz tick rate
    
    std::cout << "Server main loop started." << std::endl;
    
    while (m_running) {
        auto currentTime = clock::now();
        float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        
        if (deltaTime >= targetFrameTime) {
            lastTime = currentTime;
            updateGame(deltaTime);
            broadcastPlayerStates();
        }
        
        // Small sleep to prevent 100% CPU usage
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void GameServer::acceptConnections() {
    auto socket = std::make_shared<asio::ip::tcp::socket>(m_ioContext);
    
    m_acceptor->async_accept(*socket, [this, socket](const asio::error_code& error) {
        if (!error) {
            std::cout << "New connection from " << socket->remote_endpoint() << std::endl;
            handleNewConnection(socket);
        } else {
            std::cerr << "Accept error: " << error.message() << std::endl;
        }
        
        // Continue accepting
        if (m_running) {
            acceptConnections();
        }
    });
}

void GameServer::handleNewConnection(std::shared_ptr<asio::ip::tcp::socket> socket) {
    try {
        // Read connect request (simplified - just read player name)
        std::vector<uint8_t> buffer(256);
        size_t len = socket->read_some(asio::buffer(buffer));
        buffer.resize(len);
        
        // Parse connect request (simplified)
        if (len < 5 || buffer[0] != static_cast<uint8_t>(network::MessageType::CONNECT_REQUEST)) {
            std::cerr << "Invalid connect request" << std::endl;
            socket->close();
            return;
        }
        
        // Extract player name (skip type byte and playerId)
        uint32_t nameLen = buffer[5] | (buffer[6] << 8) | (buffer[7] << 16) | (buffer[8] << 24);
        std::string playerName(buffer.begin() + 9, buffer.begin() + 9 + nameLen);
        
        std::cout << "Player '" << playerName << "' requesting connection" << std::endl;
        
        // Create new player
        uint32_t playerId = m_nextPlayerId++;
        auto player = std::make_unique<ServerPlayer>(playerId, socket);
        player->setName(playerName);
        
        // Try to load saved data
        loadPlayerData(*player, playerName);
        
        // Send connect response
        network::ConnectResponse response;
        response.accepted = true;
        response.assignedPlayerId = playerId;
        response.message = "Welcome to CloneMine Server!";
        
        auto responseData = response.serialize();
        player->sendData(responseData);
        
        // Send player spawn notification to all existing players
        network::PlayerSpawn spawnMsg;
        spawnMsg.playerId = playerId;
        spawnMsg.position = player->getPlayer().getPosition();
        spawnMsg.playerName = playerName;
        spawnMsg.className = player->getPlayer().getClassName();
        auto spawnData = spawnMsg.serialize();
        
        for (auto& [id, existingPlayer] : m_players) {
            existingPlayer->sendData(spawnData);
        }
        
        // Add to active players
        m_players[playerId] = std::move(player);
        
        std::cout << "Player " << playerId << " (" << playerName << ") connected. Total players: " 
                  << m_players.size() << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error handling new connection: " << e.what() << std::endl;
    }
}

void GameServer::updateGame(float deltaTime) {
    // Update world
    m_world->update(deltaTime);
    
    // Update all players
    std::vector<uint32_t> disconnectedPlayers;
    
    for (auto& [id, player] : m_players) {
        if (!player->isConnected()) {
            disconnectedPlayers.push_back(id);
            continue;
        }
        
        player->update(deltaTime);
    }
    
    // Remove disconnected players
    for (uint32_t id : disconnectedPlayers) {
        auto it = m_players.find(id);
        if (it != m_players.end()) {
            std::cout << "Player " << id << " (" << it->second->getName() << ") disconnected" << std::endl;
            savePlayerData(*it->second);
            m_players.erase(it);
        }
    }
}

void GameServer::broadcastPlayerStates() {
    // Send state updates to all players
    for (auto& [id, player] : m_players) {
        if (!player->isConnected()) {
            continue;
        }
        
        // Create state update message
        network::PlayerStateUpdate update;
        update.playerId = id;
        update.position = player->getPlayer().getPosition();
        update.velocity = player->getPlayer().getVelocity();
        update.yaw = player->getPlayer().getYaw();
        update.pitch = player->getPlayer().getPitch();
        update.health = player->getPlayer().getHealth();
        update.resource = player->getPlayer().getResource();
        update.timestamp = static_cast<uint32_t>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()
            ).count()
        );
        
        auto data = update.serialize();
        
        // Send to all other players
        for (auto& [otherId, otherPlayer] : m_players) {
            if (otherId != id && otherPlayer->isConnected()) {
                otherPlayer->sendData(data);
            }
        }
    }
}

void GameServer::savePlayerData(const ServerPlayer& player) {
    PlayerSaveData saveData;
    saveData.position = player.getPlayer().getPosition();
    saveData.className = player.getPlayer().getClassName();
    saveData.level = player.getPlayer().getLevel();
    saveData.health = player.getPlayer().getHealth();
    saveData.maxHealth = player.getPlayer().getMaxHealth();
    saveData.resource = player.getPlayer().getResource();
    saveData.maxResource = player.getPlayer().getMaxResource();
    saveData.experience = 0; // TODO: Add experience tracking
    
    const auto& stats = player.getPlayer().getStats();
    saveData.strength = stats.strength;
    saveData.dexterity = stats.dexterity;
    saveData.constitution = stats.constitution;
    saveData.intelligence = stats.intelligence;
    saveData.wisdom = stats.wisdom;
    saveData.charisma = stats.charisma;
    
    std::vector<ItemData> inventory; // TODO: Serialize inventory
    
    std::string saveSlot = "server_saves/" + player.getName();
    std::filesystem::create_directories(saveSlot);
    
    if (SaveSystem::saveGame(saveSlot, saveData, inventory)) {
        std::cout << "Saved player data for " << player.getName() << std::endl;
    } else {
        std::cerr << "Failed to save player data for " << player.getName() << std::endl;
    }
}

bool GameServer::loadPlayerData(ServerPlayer& player, const std::string& playerName) {
    std::string saveSlot = "server_saves/" + playerName;
    
    PlayerSaveData saveData;
    std::vector<ItemData> inventory;
    
    if (SaveSystem::loadGame(saveSlot, saveData, inventory)) {
        player.getPlayer().setPosition(saveData.position);
        player.getPlayer().setClass(saveData.className);
        player.getPlayer().setLevel(saveData.level);
        player.getPlayer().setHealth(saveData.health);
        player.getPlayer().setResource(saveData.resource);
        
        auto& stats = player.getPlayer().getStats();
        stats.strength = saveData.strength;
        stats.dexterity = saveData.dexterity;
        stats.constitution = saveData.constitution;
        stats.intelligence = saveData.intelligence;
        stats.wisdom = saveData.wisdom;
        stats.charisma = saveData.charisma;
        
        std::cout << "Loaded player data for " << playerName << std::endl;
        return true;
    }
    
    std::cout << "No saved data found for " << playerName << ", using defaults" << std::endl;
    return false;
}

} // namespace server
} // namespace clonemine

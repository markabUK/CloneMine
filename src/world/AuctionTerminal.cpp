#include "AuctionTerminal.h"
#include <iostream>
#include <algorithm>
#include <cmath>

// ============================================================================
// AuctionTerminal Implementation
// ============================================================================

AuctionTerminal::AuctionTerminal()
    : m_position(0.0f, 0.0f, 0.0f)
    , m_state(TerminalState::IDLE)
    , m_auctionServerPort(25569)
    , m_isConnected(false)
    , m_keepaliveTimer(0.0f)
{
}

AuctionTerminal::AuctionTerminal(const glm::vec3& position, const std::string& name)
    : m_name(name)
    , m_position(position)
    , m_state(TerminalState::IDLE)
    , m_auctionServerPort(25569)
    , m_isConnected(false)
    , m_keepaliveTimer(0.0f)
{
}

AuctionTerminal::~AuctionTerminal()
{
    if (!m_currentUserId.empty()) {
        std::cout << "[AuctionTerminal] " << m_name << " destroyed while player " 
                  << m_currentUserName << " was using it" << std::endl;
    }
}

void AuctionTerminal::initialize(const std::string& terminalId, const std::string& mapId)
{
    m_terminalId = terminalId;
    m_mapId = mapId;
    
    std::cout << "[AuctionTerminal] Initialized terminal '" << m_name 
              << "' (ID: " << m_terminalId << ") on map " << m_mapId 
              << " at position (" << m_position.x << ", " << m_position.y 
              << ", " << m_position.z << ")" << std::endl;
}

void AuctionTerminal::setPosition(const glm::vec3& position)
{
    m_position = position;
}

void AuctionTerminal::setMapId(const std::string& mapId)
{
    m_mapId = mapId;
}

void AuctionTerminal::setName(const std::string& name)
{
    m_name = name;
}

void AuctionTerminal::setDisabled(bool disabled)
{
    if (disabled && m_state != TerminalState::DISABLED) {
        if (!m_currentUserId.empty()) {
            onPlayerLeave(m_currentUserId);
        }
        m_state = TerminalState::DISABLED;
        std::cout << "[AuctionTerminal] " << m_name << " disabled" << std::endl;
    } else if (!disabled && m_state == TerminalState::DISABLED) {
        m_state = TerminalState::IDLE;
        std::cout << "[AuctionTerminal] " << m_name << " enabled" << std::endl;
    }
}

void AuctionTerminal::setMaintenance(bool maintenance)
{
    if (maintenance && m_state != TerminalState::MAINTENANCE) {
        if (!m_currentUserId.empty()) {
            onPlayerLeave(m_currentUserId);
        }
        m_state = TerminalState::MAINTENANCE;
        std::cout << "[AuctionTerminal] " << m_name << " under maintenance" << std::endl;
    } else if (!maintenance && m_state == TerminalState::MAINTENANCE) {
        m_state = TerminalState::IDLE;
        std::cout << "[AuctionTerminal] " << m_name << " maintenance complete" << std::endl;
    }
}

bool AuctionTerminal::canInteract(const glm::vec3& playerPosition, float interactionRange) const
{
    if (m_state != TerminalState::IDLE) {
        return false;
    }
    
    // Calculate distance between player and terminal
    float dx = playerPosition.x - m_position.x;
    float dy = playerPosition.y - m_position.y;
    float dz = playerPosition.z - m_position.z;
    float distance = std::sqrt(dx * dx + dy * dy + dz * dz);
    
    return distance <= interactionRange;
}

bool AuctionTerminal::onInteract(const std::string& playerId, const std::string& playerName)
{
    if (m_state != TerminalState::IDLE) {
        std::cout << "[AuctionTerminal] " << m_name << " is not available (state: " 
                  << static_cast<int>(m_state) << ")" << std::endl;
        return false;
    }
    
    m_currentUserId = playerId;
    m_currentUserName = playerName;
    m_state = TerminalState::IN_USE;
    
    std::cout << "[AuctionTerminal] Player '" << playerName << "' started using " 
              << m_name << " on map " << m_mapId << std::endl;
    std::cout << "[AuctionTerminal] Connecting to cross-server auction house..." << std::endl;
    
    // Simulate connection to auction server
    if (!m_auctionServerAddress.empty()) {
        m_isConnected = true;  // In real implementation, would connect asynchronously
        if (m_connectionCallback) {
            m_connectionCallback(true);
        }
    }
    
    return true;
}

void AuctionTerminal::onPlayerLeave(const std::string& playerId)
{
    if (m_currentUserId != playerId) {
        return;
    }
    
    std::cout << "[AuctionTerminal] Player '" << m_currentUserName << "' left " 
              << m_name << std::endl;
    
    m_currentUserId.clear();
    m_currentUserName.clear();
    m_state = TerminalState::IDLE;
}

void AuctionTerminal::setAuctionServerAddress(const std::string& address, uint16_t port)
{
    m_auctionServerAddress = address;
    m_auctionServerPort = port;
}

void AuctionTerminal::setConnectionCallback(ConnectionCallback callback)
{
    m_connectionCallback = callback;
}

void AuctionTerminal::update(float deltaTime)
{
    if (m_isConnected && m_state == TerminalState::IN_USE) {
        m_keepaliveTimer += deltaTime;
        if (m_keepaliveTimer >= KEEPALIVE_INTERVAL) {
            // Send keepalive to auction server
            m_keepaliveTimer = 0.0f;
        }
    }
}

// ============================================================================
// AuctionTerminalManager Implementation
// ============================================================================

AuctionTerminalManager& AuctionTerminalManager::getInstance()
{
    static AuctionTerminalManager instance;
    return instance;
}

AuctionTerminalManager::AuctionTerminalManager()
    : m_auctionServerPort(25569)
    , m_connectedToServer(false)
{
}

AuctionTerminalManager::~AuctionTerminalManager()
{
    shutdown();
}

void AuctionTerminalManager::initialize()
{
    std::cout << "[AuctionTerminalManager] Initializing cross-server auction terminal system" << std::endl;
}

void AuctionTerminalManager::shutdown()
{
    if (m_connectedToServer) {
        disconnectFromAuctionServer();
    }
    m_terminals.clear();
    std::cout << "[AuctionTerminalManager] Shutdown complete" << std::endl;
}

void AuctionTerminalManager::registerTerminal(std::shared_ptr<AuctionTerminal> terminal)
{
    if (!terminal) return;
    
    const std::string& terminalId = terminal->getTerminalId();
    m_terminals[terminalId] = terminal;
    
    // Configure terminal with server connection info
    terminal->setAuctionServerAddress(m_auctionServerAddress, m_auctionServerPort);
    
    std::cout << "[AuctionTerminalManager] Registered terminal '" << terminal->getName() 
              << "' on map " << terminal->getMapId() << std::endl;
}

void AuctionTerminalManager::unregisterTerminal(const std::string& terminalId)
{
    auto it = m_terminals.find(terminalId);
    if (it != m_terminals.end()) {
        std::cout << "[AuctionTerminalManager] Unregistered terminal " << terminalId << std::endl;
        m_terminals.erase(it);
    }
}

std::shared_ptr<AuctionTerminal> AuctionTerminalManager::getTerminal(const std::string& terminalId)
{
    auto it = m_terminals.find(terminalId);
    if (it != m_terminals.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<std::shared_ptr<AuctionTerminal>> AuctionTerminalManager::getTerminalsOnMap(const std::string& mapId)
{
    std::vector<std::shared_ptr<AuctionTerminal>> result;
    for (const auto& [id, terminal] : m_terminals) {
        if (terminal->getMapId() == mapId) {
            result.push_back(terminal);
        }
    }
    return result;
}

std::vector<std::shared_ptr<AuctionTerminal>> AuctionTerminalManager::getAllTerminals()
{
    std::vector<std::shared_ptr<AuctionTerminal>> result;
    for (const auto& [id, terminal] : m_terminals) {
        result.push_back(terminal);
    }
    return result;
}

void AuctionTerminalManager::setAuctionServerAddress(const std::string& address, uint16_t port)
{
    m_auctionServerAddress = address;
    m_auctionServerPort = port;
    
    // Update all registered terminals
    for (auto& [id, terminal] : m_terminals) {
        terminal->setAuctionServerAddress(address, port);
    }
}

void AuctionTerminalManager::connectToAuctionServer()
{
    std::cout << "[AuctionTerminalManager] Connecting to central auction server at " 
              << m_auctionServerAddress << ":" << m_auctionServerPort << std::endl;
    std::cout << "[AuctionTerminalManager] Map server ID: " << m_mapServerId << std::endl;
    
    // In real implementation, would establish TCP/UDP connection
    m_connectedToServer = true;
    
    std::cout << "[AuctionTerminalManager] Connected to cross-server auction house" << std::endl;
    std::cout << "[AuctionTerminalManager] All terminals on this map server now have auction access" << std::endl;
}

void AuctionTerminalManager::disconnectFromAuctionServer()
{
    std::cout << "[AuctionTerminalManager] Disconnecting from auction server" << std::endl;
    m_connectedToServer = false;
}

void AuctionTerminalManager::broadcastAuctionUpdate(const std::string& auctionId, const std::string& updateType)
{
    if (!m_connectedToServer) return;
    
    std::cout << "[AuctionTerminalManager] Broadcasting auction update: " 
              << updateType << " for auction " << auctionId << std::endl;
    
    // Notify all terminals with active users
    for (auto& [id, terminal] : m_terminals) {
        if (terminal->getState() == TerminalState::IN_USE) {
            // Would send update to the player using this terminal
        }
    }
}

void AuctionTerminalManager::notifyAllTerminals(const std::string& message)
{
    std::cout << "[AuctionTerminalManager] Broadcasting to all terminals: " << message << std::endl;
    m_pendingNotifications.push_back(message);
}

void AuctionTerminalManager::setMapServerId(const std::string& serverId)
{
    m_mapServerId = serverId;
}

void AuctionTerminalManager::update(float deltaTime)
{
    for (auto& [id, terminal] : m_terminals) {
        terminal->update(deltaTime);
    }
    
    // Process pending notifications
    m_pendingNotifications.clear();
}

int AuctionTerminalManager::getTerminalCount() const
{
    return static_cast<int>(m_terminals.size());
}

int AuctionTerminalManager::getActiveUserCount() const
{
    int count = 0;
    for (const auto& [id, terminal] : m_terminals) {
        if (terminal->getState() == TerminalState::IN_USE) {
            count++;
        }
    }
    return count;
}

// ============================================================================
// TerminalPlacement Implementation
// ============================================================================

std::vector<TerminalLocation> TerminalPlacement::getDefaultCityTerminals()
{
    return {
        { glm::vec3(100.0f, 0.0f, 50.0f), "city_main", "Market District", "Market Auction Terminal" },
        { glm::vec3(150.0f, 0.0f, 50.0f), "city_main", "Market District", "Trade Hall Terminal" },
        { glm::vec3(200.0f, 0.0f, 100.0f), "city_main", "Bank Quarter", "Bank Auction Kiosk" },
        { glm::vec3(50.0f, 0.0f, 200.0f), "city_main", "Adventurer's Guild", "Guild Auction Terminal" }
    };
}

std::vector<TerminalLocation> TerminalPlacement::getDefaultTownTerminals()
{
    return {
        { glm::vec3(50.0f, 0.0f, 50.0f), "town_square", "Town Center", "Town Auction Board" }
    };
}

std::vector<TerminalLocation> TerminalPlacement::getDefaultOutpostTerminals()
{
    return {
        { glm::vec3(25.0f, 0.0f, 25.0f), "outpost", "Outpost", "Remote Trading Terminal" }
    };
}

void TerminalPlacement::placeTerminalsOnMap(const std::string& mapId, 
                                            const std::vector<TerminalLocation>& locations)
{
    auto& manager = AuctionTerminalManager::getInstance();
    
    int terminalNum = 1;
    for (const auto& loc : locations) {
        auto terminal = std::make_shared<AuctionTerminal>(loc.position, loc.terminalName);
        std::string terminalId = mapId + "_terminal_" + std::to_string(terminalNum++);
        terminal->initialize(terminalId, mapId);
        manager.registerTerminal(terminal);
    }
    
    std::cout << "[TerminalPlacement] Placed " << locations.size() 
              << " auction terminals on map " << mapId << std::endl;
}

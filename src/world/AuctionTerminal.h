#pragma once

#include <string>
#include <glm/glm.hpp>
#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>

// Forward declarations
class AuctionHouse;

/**
 * AuctionTerminal - In-game interactable object for accessing the cross-server auction house
 * 
 * Terminals can be placed on any map and connect to the central auction server.
 * Players interact with terminals to browse, list, and buy auction items.
 */

enum class TerminalState {
    IDLE,           // Not being used
    IN_USE,         // Currently being used by a player
    DISABLED,       // Temporarily disabled
    MAINTENANCE     // Under maintenance
};

struct TerminalLocation {
    glm::vec3 position;
    std::string mapId;
    std::string zoneName;
    std::string terminalName;
};

class AuctionTerminal {
public:
    AuctionTerminal();
    AuctionTerminal(const glm::vec3& position, const std::string& name = "Auction Terminal");
    ~AuctionTerminal();

    // Initialization
    void initialize(const std::string& terminalId, const std::string& mapId);
    
    // Terminal placement
    void setPosition(const glm::vec3& position);
    glm::vec3 getPosition() const { return m_position; }
    
    void setMapId(const std::string& mapId);
    std::string getMapId() const { return m_mapId; }
    
    void setName(const std::string& name);
    std::string getName() const { return m_name; }
    
    std::string getTerminalId() const { return m_terminalId; }
    
    // State management
    TerminalState getState() const { return m_state; }
    bool isAvailable() const { return m_state == TerminalState::IDLE; }
    
    void setDisabled(bool disabled);
    void setMaintenance(bool maintenance);
    
    // Player interaction
    bool canInteract(const glm::vec3& playerPosition, float interactionRange = 5.0f) const;
    bool onInteract(const std::string& playerId, const std::string& playerName);
    void onPlayerLeave(const std::string& playerId);
    
    // Get the player currently using this terminal
    std::string getCurrentUser() const { return m_currentUserId; }
    std::string getCurrentUserName() const { return m_currentUserName; }
    
    // Connection to auction server
    void setAuctionServerAddress(const std::string& address, uint16_t port);
    bool isConnectedToServer() const { return m_isConnected; }
    
    // Connection callbacks
    using ConnectionCallback = std::function<void(bool success)>;
    void setConnectionCallback(ConnectionCallback callback);
    
    // Update (for connection keepalive, etc.)
    void update(float deltaTime);

private:
    std::string m_terminalId;
    std::string m_name;
    std::string m_mapId;
    glm::vec3 m_position;
    TerminalState m_state;
    
    // Current user
    std::string m_currentUserId;
    std::string m_currentUserName;
    
    // Server connection
    std::string m_auctionServerAddress;
    uint16_t m_auctionServerPort;
    bool m_isConnected;
    
    ConnectionCallback m_connectionCallback;
    
    // Connection keepalive
    float m_keepaliveTimer;
    static constexpr float KEEPALIVE_INTERVAL = 30.0f;
};

/**
 * AuctionTerminalManager - Manages all auction terminals across maps
 * 
 * This is used by map servers to track and manage all terminals.
 * Provides cross-server coordination for the auction system.
 */
class AuctionTerminalManager {
public:
    static AuctionTerminalManager& getInstance();
    
    // Initialization
    void initialize();
    void shutdown();
    
    // Terminal registration
    void registerTerminal(std::shared_ptr<AuctionTerminal> terminal);
    void unregisterTerminal(const std::string& terminalId);
    
    // Get terminals
    std::shared_ptr<AuctionTerminal> getTerminal(const std::string& terminalId);
    std::vector<std::shared_ptr<AuctionTerminal>> getTerminalsOnMap(const std::string& mapId);
    std::vector<std::shared_ptr<AuctionTerminal>> getAllTerminals();
    
    // Auction server connection (central cross-server auction house)
    void setAuctionServerAddress(const std::string& address, uint16_t port);
    void connectToAuctionServer();
    void disconnectFromAuctionServer();
    bool isConnectedToAuctionServer() const { return m_connectedToServer; }
    
    // Cross-server messaging
    void broadcastAuctionUpdate(const std::string& auctionId, const std::string& updateType);
    void notifyAllTerminals(const std::string& message);
    
    // Map server identification
    void setMapServerId(const std::string& serverId);
    std::string getMapServerId() const { return m_mapServerId; }
    
    // Update all terminals
    void update(float deltaTime);
    
    // Statistics
    int getTerminalCount() const;
    int getActiveUserCount() const;

private:
    AuctionTerminalManager();
    ~AuctionTerminalManager();
    AuctionTerminalManager(const AuctionTerminalManager&) = delete;
    AuctionTerminalManager& operator=(const AuctionTerminalManager&) = delete;
    
    std::unordered_map<std::string, std::shared_ptr<AuctionTerminal>> m_terminals;
    
    std::string m_auctionServerAddress;
    uint16_t m_auctionServerPort;
    bool m_connectedToServer;
    
    std::string m_mapServerId;
    
    // Pending notifications for cross-server updates
    std::vector<std::string> m_pendingNotifications;
};

/**
 * Terminal placement helper for map designers
 */
struct TerminalPlacement {
    static std::vector<TerminalLocation> getDefaultCityTerminals();
    static std::vector<TerminalLocation> getDefaultTownTerminals();
    static std::vector<TerminalLocation> getDefaultOutpostTerminals();
    static void placeTerminalsOnMap(const std::string& mapId, 
                                    const std::vector<TerminalLocation>& locations);
};

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <chrono>
#include <set>

enum class AuctionStatus {
    ACTIVE,
    SOLD,
    EXPIRED,
    CANCELLED
};

struct AuctionItem {
    std::string auctionId;
    std::string itemId;
    std::string itemName;
    std::string sellerId;
    std::string sellerName;
    uint64_t buyoutPrice;      // In copper (1 gold = 10000 copper)
    std::chrono::system_clock::time_point listingTime;
    std::chrono::system_clock::time_point expirationTime;
    AuctionStatus status;
    int itemLevel;
    std::string itemType;
    std::string rarity;
    int stackSize;
    
    // Cross-server tracking
    std::string originServerId;     // Which server the seller was on
    std::string originMapId;        // Which map the seller listed from
};

struct AuctionSearchQuery {
    std::string nameFilter;
    std::string typeFilter;
    std::string rarityFilter;
    int minLevel = 0;
    int maxLevel = 999;
    uint64_t maxPrice = UINT64_MAX;
    std::string sortBy = "time";  // "time", "price", "level", "name"
    bool ascending = true;
};

// Cross-server map registration
struct MapServerRegistration {
    std::string serverId;
    std::string serverName;
    std::string address;
    uint16_t port;
    std::chrono::system_clock::time_point lastHeartbeat;
    int terminalCount;
    bool isOnline;
};

class AuctionHouse {
public:
    AuctionHouse();
    ~AuctionHouse();

    // Listing management
    std::string listItem(const std::string& sellerId, const std::string& sellerName,
                        const std::string& itemId, const std::string& itemName,
                        uint64_t buyoutPrice, int durationHours);
    
    // Cross-server listing with origin tracking
    std::string listItemCrossServer(const std::string& sellerId, const std::string& sellerName,
                                   const std::string& itemId, const std::string& itemName,
                                   uint64_t buyoutPrice, int durationHours,
                                   const std::string& originServerId, const std::string& originMapId);
    
    bool cancelAuction(const std::string& auctionId, const std::string& sellerId);
    
    // Browsing and searching
    std::vector<AuctionItem> searchAuctions(const AuctionSearchQuery& query) const;
    std::vector<AuctionItem> getActiveAuctions() const;
    AuctionItem getAuction(const std::string& auctionId) const;
    
    // Purchasing
    bool buyoutAuction(const std::string& auctionId, const std::string& buyerId,
                      const std::string& buyerName);
    
    // Auction lifecycle
    void update();  // Process expirations
    std::vector<AuctionItem> getExpiredAuctions() const;
    
    // Statistics
    int getActiveAuctionCount() const;
    int getTotalAuctionsProcessed() const;
    
    // Persistence
    void saveToFile(const std::string& filepath) const;
    void loadFromFile(const std::string& filepath);
    
    // =========================================================================
    // Cross-Server Auction House Support
    // =========================================================================
    
    // Map server registration
    void registerMapServer(const std::string& serverId, const std::string& serverName,
                          const std::string& address, uint16_t port);
    void unregisterMapServer(const std::string& serverId);
    void updateMapServerHeartbeat(const std::string& serverId);
    std::vector<MapServerRegistration> getRegisteredMapServers() const;
    bool isMapServerOnline(const std::string& serverId) const;
    
    // Terminal registration
    void registerTerminal(const std::string& serverId, const std::string& terminalId);
    void unregisterTerminal(const std::string& serverId, const std::string& terminalId);
    int getTerminalCountForServer(const std::string& serverId) const;
    
    // Cross-server notifications
    using AuctionUpdateCallback = std::function<void(const std::string& auctionId, 
                                                     const std::string& updateType)>;
    void setUpdateCallback(AuctionUpdateCallback callback);
    
    // Broadcast to all connected map servers
    void broadcastUpdate(const std::string& auctionId, const std::string& updateType);

private:
    std::unordered_map<std::string, AuctionItem> m_auctions;
    int m_totalProcessed;
    
    // Cross-server registrations
    std::unordered_map<std::string, MapServerRegistration> m_registeredServers;
    std::unordered_map<std::string, std::set<std::string>> m_serverTerminals; // serverId -> terminalIds
    
    AuctionUpdateCallback m_updateCallback;
    
    // Constants
    static constexpr float LISTING_FEE_PERCENT = 0.05f;  // 5%
    static constexpr float SALE_CUT_PERCENT = 0.05f;     // 5%
    static constexpr int SERVER_TIMEOUT_SECONDS = 60;    // Mark offline after 60s no heartbeat
    
    // Helper methods
    std::string generateAuctionId() const;
    uint64_t calculateListingFee(uint64_t buyoutPrice) const;
    uint64_t calculateSellerProceeds(uint64_t buyoutPrice) const;
    void sendMailToSeller(const std::string& sellerId, const std::string& itemId, 
                         uint64_t gold, const std::string& reason);
    void sendMailToBuyer(const std::string& buyerId, const std::string& itemId);
    bool matchesQuery(const AuctionItem& item, const AuctionSearchQuery& query) const;
    void checkServerTimeouts();
};

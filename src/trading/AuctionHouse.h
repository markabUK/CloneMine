#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <chrono>

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

class AuctionHouse {
public:
    AuctionHouse();
    ~AuctionHouse();

    // Listing management
    std::string listItem(const std::string& sellerId, const std::string& sellerName,
                        const std::string& itemId, const std::string& itemName,
                        uint64_t buyoutPrice, int durationHours);
    
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

private:
    std::unordered_map<std::string, AuctionItem> m_auctions;
    int m_totalProcessed;
    
    // Constants
    static constexpr float LISTING_FEE_PERCENT = 0.05f;  // 5%
    static constexpr float SALE_CUT_PERCENT = 0.05f;     // 5%
    
    // Helper methods
    std::string generateAuctionId() const;
    uint64_t calculateListingFee(uint64_t buyoutPrice) const;
    uint64_t calculateSellerProceeds(uint64_t buyoutPrice) const;
    void sendMailToSeller(const std::string& sellerId, const std::string& itemId, 
                         uint64_t gold, const std::string& reason);
    void sendMailToBuyer(const std::string& buyerId, const std::string& itemId);
    bool matchesQuery(const AuctionItem& item, const AuctionSearchQuery& query) const;
};

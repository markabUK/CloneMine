#include "AuctionHouse.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>

AuctionHouse::AuctionHouse() 
    : m_totalProcessed(0) {
    std::cout << "[AuctionHouse] Initialized" << std::endl;
}

AuctionHouse::~AuctionHouse() = default;

std::string AuctionHouse::listItem(const std::string& sellerId, const std::string& sellerName,
                                   const std::string& itemId, const std::string& itemName,
                                   uint64_t buyoutPrice, int durationHours) {
    // Calculate listing fee
    uint64_t listingFee = calculateListingFee(buyoutPrice);
    
    // TODO: Deduct listing fee from seller's gold
    std::cout << "[AuctionHouse] Listing fee: " << listingFee << " copper" << std::endl;
    
    // Create auction
    AuctionItem auction;
    auction.auctionId = generateAuctionId();
    auction.itemId = itemId;
    auction.itemName = itemName;
    auction.sellerId = sellerId;
    auction.sellerName = sellerName;
    auction.buyoutPrice = buyoutPrice;
    auction.listingTime = std::chrono::system_clock::now();
    auction.expirationTime = auction.listingTime + std::chrono::hours(durationHours);
    auction.status = AuctionStatus::ACTIVE;
    auction.stackSize = 1; // TODO: Get from item data
    
    m_auctions[auction.auctionId] = auction;
    
    std::cout << "[AuctionHouse] Listed item '" << itemName << "' for " 
              << buyoutPrice << " copper (Auction ID: " << auction.auctionId << ")" << std::endl;
    
    return auction.auctionId;
}

bool AuctionHouse::cancelAuction(const std::string& auctionId, const std::string& sellerId) {
    auto it = m_auctions.find(auctionId);
    if (it == m_auctions.end()) {
        return false;
    }
    
    if (it->second.sellerId != sellerId) {
        std::cout << "[AuctionHouse] Cancel failed: Not the seller" << std::endl;
        return false;
    }
    
    if (it->second.status != AuctionStatus::ACTIVE) {
        std::cout << "[AuctionHouse] Cancel failed: Auction not active" << std::endl;
        return false;
    }
    
    it->second.status = AuctionStatus::CANCELLED;
    
    // Return item to seller via mail
    sendMailToSeller(sellerId, it->second.itemId, 0, "Auction cancelled");
    
    std::cout << "[AuctionHouse] Cancelled auction " << auctionId << std::endl;
    return true;
}

std::vector<AuctionItem> AuctionHouse::searchAuctions(const AuctionSearchQuery& query) const {
    std::vector<AuctionItem> results;
    
    for (const auto& [id, auction] : m_auctions) {
        if (auction.status == AuctionStatus::ACTIVE && matchesQuery(auction, query)) {
            results.push_back(auction);
        }
    }
    
    // Sort results
    if (query.sortBy == "price") {
        std::sort(results.begin(), results.end(), [&](const AuctionItem& a, const AuctionItem& b) {
            return query.ascending ? a.buyoutPrice < b.buyoutPrice : a.buyoutPrice > b.buyoutPrice;
        });
    } else if (query.sortBy == "level") {
        std::sort(results.begin(), results.end(), [&](const AuctionItem& a, const AuctionItem& b) {
            return query.ascending ? a.itemLevel < b.itemLevel : a.itemLevel > b.itemLevel;
        });
    } else if (query.sortBy == "name") {
        std::sort(results.begin(), results.end(), [&](const AuctionItem& a, const AuctionItem& b) {
            return query.ascending ? a.itemName < b.itemName : a.itemName > b.itemName;
        });
    } else { // Default: sort by time
        std::sort(results.begin(), results.end(), [&](const AuctionItem& a, const AuctionItem& b) {
            return query.ascending ? a.listingTime < b.listingTime : a.listingTime > b.listingTime;
        });
    }
    
    return results;
}

std::vector<AuctionItem> AuctionHouse::getActiveAuctions() const {
    std::vector<AuctionItem> active;
    
    for (const auto& [id, auction] : m_auctions) {
        if (auction.status == AuctionStatus::ACTIVE) {
            active.push_back(auction);
        }
    }
    
    return active;
}

AuctionItem AuctionHouse::getAuction(const std::string& auctionId) const {
    auto it = m_auctions.find(auctionId);
    if (it != m_auctions.end()) {
        return it->second;
    }
    return AuctionItem{}; // Return empty auction if not found
}

bool AuctionHouse::buyoutAuction(const std::string& auctionId, const std::string& buyerId,
                                const std::string& buyerName) {
    auto it = m_auctions.find(auctionId);
    if (it == m_auctions.end()) {
        return false;
    }
    
    if (it->second.status != AuctionStatus::ACTIVE) {
        std::cout << "[AuctionHouse] Purchase failed: Auction not active" << std::endl;
        return false;
    }
    
    // Check if trying to buy own auction
    if (it->second.sellerId == buyerId) {
        std::cout << "[AuctionHouse] Purchase failed: Cannot buy your own auction" << std::endl;
        return false;
    }
    
    // TODO: Deduct gold from buyer
    uint64_t price = it->second.buyoutPrice;
    std::cout << "[AuctionHouse] Buyer " << buyerName << " pays " << price << " copper" << std::endl;
    
    // Calculate seller's proceeds (95% after 5% cut)
    uint64_t sellerProceeds = calculateSellerProceeds(price);
    
    // Update auction status
    it->second.status = AuctionStatus::SOLD;
    m_totalProcessed++;
    
    // Send mail to seller with gold
    sendMailToSeller(it->second.sellerId, "", sellerProceeds, "Auction sold");
    
    // Send mail to buyer with item
    sendMailToBuyer(buyerId, it->second.itemId);
    
    std::cout << "[AuctionHouse] Auction " << auctionId << " sold! Buyer: " << buyerName 
              << ", Seller gets: " << sellerProceeds << " copper" << std::endl;
    
    return true;
}

void AuctionHouse::update() {
    auto now = std::chrono::system_clock::now();
    
    for (auto& [id, auction] : m_auctions) {
        if (auction.status == AuctionStatus::ACTIVE && now >= auction.expirationTime) {
            auction.status = AuctionStatus::EXPIRED;
            m_totalProcessed++;
            
            // Return item to seller via mail
            sendMailToSeller(auction.sellerId, auction.itemId, 0, "Auction expired");
            
            std::cout << "[AuctionHouse] Auction " << id << " expired" << std::endl;
        }
    }
}

std::vector<AuctionItem> AuctionHouse::getExpiredAuctions() const {
    std::vector<AuctionItem> expired;
    
    for (const auto& [id, auction] : m_auctions) {
        if (auction.status == AuctionStatus::EXPIRED) {
            expired.push_back(auction);
        }
    }
    
    return expired;
}

int AuctionHouse::getActiveAuctionCount() const {
    int count = 0;
    for (const auto& [id, auction] : m_auctions) {
        if (auction.status == AuctionStatus::ACTIVE) {
            count++;
        }
    }
    return count;
}

int AuctionHouse::getTotalAuctionsProcessed() const {
    return m_totalProcessed;
}

void AuctionHouse::saveToFile(const std::string& filepath) const {
    // TODO: Implement JSON serialization
    std::cout << "[AuctionHouse] Saving " << m_auctions.size() << " auctions to " << filepath << std::endl;
}

void AuctionHouse::loadFromFile(const std::string& filepath) {
    // TODO: Implement JSON deserialization
    std::cout << "[AuctionHouse] Loading auctions from " << filepath << std::endl;
}

std::string AuctionHouse::generateAuctionId() const {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    
    std::stringstream ss;
    ss << "AH";
    for (int i = 0; i < 16; ++i) {
        ss << std::hex << dis(gen);
    }
    return ss.str();
}

uint64_t AuctionHouse::calculateListingFee(uint64_t buyoutPrice) const {
    return static_cast<uint64_t>(buyoutPrice * LISTING_FEE_PERCENT);
}

uint64_t AuctionHouse::calculateSellerProceeds(uint64_t buyoutPrice) const {
    return static_cast<uint64_t>(buyoutPrice * (1.0f - SALE_CUT_PERCENT));
}

void AuctionHouse::sendMailToSeller(const std::string& sellerId, const std::string& itemId,
                                   uint64_t gold, const std::string& reason) {
    // TODO: Integrate with mail system
    std::cout << "[AuctionHouse] Sending mail to " << sellerId << ": " << reason;
    if (!itemId.empty()) {
        std::cout << " (Item: " << itemId << ")";
    }
    if (gold > 0) {
        std::cout << " (Gold: " << gold << " copper)";
    }
    std::cout << std::endl;
}

void AuctionHouse::sendMailToBuyer(const std::string& buyerId, const std::string& itemId) {
    // TODO: Integrate with mail system
    std::cout << "[AuctionHouse] Sending mail to " << buyerId << " (Item: " << itemId << ")" << std::endl;
}

bool AuctionHouse::matchesQuery(const AuctionItem& item, const AuctionSearchQuery& query) const {
    // Name filter
    if (!query.nameFilter.empty()) {
        std::string lowerName = item.itemName;
        std::string lowerFilter = query.nameFilter;
        std::transform(lowerName.begin(), lowerName.end(), lowerName.begin(), ::tolower);
        std::transform(lowerFilter.begin(), lowerFilter.end(), lowerFilter.begin(), ::tolower);
        
        if (lowerName.find(lowerFilter) == std::string::npos) {
            return false;
        }
    }
    
    // Type filter
    if (!query.typeFilter.empty() && item.itemType != query.typeFilter) {
        return false;
    }
    
    // Rarity filter
    if (!query.rarityFilter.empty() && item.rarity != query.rarityFilter) {
        return false;
    }
    
    // Level filter
    if (item.itemLevel < query.minLevel || item.itemLevel > query.maxLevel) {
        return false;
    }
    
    // Price filter
    if (item.buyoutPrice > query.maxPrice) {
        return false;
    }
    
    return true;
}

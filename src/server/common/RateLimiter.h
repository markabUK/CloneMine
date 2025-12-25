#pragma once

#include <string>
#include <unordered_map>
#include <chrono>
#include <mutex>

namespace clonemine {
namespace server {

// Rate limiter using token bucket algorithm to prevent DOS attacks
class RateLimiter {
public:
    // Constructor
    // maxRequests: maximum number of requests allowed in the time window
    // windowSeconds: time window in seconds for rate limiting
    explicit RateLimiter(size_t maxRequests = 100, size_t windowSeconds = 60);
    
    // Check if a request from the given IP address should be allowed
    // Returns true if allowed, false if rate limit exceeded
    bool allowRequest(const std::string& ipAddress);
    
    // Reset rate limit for a specific IP address
    void resetIp(const std::string& ipAddress);
    
    // Clear all rate limit data (useful for testing or maintenance)
    void clearAll();
    
    // Get current request count for an IP address
    size_t getRequestCount(const std::string& ipAddress) const;

private:
    struct ClientInfo {
        size_t requestCount;
        std::chrono::steady_clock::time_point windowStart;
    };
    
    void cleanupExpiredEntries();
    
    size_t m_maxRequests;
    std::chrono::seconds m_windowDuration;
    mutable std::mutex m_mutex;
    std::unordered_map<std::string, ClientInfo> m_clients;
    std::chrono::steady_clock::time_point m_lastCleanup;
};

} // namespace server
} // namespace clonemine

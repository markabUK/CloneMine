#include "RateLimiter.h"
#include <iostream>

namespace clonemine {
namespace server {

RateLimiter::RateLimiter(size_t maxRequests, size_t windowSeconds)
    : m_maxRequests(maxRequests)
    , m_windowDuration(std::chrono::seconds(windowSeconds))
    , m_lastCleanup(std::chrono::steady_clock::now())
{
}

bool RateLimiter::allowRequest(const std::string& ipAddress) {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    auto now = std::chrono::steady_clock::now();
    
    // Periodic cleanup of expired entries (every 5 minutes)
    if (now - m_lastCleanup > std::chrono::minutes(5)) {
        cleanupExpiredEntries();
        m_lastCleanup = now;
    }
    
    auto it = m_clients.find(ipAddress);
    
    if (it == m_clients.end()) {
        // New client, create entry
        m_clients[ipAddress] = ClientInfo{1, now};
        return true;
    }
    
    auto& clientInfo = it->second;
    auto elapsed = now - clientInfo.windowStart;
    
    if (elapsed >= m_windowDuration) {
        // Window expired, reset counter
        clientInfo.requestCount = 1;
        clientInfo.windowStart = now;
        return true;
    }
    
    // Within the window, check if limit exceeded
    if (clientInfo.requestCount >= m_maxRequests) {
        std::cout << "[RateLimiter] Rate limit exceeded for IP: " << ipAddress 
                  << " (requests: " << clientInfo.requestCount << ")" << std::endl;
        return false;
    }
    
    // Increment counter
    clientInfo.requestCount++;
    return true;
}

void RateLimiter::resetIp(const std::string& ipAddress) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_clients.erase(ipAddress);
}

void RateLimiter::clearAll() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_clients.clear();
}

size_t RateLimiter::getRequestCount(const std::string& ipAddress) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    auto it = m_clients.find(ipAddress);
    if (it == m_clients.end()) {
        return 0;
    }
    return it->second.requestCount;
}

void RateLimiter::cleanupExpiredEntries() {
    auto now = std::chrono::steady_clock::now();
    
    for (auto it = m_clients.begin(); it != m_clients.end(); ) {
        auto elapsed = now - it->second.windowStart;
        if (elapsed >= m_windowDuration * 2) {
            // Remove entries that are expired for more than 2x the window duration
            it = m_clients.erase(it);
        } else {
            ++it;
        }
    }
}

} // namespace server
} // namespace clonemine

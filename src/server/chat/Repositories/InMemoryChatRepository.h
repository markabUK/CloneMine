#pragma once

#include "../Interfaces/IChatRepository.h"
#include "../Models/ChatMessage.h"
#include <vector>
#include <mutex>
#include <algorithm>

namespace CloneMine {
namespace Chat {

/**
 * @brief In-memory implementation of chat repository with thread safety
 * 
 * Follows Single Responsibility Principle - only manages message storage
 * Follows Dependency Inversion Principle - implements IChatRepository interface
 */
class InMemoryChatRepository : public IChatRepository {
private:
    std::vector<ChatMessage> messages;
    mutable std::mutex mutex;
    int maxSize;

public:
    explicit InMemoryChatRepository(int maxSize = 100) : maxSize(maxSize) {}

    void AddMessage(const ChatMessage& message) override {
        std::lock_guard<std::mutex> lock(mutex);
        messages.push_back(message);
        
        // Keep only the last maxSize messages
        if (messages.size() > static_cast<size_t>(maxSize)) {
            messages.erase(messages.begin());
        }
    }

    std::vector<ChatMessage> GetHistory(int count) override {
        std::lock_guard<std::mutex> lock(mutex);
        
        if (count <= 0 || messages.empty()) {
            return {};
        }
        
        int actualCount = std::min(count, static_cast<int>(messages.size()));
        int startIndex = messages.size() - actualCount;
        
        return std::vector<ChatMessage>(messages.begin() + startIndex, messages.end());
    }

    void ClearHistory() override {
        std::lock_guard<std::mutex> lock(mutex);
        messages.clear();
    }

    int GetMessageCount() const override {
        std::lock_guard<std::mutex> lock(mutex);
        return static_cast<int>(messages.size());
    }
};

} // namespace Chat
} // namespace CloneMine

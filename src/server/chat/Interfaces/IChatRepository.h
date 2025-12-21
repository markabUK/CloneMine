#pragma once

#include "../Models/ChatMessage.h"
#include <vector>
#include <memory>

namespace CloneMine {
namespace Chat {

/**
 * @brief Interface for chat message repository
 * 
 * Follows Interface Segregation Principle - focused contract for message storage
 */
class IChatRepository {
public:
    virtual ~IChatRepository() = default;

    virtual void AddMessage(const ChatMessage& message) = 0;
    virtual std::vector<ChatMessage> GetHistory(int count) = 0;
    virtual void ClearHistory() = 0;
    virtual int GetMessageCount() const = 0;
};

} // namespace Chat
} // namespace CloneMine

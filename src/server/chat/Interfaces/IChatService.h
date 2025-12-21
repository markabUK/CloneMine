#pragma once

#include "../Models/ChatMessage.h"
#include <string>
#include <memory>

namespace CloneMine {
namespace Chat {

/**
 * @brief Interface for chat service operations
 * 
 * Follows Interface Segregation Principle - focused contract for chat business logic
 */
class IChatService {
public:
    virtual ~IChatService() = default;

    virtual void SendMessage(const std::string& sender, const std::string& content) = 0;
    virtual void BroadcastMessage(const std::string& sender, const std::string& content) = 0;
    virtual std::string GetHistory(int count) = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;
};

} // namespace Chat
} // namespace CloneMine

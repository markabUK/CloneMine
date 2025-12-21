#pragma once

#include <string>
#include <ctime>

namespace CloneMine {
namespace Chat {

/**
 * @brief ChatMessage model representing a chat message
 * 
 * Follows Single Responsibility Principle - only holds chat message data
 */
class ChatMessage {
public:
    std::string sender;
    std::string content;
    time_t timestamp;

    ChatMessage() : sender(""), content(""), timestamp(0) {}

    ChatMessage(const std::string& sender, const std::string& content, time_t timestamp)
        : sender(sender), content(content), timestamp(timestamp) {}

    ChatMessage(const std::string& sender, const std::string& content)
        : sender(sender), content(content), timestamp(std::time(nullptr)) {}
};

} // namespace Chat
} // namespace CloneMine

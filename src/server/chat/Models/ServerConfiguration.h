#pragma once

#include <string>

namespace CloneMine {
namespace Chat {

/**
 * @brief ServerConfiguration model for chat server settings
 * 
 * Follows Single Responsibility Principle - only holds configuration data
 */
class ServerConfiguration {
public:
    int port;
    std::string encryptionKey;
    int maxHistorySize;
    int maxMessageLength;

    ServerConfiguration()
        : port(25566),
          encryptionKey("default_key_change_in_production"),
          maxHistorySize(100),
          maxMessageLength(500) {}

    ServerConfiguration(int port, const std::string& key, int maxHistory, int maxMsgLen)
        : port(port),
          encryptionKey(key),
          maxHistorySize(maxHistory),
          maxMessageLength(maxMsgLen) {}
};

} // namespace Chat
} // namespace CloneMine

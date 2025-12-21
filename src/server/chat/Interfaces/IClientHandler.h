#pragma once

#include <string>
#include <memory>

namespace CloneMine {
namespace Chat {

/**
 * @brief Interface for client connection handling
 * 
 * Follows Interface Segregation Principle - focused contract for client lifecycle
 */
class IClientHandler {
public:
    virtual ~IClientHandler() = default;

    virtual void HandleClient(int clientSocket) = 0;
    virtual void SendMessage(const std::string& message) = 0;
    virtual void Disconnect() = 0;
    virtual bool IsConnected() const = 0;
};

} // namespace Chat
} // namespace CloneMine

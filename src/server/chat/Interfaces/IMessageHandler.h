#pragma once

#include <string>
#include <memory>

namespace CloneMine {
namespace Chat {

/**
 * @brief Interface for message protocol handling
 * 
 * Follows Interface Segregation Principle - focused contract for message processing
 */
class IMessageHandler {
public:
    virtual ~IMessageHandler() = default;

    virtual std::string HandleMessage(const std::string& message) = 0;
    virtual bool IsValidCommand(const std::string& command) const = 0;
};

} // namespace Chat
} // namespace CloneMine

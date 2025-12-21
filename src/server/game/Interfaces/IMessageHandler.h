#pragma once

#include <string>

namespace CloneMine {
namespace Game {
namespace Interfaces {

class IMessageHandler {
public:
    virtual ~IMessageHandler() = default;
    
    virtual std::string HandleMessage(const std::string& message, const std::string& clientId) = 0;
};

} // namespace Interfaces
} // namespace Game
} // namespace CloneMine

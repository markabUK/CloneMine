#pragma once

#include <string>

namespace CloneMine {
namespace Game {
namespace Interfaces {

class IClientHandler {
public:
    virtual ~IClientHandler() = default;
    
    virtual void HandleClient(int clientSocket) = 0;
    virtual void SendMessage(const std::string& message) = 0;
    virtual std::string ReceiveMessage() = 0;
    virtual void CloseConnection() = 0;
};

} // namespace Interfaces
} // namespace Game
} // namespace CloneMine

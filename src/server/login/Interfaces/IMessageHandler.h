#pragma once

#include "../Models/LoginSession.h"
#include <vector>
#include <cstdint>

namespace clonemine {
namespace server {
namespace login {

/// <summary>
/// Interface for message handling
/// Interface Segregation: Focused on message processing only
/// </summary>
class IMessageHandler {
public:
    virtual ~IMessageHandler() = default;
    
    /// <summary>Handle incoming login request message</summary>
    virtual void handleLoginRequest(LoginSession& session, const std::vector<uint8_t>& data) = 0;
    
    /// <summary>Handle handshake challenge response</summary>
    virtual void handleHandshakeResponse(LoginSession& session, const std::vector<uint8_t>& data) = 0;
    
    /// <summary>Send response message to client</summary>
    virtual void sendResponse(LoginSession& session, bool success, const std::string& message, const std::string& token = "") = 0;
};

} // namespace login
} // namespace server
} // namespace clonemine

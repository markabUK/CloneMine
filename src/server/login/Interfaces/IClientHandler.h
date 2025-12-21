#pragma once

#include "../Models/LoginSession.h"
#include <asio.hpp>
#include <memory>

namespace clonemine {
namespace server {
namespace login {

/// <summary>
/// Interface for client connection handling
/// Interface Segregation: Focused on client lifecycle only
/// </summary>
class IClientHandler {
public:
    virtual ~IClientHandler() = default;
    
    /// <summary>Handle new client connection</summary>
    virtual void handleNewConnection(std::shared_ptr<asio::ip::tcp::socket> socket) = 0;
    
    /// <summary>Disconnect client session</summary>
    virtual void disconnectSession(uint32_t sessionId) = 0;
    
    /// <summary>Get session by ID</summary>
    virtual LoginSession* getSession(uint32_t sessionId) = 0;
    
    /// <summary>Clean up disconnected sessions</summary>
    virtual void cleanupDisconnectedSessions() = 0;
};

} // namespace login
} // namespace server
} // namespace clonemine

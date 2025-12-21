#pragma once

#include "../Models/LoginResult.h"
#include <string>

namespace clonemine {
namespace server {
namespace login {

/// <summary>
/// Interface for authentication operations
/// Interface Segregation: Focused on authentication only
/// </summary>
class IAuthenticationService {
public:
    virtual ~IAuthenticationService() = default;
    
    /// <summary>Authenticate user with username and password</summary>
    virtual LoginResult authenticate(const std::string& username, const std::string& password) = 0;
    
    /// <summary>Validate session token</summary>
    virtual bool validateSessionToken(const std::string& token, std::string& outUsername) = 0;
    
    /// <summary>Invalidate/logout session token</summary>
    virtual void invalidateSessionToken(const std::string& token) = 0;
    
    /// <summary>Register new account</summary>
    virtual LoginResult registerAccount(const std::string& username, const std::string& password) = 0;
};

} // namespace login
} // namespace server
} // namespace clonemine

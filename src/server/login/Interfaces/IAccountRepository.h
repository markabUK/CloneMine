#pragma once

#include "../Models/Account.h"
#include <string>
#include <vector>
#include <optional>

namespace clonemine {
namespace server {
namespace login {

/// <summary>
/// Interface for account data access
/// Interface Segregation: Focused on account persistence only
/// Dependency Inversion: Business logic depends on this abstraction
/// </summary>
class IAccountRepository {
public:
    virtual ~IAccountRepository() = default;
    
    /// <summary>Get account by username</summary>
    virtual std::optional<Account> getByUsername(const std::string& username) = 0;
    
    /// <summary>Get account by ID</summary>
    virtual std::optional<Account> getById(uint32_t id) = 0;
    
    /// <summary>Create new account</summary>
    virtual bool create(const Account& account) = 0;
    
    /// <summary>Update existing account</summary>
    virtual bool update(const Account& account) = 0;
    
    /// <summary>Delete account</summary>
    virtual bool remove(uint32_t id) = 0;
    
    /// <summary>Check if username exists</summary>
    virtual bool exists(const std::string& username) = 0;
    
    /// <summary>Get all accounts (for admin purposes)</summary>
    virtual std::vector<Account> getAll() = 0;
};

} // namespace login
} // namespace server
} // namespace clonemine

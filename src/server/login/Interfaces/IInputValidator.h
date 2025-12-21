#pragma once

#include <string>
#include <utility>

namespace clonemine {
namespace server {
namespace login {

/// <summary>
/// Interface for input validation
/// Interface Segregation: Focused on validation only
/// </summary>
class IInputValidator {
public:
    virtual ~IInputValidator() = default;
    
    /// <summary>Validate username format and length</summary>
    /// <returns>pair of (isValid, errorMessage)</returns>
    virtual std::pair<bool, std::string> validateUsername(const std::string& username) = 0;
    
    /// <summary>Validate password format and length</summary>
    /// <returns>pair of (isValid, errorMessage)</returns>
    virtual std::pair<bool, std::string> validatePassword(const std::string& password) = 0;
    
    /// <summary>Check for SQL injection patterns</summary>
    virtual bool containsSqlInjection(const std::string& input) = 0;
    
    /// <summary>Check for null bytes</summary>
    virtual bool containsNullBytes(const std::string& input) = 0;
    
    /// <summary>Validate message size</summary>
    virtual bool isValidMessageSize(size_t size) = 0;
};

} // namespace login
} // namespace server
} // namespace clonemine
